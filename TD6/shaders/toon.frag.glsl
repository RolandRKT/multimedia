#version 150 core

out vec4 frag_color;

in vec3 lightDir;
in vec3 eyeVec;
in vec3 out_normal;

vec4 toonify(in float intensity) {
    vec4 color;
    if (intensity > 0.98)
    color = vec4(0.8,0.8,0.8,1.0);
    else if (intensity > 0.5)
    color = vec4(0.4,0.4,0.8,1.0);
    else if (intensity > 0.25)
    color = vec4(0.2,0.2,0.4,1.0);
    else
    color = vec4(0.1,0.1,0.1,1.0);
    return(color);
}

void main(void)
{
    vec3 L = normalize(lightDir);
    vec3 N = normalize(out_normal);
    
    float intensity = max(dot(L,N), 0.0);
    
    frag_color = toonify(intensity);
}
