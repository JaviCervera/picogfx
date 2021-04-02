uniform mat4 Projection;
uniform mat4 ModelView;

attribute vec3 VertexPosition;
attribute vec4 VertexColor;

varying vec4 FragColor;

void main() {
    gl_Position = Projection * ModelView * vec4(VertexPosition, 1);
    FragColor = VertexColor;
}
