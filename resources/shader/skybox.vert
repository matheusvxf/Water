#version 120

uniform vec3 CameraPosition;

void main()
{
    gl_TexCoord[0].stp = vec3(gl_Vertex.x, gl_Vertex.yz);
    gl_Position = gl_ModelViewProjectionMatrix * vec4(gl_Vertex.xyz + CameraPosition, 1.0);
	gl_Position = ftransform();
}