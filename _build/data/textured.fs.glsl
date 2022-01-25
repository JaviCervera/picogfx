uniform sampler2D texture;

varying vec2 frag_texcoords;

void main() {
  gl_FragColor = texture2D(texture, frag_texcoords);
}
