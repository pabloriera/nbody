varying vec4 v_position;
varying vec3 v_normal;

void main() {

	vec4 pixPos = gl_Vertex;

    float s = 80.0;
 //    pixPos.x *= s;
	// pixPos.y *= s;
    
    // pixPos.x -= r1.0;

    gl_PointSize  = 1.0;

    gl_Position   = pixPos;

    v_position    = gl_Vertex;
    v_normal      = gl_Normal+1.0;


}
