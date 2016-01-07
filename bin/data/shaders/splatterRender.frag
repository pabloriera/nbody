

uniform sampler2DRect posData;

varying vec4 v_position;
varying vec3 v_normal;

uniform float u_time;
uniform float resolution;

void main (void) {

    vec2 pos = texture2DRect( posData, v_normal.xy).xy*0.5+0.5;
    vec2 pos2 = texture2DRect( posData, v_position.xy*resolution ).xy*0.5+0.5;
    
    float d = distance(pos2.xy,pos.xy);
    
    vec2 r = abs(pos2.xy-pos.xy);
    // vec2 r = (pos2.xy-pos.xy);

    // if (pos2.xy == vec2(1.0,0.0))
        // r = 0;

    // if (distance(pos.xy, vec2(0.0,0.0) ) <0.01 )
    //     r = 0.5;

    vec3 color = vec3(r,0.0);    
    // color = vec3(d);
    gl_FragColor = vec4(color,1.0);
    
}
