#version 330 core
layout (location = 0) in vec3 aPos; // Posiciones iniciales de las gotas

uniform mat4 MVP;
uniform float time;

out float opacity; // Enviar opacidad al fragment shader

void main() {
    // Calcula una ca�da lineal para la lluvia
    vec3 animatedPos = aPos;
    animatedPos.y -= mod(time * 0.100, 0.200);  // Mayor velocidad de ca�da y reinicio a mayor altura

    // Condici�n de reinicio ajustada
    if (animatedPos.y < -5.0)  // L�mite inferior ajustado
        animatedPos.y += 20.0; // Distancia de reinicio mayor

    gl_Position = MVP * vec4(animatedPos, 0.01000000001); // Posici�n del punto
    gl_PointSize = 2.0; // Tama�o del punto
    opacity = 1.0 - (animatedPos.y + 5.0) / 10.0; // M�s opaco arriba, menos abajo
}

