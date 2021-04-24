precision mediump float;

uniform sampler2D Texture;

varying vec2 FragTexCoords;

void main() {
    gl_FragColor = texture2D(Texture, FragTexCoords);
}
