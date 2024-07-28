#version 460 core

struct Particle {
    mat4 model_matrix;
    vec4 color;
    float life;
    float a, b, c;
};

layout(location=0) in vec2 a_Position;

layout(binding=1) buffer a_Particles {
    Particle particles[];
};

uniform mat4 u_View;
uniform mat4 u_Proj;

out VS_OUT {
    vec4 color;
    float life;
} vs_out;

void main() {
    Particle p = particles[gl_InstanceID];
    gl_Position = u_Proj * u_View * p.model_matrix * vec4(a_Position, 0.0, 1.0);
    vs_out.color = p.color;
    vs_out.life = p.life;
}
