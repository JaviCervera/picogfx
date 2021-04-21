#version 100

precision mediump float;

struct SLight {
    vec3 Direction;
    vec3 Color;
};

uniform SLight DirLight;
uniform vec3 Ambient;

varying vec3 FragNormal;
varying vec4 FragColor;

void main() {
    float NdotL = max(dot(FragNormal, DirLight.Direction), 0.0);
    vec3 Lighting = Ambient + DirLight.Color * NdotL;
    gl_FragColor = FragColor * vec4(Lighting, 1);
}
