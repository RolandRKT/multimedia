Le problème venait de l'ordre d'application des matrices de transformation.

​
Le Problème : Le Centrage Échouait

Les objets "invisibles" (la navette et le Faucon Millenium) avaient leur centre géométrique très loin de l'origine $(0,0,0)$ dans leur fichier .off.

Dans l'ancienne version de votre code, l'ordre des multiplications matricielles appliquait le redimensionnement (scale) après le déplacement vers l'origine (translate). Mathématiquement, cela revient à :
Pfinale=Position×Translate(−centre)×Scale×PinitialePfinale=Position×Translate(−centre)×Scale×Pinitiale

Cela déformait le vecteur de déplacement. Au lieu de ramener l'objet à l'origine $(0,0,0)$, cela le décalait d'une valeur incorrecte, propulsant l'objet très loin hors de l'écran (hors du champ de la caméra).
La Solution : Inverser l'Ordre Scale/Translate

Nous avons modifié la fonction displayMesh pour corriger l'ordre des opérations. En OpenGL (avec la librairie GLM), pour centrer puis redimensionner un objet localement, il faut écrire les instructions dans l'ordre inverse de l'application logique :

    On applique d'abord glm::scale (ligne écrite en premier).

    On applique ensuite glm::translate (ligne écrite en second).

Mathématiquement, la matrice finale devient :
Mfinale=Mposition×Mscale×McentrageMfinale=Mposition×Mscale×Mcentrage

Cela garantit que :

    L'objet est d'abord déplacé pour que son centre soit à $(0,0,0)$.

    L'objet centré est ensuite redimensionné à la bonne taille.

    L'objet propre est enfin positionné à sa place finale dans la scène (les coins de l'écran).