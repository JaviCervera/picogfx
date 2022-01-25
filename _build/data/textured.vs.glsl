uniform mat4 projection;
uniform mat4 model_view;

attribute vec3 vertex_position;
attribute vec2 vertex_texcoords;

varying vec2 frag_texcoords;

void main() {
  gl_Position = projection * model_view * vec4(vertex_position, 1);
  frag_texcoords = vertex_texcoords;
}
