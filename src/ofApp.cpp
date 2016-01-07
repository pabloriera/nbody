#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){


	ofBackgroundHex(0x000000);
	ofSetFrameRate(60);
	ofSetVerticalSync(true);

	// set the camera distance
    camDist  = 2000;
	camera.setDistance(camDist);

    particleSize = 4.0;
    dt = 0.004f;
    numParticles = 50*50;
    numParticles = 2*2;



    // Width and Heigth of the windows
    width = 100;
    height = 100;


    shadersFolder="shaders";


    // Loading the Shaders
    updatePos.load("",shadersFolder+"/posUpdate.frag");// shader for updating the texture that store the particles position on RG channels
    updateVel.load("",shadersFolder+"/velUpdate.frag");// shader for updating the texture that store the particles velocity on RG channels
    forceSplatter.load(shadersFolder+"/splatter");
    forceSplatterRender.load(shadersFolder+"/splatterRender");
    render.load(shadersFolder+"/render");


    // Seting the textures where the information ( position and velocity ) will be
    textureRes = (int)sqrt((float)numParticles);
    numParticles = textureRes * textureRes;

    fboForces.allocate(textureRes,textureRes,GL_RGB32F_ARB);

    fboForces.begin();
        ofBackground(0);
    fboForces.end();

    zerostate(0);

    int xm = textureRes;
    float xm2 = 1.0;

    vector<ofFloatColor> colors;


//    Lists of points, sizes and colors, to build the vbo used for the particles

    for(int x = 0; x < textureRes; x++){
        for(int y = 0; y < textureRes; y++){

        ofVec3f p(x,y);
        points.push_back(p);
        // we are passing the size in as a normal x position

        sizes.push_back(ofVec3f(particleSize));

        colors.push_back(ofFloatColor((float)x/(textureRes-1.0),(float)y/(textureRes-1.0),1-(float)y/(textureRes-1.0) ) );

        }
    }


	vbo.setVertexData(&points[0], numParticles, GL_STATIC_DRAW);
	vbo.setNormalData(&sizes[0], numParticles, GL_STATIC_DRAW);
    vbo.setColorData(&colors[0], numParticles, GL_STATIC_DRAW);

    float d = 1.0;

    // The forces uses a vbo made from a mesh of numParticles quads, the normal and the blue channel retains the particle, and the vertex are all the same (0,0),(0,1),(1,1),(1,0) that will be used to look the other particles position

    for(int n = 0; n < numParticles; n++){

        float c = ofMap(n,0.0,numParticles-1,0.0,1.0);

        mesh.addNormal(points[n]);
        mesh.addColor(ofFloatColor(0.0,0.0,c));
        mesh.addVertex(ofVec3f(0.0,0.0));

        mesh.addNormal(points[n]);
        mesh.addColor(ofFloatColor(0.0,1.0,c));
        mesh.addVertex(ofVec3f(0.0,d));

        mesh.addNormal(points[n]);
        mesh.addColor(ofFloatColor(1.0,1.0,c));
        mesh.addVertex(ofVec3f(d,d));

        mesh.addNormal(points[n]);
        mesh.addColor(ofFloatColor(1.0,0.0,c));
        mesh.addVertex(ofVec3f(d,0.0));
    }

    vboSplatter.setMesh(mesh,GL_STATIC_DRAW);

    bForceSplatter = false;

}

//--------------------------------------------------------------
void ofApp::update(){

        // In each cycle it�s going to update the velocity first and the the position
    // Each one one with a different shader and FBO.
    // Because on GPU you can�t write over the texture that you are reading we are
    // using to pair of ofFbo attached together on what we call pingPongBuffer
    // Learn more about Ping-Pong at:
    //
    // http://www.comp.nus.edu/~ashwinna/docs/PingPong_FBO.pdf
    // http://www.seas.upenn.edu/~cis565/fbo.htm#setupgl4

    // Velocities PingPong
    //
    // It calculates the next frame and see if it�s there any collition
    // then updates the velocity with that information
    //
    velPingPong.dst->begin();
    ofClear(0);
    updateVel.begin();
    updateVel.setUniformTexture("backbuffer", velPingPong.src->getTexture(), 0);   // passing the previus velocity information
    updateVel.setUniformTexture("posData", posPingPong.src->getTexture(), 1);  // passing the position information
    updateVel.setUniformTexture("forceData", fboForces.getTexture(), 2);
    updateVel.setUniform2f("screen", (float)width, (float)height);
    updateVel.setUniform1f("dt", (float)dt);

    // draw the source velocity texture to be updated
    velPingPong.src->draw(0, 0);

    updateVel.end();
    velPingPong.dst->end();

    velPingPong.swap();


    // Positions PingPong
    //
    // With the velocity calculated updates the position
    //
    posPingPong.dst->begin();
    ofClear(0);
    updatePos.begin();
    updatePos.setUniformTexture("prevPosData", posPingPong.src->getTexture(), 0); // Previus position
    updatePos.setUniformTexture("velData", velPingPong.src->getTexture(), 1);  // Velocity
    updatePos.setUniformTexture("forceData", fboForces.getTexture(), 2);
    updatePos.setUniform1f("dt",(float) dt );
    updatePos.setUniform2f("u_mouse",(float) ofMap(mouseX,0,ofGetWidth(),-1,1), ofMap(mouseY,0,ofGetHeight(),1,-1 ) ) ;

    // draw the source position texture to be updated
    posPingPong.src->draw(0, 0);

    updatePos.end();
    posPingPong.dst->end();

    posPingPong.swap();



    fboForces.begin();
//    ofClear(0,0,0,0);
    fboForces.end();


    if(bForceSplatter)
    {

        ///////////////////////////////////////////////////////////////////////////////////
        // Here the fboForces should have blended all the quads from forceSplatter to finally get all the summed forces, but
        // this blend mode is not taking action, if commented/uncommented behaves the same
        ofEnableBlendMode(OF_BLENDMODE_ADD);
        //////////////////////////////////////////////////////////////////////////////////

        fboForces.begin();

            forceSplatter.begin();



                forceSplatter.setUniformTexture("posData", posPingPong.dst->getTexture(), 0);  // passing the position information
                forceSplatter.setUniform1f("u_time", ofGetElapsedTimef() );

                ofSetColor(255,255,255);
                vboSplatter.draw(GL_QUADS,0,numParticles*4);

            forceSplatter.end();


        fboForces.end();

        ofDisableBlendMode();
    }

}

