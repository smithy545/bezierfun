#version 430 core

layout (location = 0) in vec2 inPos;
layout (location = 1) in vec4 inColor;

uniform mat4 MVP;

out vec4 color;

void main() {
    gl_Position = MVP * vec4(inPos, 0, 1);
    color = inColor;
}
