// Test vertex shader to make sure a shader is working for a given object

void main() {
	gl_Position = ftransform();
}