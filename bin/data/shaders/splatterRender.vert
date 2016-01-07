varying vec4 v_position;
varying vec3 v_normal;

uniform float resolution;
uniform vec2 u_mouse;

const float S = 200.0;

void main() {

	// vec2 verPos = gl_MultiTexCoord0.xy;
	vec4 verPos = gl_Vertex;
    
	vec4 pixPos = verPos;

    float Sf = S/resolution;


    pixPos.x += gl_Normal.x-1.0;
    pixPos.y += gl_Normal.y-1.0;
    
    pixPos.x *= Sf;
	pixPos.y *= Sf;

    // pixPos.x += S*4;
    // pixPos.y += S;

    gl_PointSize  = 1.0;

    gl_FrontColor = gl_Color;

    gl_Position   = gl_ModelViewProjectionMatrix * pixPos;

    v_position    = verPos;
    v_normal      = gl_Normal+1.0;


}