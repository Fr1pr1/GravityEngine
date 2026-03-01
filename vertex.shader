#version 330 core

layout(location = 0) in vec2 positions;  // needs layout, matches attrib pointer 0

void main(){  // was "int main"
    gl_Position = vec4(positions.xy, 0.0, 1.0);
}