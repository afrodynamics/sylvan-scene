// Fragment Shader
/*
tex/right.ppm has tex ID: 1      +X
tex/left.ppm has tex ID: 2       
tex/front.ppm has tex ID: 3
tex/back.ppm has tex ID: 4
tex/top.ppm has tex ID: 5
tex/base.ppm has tex ID: 6
 */
uniform sampler2D front; // +Z
uniform sampler2D back;  // -Z
uniform sampler2D top;   // +Y
uniform sampler2D base;  // -Y
uniform sampler2D left;  // -X
uniform sampler2D right; // +X

void main()
{
	vec3 refl = gl_TexCoord[0].stp; // Hacky but hopefully it'll work
	vec3 reflAbs = vec3( abs(refl.x), abs(refl.y), abs(refl.z) );
	float rMax = 0.0;

	if ( reflAbs.x >= reflAbs.y && reflAbs.x >= reflAbs.z ) {
		// X is the biggest
		gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0); // Testing to see if reflect is working
		rMax = reflAbs.x;
	}
	else if ( reflAbs.y >= reflAbs.x && reflAbs.y >= reflAbs.z ) {
		// Y is the biggest
		gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0); // Testing to see if reflect is working
		rMax = reflAbs.y;
	}
	else if ( reflAbs.z >= reflAbs.x && reflAbs.z >= reflAbs.y ) {
		// Z is the biggest
		gl_FragColor = vec4(0.0, 0.0, 1.0, 1.0); // Testing to see if reflect is working
		rMax = reflAbs.z;
	}

	gl_FragColor = texture2D( front, reflAbs.xy );

	//gl_FragColor = vec4( gl_TexCoord[0].stp, 1.0 ); // passed in from vtx shader
}