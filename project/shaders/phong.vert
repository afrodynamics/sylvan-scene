/**
 * Simple phong shader for one point light.
 * https://www.opengl.org/sdk/docs/tutorials/ClockworkCoders/lighting.php
 * Modified a little.
 */
varying vec3 N;
varying vec3 v;

void main(void)  
{     
   v = vec3(gl_ModelViewMatrix * gl_Vertex);       
   N = normalize(gl_NormalMatrix * gl_Normal);
   gl_Position = ftransform();  
}