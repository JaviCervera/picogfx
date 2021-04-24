uniform mat4 Projection;
uniform mat4 ModelView;

attribute vec3 VertexPosition;
attribute vec3 VertexNormal;
attribute vec4 VertexColor;

varying vec3 FragNormal;
varying vec4 FragColor;

void main() {
    gl_Position = Projection * ModelView * vec4(VertexPosition, 1);
    FragNormal = vec3(ModelView * vec4(VertexNormal, 0));
    FragColor = VertexColor;
}
