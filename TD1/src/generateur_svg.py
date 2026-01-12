#!/usr/bin/env python  # Shebang - indique l'interpréteur à utiliser

# Importe la bibliothèque svgwrite pour créer des fichiers SVG
import svgwrite

# Crée un nouveau dessin SVG de taille 800x600 pixels
dessin = svgwrite.Drawing('../assets/svg/exercice_1.svg', size=(800,600))

# Définit les coordonnées des trois points du triangle
# Format: [(x1,y1), (x2,y2), (x3,y3)]
triangle=[(0,400),(400,400),(200,100)]

# Ajoute un polygone (triangle) au dessin avec les propriétés suivantes:
# - remplissage bleu (#0000FF)
# - contour noir (#000000)
# - transparence de 0.7 (70% opaque)
dessin.add(dessin.polygon(triangle, fill="#FF0000",\
                          stroke="#000000", opacity=0.7 ))

# Sauvegarde le dessin dans le fichier SVG
dessin.save()