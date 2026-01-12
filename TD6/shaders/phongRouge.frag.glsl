#version 150 core

in vec3 lightDir;
in vec3 eyeVec;
in vec3 out_normal;

out vec4 frag_color;

void main(void)
{
    vec3 L = normalize(lightDir);
    vec3 N = normalize(out_normal);
    float intensity = max(dot(L, N), 0.0);
    
    vec3 E = normalize(eyeVec);
    vec3 R = reflect(-L, N);
    float specular = pow(max(dot(R, E), 0.0), 32.0);
    
    vec4 final_color = vec4(0.2, 0.1, 0.1, 1.0);
    final_color = vec4(0.9, 0.3, 0.3, 1.0) * specular;
    final_color += vec4(0.8, 0.0, 0.0, 1.0) * intensity;
    
    frag_color = final_color;
}
