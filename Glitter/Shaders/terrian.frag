#version 410 core

in float Height;
in vec3 Position;
out vec4 FragColor;

// Smoothstep function
float smoothstep(float edge0, float edge1, float x) {
    float t = clamp((x - edge0) / (edge1 - edge0), 0.0, 1.0);
    return t * t * (3.0 - 2.0 * t);
}

void main()
{
    vec3 color;

    float oceanEdge = 0.1;
    float beachEdge = 0.5;
    float lowlandEdge = 2.0;
    float hillEdge = 5.0;
    float mountainEdge = 15.0;
    float snowEdge = 40.0;

    if (Height < oceanEdge) {             // Deep Ocean (dark blue)
        color = vec3(0.0, 0.0, 0.2);
    } else if (Height < beachEdge) {      // Ocean (light blue)
        color = mix(vec3(0.0, 0.5, 1.0), vec3(1.0, 0.8, 0.4), smoothstep(oceanEdge, beachEdge, Height));
    } else if (Height < lowlandEdge) {    // Beach (sandy yellow)
        color = mix(vec3(1.0, 0.8, 0.4), vec3(0.0, 0.7, 0.0), smoothstep(beachEdge, lowlandEdge, Height));
    } else if (Height < hillEdge) {       // Lowlands (green)
        color = mix(vec3(0.0, 0.7, 0.0), vec3(0.5, 0.7, 0.3), smoothstep(lowlandEdge, hillEdge, Height));
    } else if (Height < mountainEdge) {   // Hills (light green)
        color = mix(vec3(0.5, 0.7, 0.3), vec3(0.5, 0.3, 0.1), smoothstep(hillEdge, mountainEdge, Height));
    } else if (Height < snowEdge) {       // Mountains (brown)
        color = mix(vec3(0.5, 0.3, 0.1), vec3(1.0, 1.0, 1.0), smoothstep(mountainEdge, snowEdge, Height));
    } else {                              // Snow-capped Peaks (white)
        color = vec3(1.0, 1.0, 1.0);
    }

    FragColor = vec4(color, 1.0);
}
