Il a été "nettoyé" par rapport au basic.vert précédent. On a retiré les fonctions de bruit (random) qui déformaient les sommets. Son rôle est standard : il projette les points 3D à l'écran (gl_Position) et calcule les vecteurs nécessaires à l'éclairage (normale, direction lumière, direction vue) qu'il envoie aux fragment shaders.

Les Shaders Phong (phong*.frag.glsl)
Ils implémentent le modèle d'illumination de Phong par pixel (plus précis que Gouraud).

    Ambiante : Une couleur de base constante pour que les zones d'ombre ne soient pas noires pures.

    Diffuse : La couleur principale de l'objet, dont l'intensité dépend de l'orientation face à la lumière (dot(L, N)). C'est ici qu'on change la couleur (Rouge, Vert, Gris).

    Spéculaire : Une tache blanche brillante (dot(R, E)) qui simule le reflet de la source lumineuse sur une surface lisse (plastique/métal).

Le Shader Toon (toon.frag.glsl)
C'est un shader non-photoréaliste. Au lieu d'utiliser l'intensité lumineuse calculée directement pour colorer le pixel, on l'utilise comme index pour choisir parmi 3 ou 4 couleurs fixes (paliers). Cela supprime les dégradés progressifs et crée cet effet "aplats de couleurs" typique des dessins animés.