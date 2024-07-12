#version 460 core

in vec2 v_TexCoord;

uniform sampler2D u_Texture;

out vec4 o_Color;

void main() {
    vec4 col = texture(u_Texture, v_TexCoord);
    if (col.a < 0.5) {
        discard;
    }
    o_Color = col;
}
