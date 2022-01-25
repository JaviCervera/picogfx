uniform vec3 light_dir;
uniform vec3 diffuse;
uniform vec3 ambient;
uniform vec3 emissive;
uniform vec3 specular;
uniform int spec_power;
uniform vec3 half_vector;

varying vec3 frag_normal;
varying vec4 frag_color;

void main() {
  float ndotl = max(dot(frag_normal, light_dir), 0.0);
  float ndoth = max(dot(frag_normal, half_vector), 0.0);
  vec3 amb_diffuse = ambient + diffuse * ndotl;
  gl_FragColor = frag_color * vec4(amb_diffuse, 1) + vec4(emissive, 0) + vec4(specular * pow(ndoth, float(spec_power)), 0);
}
