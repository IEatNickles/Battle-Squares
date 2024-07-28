#version 460 core

layout(location=0) in vec2 a_Position;
layout(location=1) in vec2 a_TexCoord;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Proj;

out vec2 v_TexCoord;

void main() {
    v_TexCoord = a_TexCoord;
    gl_Position = u_Proj * u_View * u_Model * vec4(a_Position, 0.0, 1.0);
}
