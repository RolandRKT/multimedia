#version 150 core

in vec3 lightDir;
in vec3 eyeVec;
in vec3 out_normal;

out vec4 frag_color;

void main(void)
{
    // Couleur fixe rouge vif pour tester
    frag_color = vec4(1.0, 0.0, 0.0, 1.0);
}
