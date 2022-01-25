uniform mat4 projection;
uniform mat4 model_view;

attribute vec3 vertex_position;
attribute vec4 vertex_color;

varying vec4 frag_color;

void main() {
  gl_Position = projection * model_view * vec4(vertex_position, 1);
  frag_color = vertex_color;
}
