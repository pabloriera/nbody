#pragma once

#include "ofMain.h"

struct pingPongBuffer {
public:
    void allocate( int _width, int _height, int _internalformat = GL_RGBA, float _dissipation = 1.0f){
        // Allocate
        for(int i = 0; i < 2; i++){
            FBOs[i].allocate(_width,_height, _internalformat );
            FBOs[i].getTexture().setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
        }

        // Clean
        clear();

        // Set everything to 0
        flag = 0;
        swap();
        flag = 0;
    }

    void swap(){
        src = &(FBOs[(flag)%2]);
        dst = &(FBOs[++(flag)%2]);
    }

    void clear(){
        for(int i = 0; i < 2; i++){
            FBOs[i].begin();
            ofClear(0,255);
            FBOs[i].end();
        }
    }

    ofFbo& operator[]( int n ){ return FBOs[n];}

    ofFbo   *src;       // Source       ->  Ping
    ofFbo   *dst;       // Destination  ->  Pong
private:
    ofFbo   FBOs[2];    // Real addresses of ping/pong FBO«s
    int     flag;       // Integer for making a quick swap
};


class ofApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();
    void zerostate(float vm);

    void keyPressed  (int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

    string shadersFolder;

    ofShader    updatePos;
    ofShader    updateVel;
    ofShader    forceSplatter;
    ofShader    forceSplatterRender;

    pingPongBuffer posPingPong;
    pingPongBuffer velPingPong;

    float   dt;
    float   particleSize;

    int     width, height;
    int     imgWidth, imgHeight;
    int     numParticles;
    int     textureRes;

    ofEasyCam camera;
    ofShader render;

    bool bForceSplatter;

    vector <ofVec3f> points;
    vector <ofVec3f> sizes;

    ofMesh mesh;

    ofVbo vboSplatter;
    ofFbo fboForces;

    ofVbo vbo;

    float camDist;



};
