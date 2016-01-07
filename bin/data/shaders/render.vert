// attribute vec4 gl_Color;

uniform sampler2DRect posTex;
uniform vec2 screen;



void main() {

	// vec2 verrPos = gl_MultiTexCoord0.xy;
	vec2 verPos = gl_Vertex.xy;

	vec4 pixPos = texture2DRect( posTex, verPos );

    pixPos.x -=0.5;

    pixPos.x *= screen.x;
    pixPos.y *= screen.x;

	float size    = gl_Normal.x;

    gl_PointSize  = size*1.0;
    gl_FrontColor = gl_Color;

    gl_Position   = gl_ModelViewProjectionMatrix * pixPos;

}