//--------------------------------------------------------------
void ofApp::draw(){


//    glDepthMask(GL_FALSE);

    if(bForceSplatter)
    {
        ofPushStyle();
            ofPushMatrix();
    //        camera.begin();
                ofTranslate(5*width,1*height );
                fboForces.draw(0,0,width,height );
    //        camera.end();
            ofPopMatrix();
        ofPopStyle();
    }

	ofEnableBlendMode(OF_BLENDMODE_ADD);
	ofEnablePointSprites();
//
//	// bind the shader and camera
//	// everything inside this function
//	// will be effected by the shader/camera

    ofPushMatrix();

        render.begin();
        //	camera.begin();
        //
            render.setUniformTexture("posTex", posPingPong.dst->getTexture(), 0);
            render.setUniform2f("screen", (float)width, (float)height);

            ofTranslate(4*width,2*height );

            vbo.draw(GL_POINTS, 0, (int)points.size());

        //	camera.end();
        render.end();



        ofDisablePointSprites();
        ofDisableBlendMode();

    ofPopMatrix();

    ofPushMatrix();

        forceSplatterRender.begin();

                forceSplatterRender.setUniformTexture("posData", posPingPong.dst->getTexture(), 0);  // passing the position information
                forceSplatterRender.setUniform1f("u_time", ofGetElapsedTimef() );
                forceSplatterRender.setUniform1f("resolution", textureRes);

    //            camera.begin();
                    ofTranslate(8*width,2*height );
                    vboSplatter.draw(GL_QUADS,0,numParticles*4);
    //            camera.end();

        forceSplatterRender.end();

    ofPopMatrix();

}

void ofApp::zerostate(float vm)
{
    float xm = 1.0;

    // 1. Making arrays of float pixels with position information
    float * pos = new float[numParticles*3];
    for (int x = 0; x < textureRes; x++){
        for (int y = 0; y < textureRes; y++){
            int i = textureRes * y + x;

            pos[i*3 + 0] = ofMap(x,0,textureRes-1,-xm,xm);
            pos[i*3 + 1] = ofMap(y,0,textureRes-1,-xm,xm);
            pos[i*3 + 2] = 0.0;
        }
    }
    // Load this information in to the FBO�s texture
    posPingPong.allocate(textureRes, textureRes,GL_RGB32F);
    posPingPong.src->getTexture().loadData(pos, textureRes, textureRes, GL_RGB);
    posPingPong.dst->getTexture().loadData(pos, textureRes, textureRes, GL_RGB);

    // 2. Making arrays of float pixels with velocity information and the load it to a texture
    float * vel = new float[numParticles*3];
    for (int i = 0; i < numParticles; i++){
        vel[i*3 + 0] = ofRandom(-vm,vm);
        vel[i*3 + 1] = ofRandom(-vm,vm);
        vel[i*3 + 2] = 0.0;
    }
    // Load this information in to the FBO�s texture
    velPingPong.allocate(textureRes, textureRes,GL_RGB32F);
    velPingPong.src->getTexture().loadData(vel, textureRes, textureRes, GL_RGB);
    velPingPong.dst->getTexture().loadData(vel, textureRes, textureRes, GL_RGB);

    delete [] pos;
    delete [] vel;

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

    if(key=='r')
    {
        updatePos.load("",shadersFolder+"/posUpdate.frag");// shader for updating the texture that store the particles position on RG channels
        updateVel.load("",shadersFolder+"/velUpdate.frag");// shader for updating the texture that store the particles velocity on RG channels
        render.load(shadersFolder+"/render");
        forceSplatter.load(shadersFolder+"/splatter");
        forceSplatterRender.load(shadersFolder+"/splatterRender");

    }

    if(key==OF_KEY_UP)
    {
        dt+=0.001;
        cout << dt<<  endl;
    }


    if(key==OF_KEY_DOWN)
    {
       dt-=0.001;
       cout << dt<<  endl;
    }

    if(key=='v')
    {
        zerostate(1.0);
    }


    if(key=='s')
    {
        zerostate(0);
        camera.reset();
    }

    if(key=='f')
    {
        bForceSplatter  = !bForceSplatter;
        cout << "Force Splatter: " << bForceSplatter << endl;
    }


    if(key=='p')
    {
        ofTexture tex = posPingPong.src->getTexture();
        ofFloatPixels pix;
        tex.readToPixels(pix);
        cout << "Size: " << pix.getWidth() <<  "," <<  pix.getHeight() << endl;

        float *pos = pix.getData();

        cout << "Positions: "  << endl;

        for (int i=0;i < numParticles; i++)
            cout << i <<":" << pos[3*i] <<"," << pos[3*i+1] << endl;

        tex = fboForces.getTexture();
        tex.readToPixels(pix);
        cout << "Size: " << pix.getWidth() <<  "," <<  pix.getHeight() << endl;

        pos = pix.getData();

        cout << "Forces: "  << endl;

        for (int i=0;i < numParticles; i++)
            cout << i <<":" << pos[3*i] <<"," << pos[3*i+1] << endl;

    }




}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}
