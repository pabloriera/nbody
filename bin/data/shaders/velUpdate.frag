#version 120
#extension GL_ARB_texture_rectangle : enable


uniform sampler2DRect backbuffer;   // recive the previus velocity texture
uniform sampler2DRect forceData;   // recive the previus velocity texture
uniform sampler2DRect posData;      // recive the position textureuniform sampler2DRecvect forceData;      // recive the position text2 forceData

uniform vec2  screen;

uniform float dt;
    
void main(void){

    vec2 st = gl_TexCoord[0].st;    // gets the position of the fragment that it´s dealing with...
    
    vec2 pos = texture2DRect( posData, st).xy;      // ... for gettinh the position data 
    vec2 vel = texture2DRect( backbuffer, st ).xy;  // and the velocity
    vec2 force = texture2DRect( forceData, st ).xy;  // and the velocity

    //  Velocity verlet

    //  pos = pos + vel * dt + 0.5*acc_old*dt2;

    //  Compute New acc
    //  acc = 
    //  vel = vel + 0.5*(acc + acc_old )*dt;

    //  Save old acc
    //  acc_old = acc;
    
    // Simple Test
    // vel.x = pos.y*3.0-pos.x*0.08;
    // vel.y = -pos.x*3.0-pos.y*0.08;
    // vel.x = 0.8;
    // vel.y = 1.0;

    //aprox, works for slow varying forces
    vel = vel + force*dt;

    vec2 nextPos = pos; 
    nextPos += vel * dt;
        
    // Calculates what´s going to be the next position without updating it.
    // Just to see if it collide with the borders of the FBO texture
    // If it´s going to collide change the velocity course
    //
    if ( nextPos.x < -1.0)
        vel.x = 1.0 * abs(vel.x);
        
    if ( nextPos.x > 1.0)
        vel.x = -1.0 * abs(vel.x);
        
    if (nextPos.y < -1.0)
        vel.y = 1.0 * abs(vel.y);
    
    if ( nextPos.y > 1.0)
        vel.y = -1.0 * abs(vel.y);

        
    gl_FragColor = vec4(vel.x,vel.y,0.0,0.0);   // Then save the vel data into the velocity FBO
}