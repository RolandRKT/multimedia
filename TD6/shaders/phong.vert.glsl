#version 150 core

uniform mat4 m; // Matrice Model
uniform mat4 v; // Matrice View
uniform mat4 p; // Matrice Projection

in vec3 in_pos;    // Position du sommet (depuis le VBO)
in vec3 in_normal; // Normale du sommet (depuis le VBO)

out vec3 lightDir;   // Vecteur direction vers la lumière (interpolé)
out vec3 eyeVec;     // Vecteur direction vers l'œil/caméra (interpolé)
out vec3 out_normal; // Normale transformée (interpolée)

void main(void)
{
    // Calcul de la position du sommet dans l'espace Vue (Camera Space)
    vec4 vVertex = v * m * vec4(in_pos, 1.0);
    
    // Vecteur Vue : de la surface vers la caméra (l'origine (0,0,0) en Camera Space)
    eyeVec = -vVertex.xyz;
    
    // Position de la lumière (fixée arbitrairement un peu devant et en haut)
    vec4 LightSource_position = vec4(0.0, 10.0, 10.0, 0.0);
    
    // Vecteur Lumière : de la surface vers la lumière
    lightDir = vec3(LightSource_position.xyz - vVertex.xyz);
    
    // Transformation de la normale par la matrice Model-View
    // (Note: pour être rigoureux, il faudrait la matrice Normale, inverse transposée de MV)
    out_normal = vec3(v * m * vec4(in_normal, 0.0));
    
    // Position finale du sommet projeté à l'écran
    gl_Position = p * v * m * vec4(in_pos, 1.0);
}
