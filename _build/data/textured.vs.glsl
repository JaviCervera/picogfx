uniform mat4 Projection;
uniform mat4 ModelView;

attribute vec3 VertexPosition;
attribute vec2 VertexTexCoords;

varying vec2 FragTexCoords;

void main() {
    gl_Position = Projection * ModelView * vec4(VertexPosition, 1);
    FragTexCoords = VertexTexCoords;
}
