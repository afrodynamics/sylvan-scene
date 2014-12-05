// Fragment Shader
// uniform samplerCube CubeMap;

void main()
{
	//gl_FragColor = textureCube(CubeMap, gl_TexCoord[0].stp);
	//gl_FragColor = vec4(1.0,1.0,0.0,1.0);
	gl_FragColor = vec4( gl_TexCoord[0].stp, 1.0 ); // passed in from vtx shader
}