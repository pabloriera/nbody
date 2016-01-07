
uniform sampler2DRect prevPosData;  // recive the previus position texture
uniform sampler2DRect velData;      // recive the velocity texture
uniform sampler2DRect forceData;      // recive the velocity texture
uniform vec2 u_mouse;
uniform float dt;


void main(void){
    vec2 st = gl_TexCoord[0].st;    // gets the position of the pixel that it´s dealing with...
    
    vec2 pos = texture2DRect( prevPosData, st ).xy; // ... in order to look at a particulary place on it
    vec2 vel = texture2DRect( velData, st ).xy;     // Fetch both the pos and vel.
    vec2 force = texture2DRect( forceData, st ).xy;  // and the velocity

    pos = pos + vel * dt + 0.5*force*dt*dt;

    if(st.x<1.1)
    	if(st.y<1.1)
    		pos = (u_mouse-vec2(0.1,0.1))*4.0*vec2(1.0,-1.0);
	
	

    gl_FragColor.rgba = vec4(pos.x,pos.y,0.0,0.0);  // And finaly it store it on the position FBO
}