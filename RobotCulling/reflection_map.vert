#version 120

//uniform vec3 CameraPosition; // we don't need this

void main()
{
    vec3 Normal = gl_NormalMatrix * gl_Normal; // Make sure normal is scaled properly
    vec4 Position = gl_ModelViewProjectionMatrix * gl_Vertex; // OR: gl_ModelViewMatrix * gl_Vertex // = cam space
    vec3 Reflection = reflect(normalize(Position.xyz), normalize(Normal));
    //refract(normalize(Position.xyz), normalize(Normal), 1.333); // reflect(normalize(Position.xyz), normalize(Normal));
    Reflection = normalize(Reflection);
    gl_TexCoord[0].stp = vec3(Reflection.x, -Reflection.yz);
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex; // Transform position into clip space
}