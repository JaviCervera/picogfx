uniform vec3 LightDir;
uniform vec3 Diffuse;
uniform vec3 Ambient;
uniform vec3 Emissive;
uniform vec3 Specular;
uniform int SpecPower;
uniform vec3 HalfVector;

varying vec3 FragNormal;
varying vec4 FragColor;

void main() {
    float NdotL = max(dot(FragNormal, LightDir), 0.0);
    float NdotH = max(dot(FragNormal, HalfVector), 0.0);
    vec3 AmbDiffuse = Ambient + Diffuse * NdotL;
    gl_FragColor = FragColor * vec4(AmbDiffuse, 1) + vec4(Emissive, 0) + vec4(Specular * pow(NdotH, float(SpecPower)), 0);
}
