#version 100

precision mediump float;

uniform vec3 LightDir;
uniform vec3 Diffuse;
uniform vec3 Ambient;

varying vec3 FragNormal;
varying vec4 FragColor;

void main() {
    float NdotL = max(dot(FragNormal, LightDir), 0.0);
    vec3 Lighting = Ambient + Diffuse * NdotL;
    gl_FragColor = FragColor * vec4(Lighting, 1);
}
