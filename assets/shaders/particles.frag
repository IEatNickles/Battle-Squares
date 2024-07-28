#version 460 core

in VS_OUT {
    vec4 color;
    float life;
} fs_in;

out vec4 o_Color;

void main() {
    if (fs_in.life <= 0.0) {
        discard;
    }
    o_Color = fs_in.color;
}
