#version 150 core

out vec4 frag_color;

in vec3 lightDir;
in vec3 eyeVec;
in vec3 out_normal;

// Fonction qui seuille l'intensité lumineuse
vec4 toonify(in float intensity) {
    vec4 color;
    
    // Si très éclairé -> Bleu très clair
    if (intensity > 0.95)
        color = vec4(0.6, 0.6, 1.0, 1.0);
    // Si éclairé -> Bleu moyen
    else if (intensity > 0.5)
        color = vec4(0.3, 0.3, 0.8, 1.0);
    // Si peu éclairé -> Bleu foncé
    else if (intensity > 0.25)
        color = vec4(0.1, 0.1, 0.5, 1.0);
    // Si dans l'ombre -> Bleu nuit/noir
    else
        color = vec4(0.05, 0.05, 0.2, 1.0);
        
    return color;
}

void main(void)
{
    vec3 L = normalize(lightDir);
    vec3 N = normalize(out_normal);
    
    // On ne garde que l'intensité diffuse
    float intensity = max(dot(L,N), 0.0);
    
    // On applique la fonction de seuillage
    frag_color = toonify(intensity);
}
