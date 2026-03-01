#version 330 core

uniform vec2 resolution;
uniform float time;
out vec4 fragColor;

float circle(vec2 uv, vec2 center, float radius) {
    float d = length(uv - center);
    return step(d, radius);
}
    
void main() {
    vec2 fragCoord = gl_FragCoord.xy;
    vec2 uv = (fragCoord.xy / resolution) * 2.0 - 1.0;
    uv.x *= resolution.x / resolution.y;

    float c = circle(uv, vec2(0.0), 0.5);
    fragColor = vec4(vec3(c), 1.0);
}