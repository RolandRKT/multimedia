#version 150 core

out vec4 frag_color;

in vec3 lightDir;
in vec3 eyeVec;
in vec3 out_normal;

void main(void)
{
    // Normalisation des vecteurs interpolés
    vec3 L = normalize(lightDir);
    vec3 N = normalize(out_normal);
    vec3 E = normalize(eyeVec);
    
    // --- Composante Diffuse (Lambert) ---
    // Intensité proportionnelle à l'angle entre la lumière et la normale
    float intensity = max(dot(L,N), 0.0);
    
    // --- Composante Spéculaire (Reflet brillant) ---
    // Calcul du vecteur de réflexion R
    vec3 R = reflect(-L, N);
    // Puissance spéculaire (shininess) réglée à 16.0 pour un reflet moyennement net
    float specular = pow(max(dot(R, E), 0.0), 16.0);
    
    // --- Couleur Finale ---
    vec4 final_color = vec4(0.2, 0.2, 0.2, 1.0); // Composante Ambiante (Gris foncé)
    
    // Ajout du Diffus (Gris clair qui réagit à la lumière)
    final_color += vec4(0.6, 0.6, 0.6, 1.0) * intensity;
    
    // Ajout du Spéculaire (Reflet blanc pur)
    final_color += vec4(0.8, 0.8, 0.8, 1.0) * specular;
    
    frag_color = final_color;
}
