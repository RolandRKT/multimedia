#version 150 core

out vec4 frag_color;

in vec3 lightDir;
in vec3 eyeVec;
in vec3 out_normal;

void main(void)
{
    vec3 L = normalize(lightDir);
    vec3 N = normalize(out_normal);
    vec3 E = normalize(eyeVec);
    
    float intensity = max(dot(L,N), 0.0);
    vec3 R = reflect(-L, N);
    float specular = pow(max(dot(R, E), 0.0), 16.0);
    
    // Ambiante : Rouge sombre
    vec4 final_color = vec4(0.2, 0.0, 0.0, 1.0);
    
    // Diffuse : Rouge vif
    final_color += vec4(0.8, 0.1, 0.1, 1.0) * intensity;
    
    // Spéculaire : Blanc
    final_color += vec4(0.8, 0.8, 0.8, 1.0) * specular;
    
    frag_color = final_color;
}
