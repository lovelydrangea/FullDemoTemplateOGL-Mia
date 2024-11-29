#version 330 core
in float opacity; // Recibir opacidad desde el vertex shader

uniform vec3 lightPosition;
uniform vec3 lightColor;

void main() {
    vec3 lightEffect = lightColor; // Ejemplo básico, sin cálculo de iluminación
    gl_FragColor = vec4(lightEffect, opacity); // Aplicar opacidad a las gotas de lluvia
}
