#version 120

uniform vec3 CameraPosition;

void main()
{
    vec3 Normal = gl_NormalMatrix * gl_Normal; // Make sure normal is scaled properly
    vec4 Position = gl_ModelViewMatrix * gl_Vertex;
    vec3 Reflection = reflect(Position.xyz - CameraPosition, normalize(Normal));
    gl_TexCoord[0].stp = vec3(Reflection.x, -Reflection.yz);
    gl_Position = gl_ProjectionMatrix * Position; // Transform position into clip space
}