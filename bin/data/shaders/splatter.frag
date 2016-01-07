uniform sampler2DRect posData;

varying vec4 v_position;
varying vec3 v_normal;

uniform float u_time;
uniform float resolution;


void main (void) {


    vec2 pos = texture2DRect( posData, v_normal.xy).xy*0.5+0.5;
    vec2 pos2 = texture2DRect( posData, v_position.xy*resolution ).xy*0.5+0.5;
    
    float d = distance(pos2.xy,pos.xy);
    float F = 0.0;
    vec2 r = vec2(0.0);
    

    if (d>0.01)
    {
        r = (pos2.xy-pos.xy)/d;
        F = 0.01/pow(d,2.0);
    }
         
    vec3 force = vec3(F*r,0.0);
    // fvec3 force = vec3(F*r,0.0);

    //Change this for adding a global force
    // force = force + vec3(1.0,0.5,0.0);

    gl_FragColor = vec4(force,1.0);

    // vec3 color = hsv2rrgb(vec3(h,0.0,d));
    
    // if (pos.x < 0.5)
    // color = color + vec3(10.0,0.0,0.0);



    // float d = distance(pos2.xy,pos.xy);
    // float F = 0.1/pow(distance(pos2.xy,pos.xy)+0.2,2.0);

    

    // vec3 color = vec3(d);
    // // color = color + vec3(10.0,0.0,0.0);
    // // vec3 color = vec3(d*r,0.0);

    // // vec3 color = vec3(10.0,10.0,0.0);    
    // gl_FragColor = vec4( color,1.0);


    // gl_FragColor = vec4(0.1,1.0,0.2,1.0);   

    
    // vec2 pos = texture2DRect( posData, v_normal.xy).xy*0.5+0.5;
    // float d = 0.1/pow(distance(v_position.xy,pos.xy)+0.1,2);
    // vec3 color = hsv2rgb(vec3(h,1.0,d));

    // gl_FragColor = vec4( color,1.0);    

    
}
