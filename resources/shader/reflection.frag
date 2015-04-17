#version 330 compatibility

uniform mat4 CameraMatrix;
uniform samplerCube cube_texture;
in vec3 pos_eye;
in vec3 n_eye;

void main () {
  /* reflect ray around normal from eye to surface */
  vec3 incident_eye = normalize (pos_eye);
  vec3 normal = normalize (n_eye);

  vec3 reflected = reflect (incident_eye, normal);
  // convert from eye to world space
  reflected = vec3 (inverse (CameraMatrix) * vec4 (reflected, 0.0));

  gl_FragColor = texture (cube_texture, reflected);
}