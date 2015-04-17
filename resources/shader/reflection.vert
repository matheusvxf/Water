#version 330 compatibility

uniform mat4 CameraMatrix;
out vec3 pos_eye;
out vec3 n_eye;

void main () {
  pos_eye = vec3 (gl_ModelViewMatrix * gl_Vertex);
  n_eye = vec3 (gl_ModelViewMatrix * vec4 (gl_Normal, 0.0));
  gl_Position = ftransform();
}