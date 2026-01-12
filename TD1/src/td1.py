import math
from time import sleep
import svgwrite
from fonctions import *

# ============ TRANSFORMATION CLASSIQUE ============ #

from pathlib import Path

# fix : bug de chemin
script_dir = Path(__file__).parent
chemin_fic = script_dir.parent / "assets" / "svg" / ""

# Puis utiliser
dessin = svgwrite.Drawing(str(chemin_fic / "exercice_carre_trans.svg"), size=(800, 600))

carre = [(-100, -100),(100,-100),(100,100),(-100,100)]
trans = (0, 0)

carre_trans = [translater(rotation(sommet, ANGLE), trans) for sommet in carre]

# dessin.add(dessin.polygon(carre_trans, fill='#FF0000', stroke="#000000", opacity=0.7))
# dessin.save()

# ============ UTILISATION DE MATRICE ============ #

# == == == MATRICE DE ROTATION == == == #

matRot = Matrotation(ANGLE)

carre_out=[translater(prodMatVect(matRot,sommet),trans) for sommet in carre]

# dessin.add(dessin.polygon(carre_out, fill='#FF0000', stroke="#000000", opacity=0.7))
# dessin.save()

# == == == MATRICE DE DILATATION == == == #

# == IMBRIQUER LES TRANSFORMATIONS == #

TD1questionDilat2 = svgwrite.Drawing(str(chemin_fic / "TD1questionDilat2.svg"), size=(800, 600))

for i in range(6):
    
    trans = (400, 300)

    coef = .5 * i
    matDil = Matdilatation(coef)

    carre_out=[translater(prodMatVect(matDil,sommet),trans) for sommet in carre]

    TD1questionDilat2.add(TD1questionDilat2.polygon(carre_out, fill='#FF0000', stroke="#000000", opacity=0.5))
    TD1questionDilat2.save()

# == DILATATION + ROTATION == #

TD1questionDilat3 = svgwrite.Drawing(str(chemin_fic / "TD1questionDilat3.svg"), size=(800, 600))

carre = [(-100, -100),(100,-100),(100,100),(-100,100)]

for i in range(5):

    trans = (200*((i+1)/2), 200)

    matDil = Matdilatation(.5**(i))

    carre_out=[translater(prodMatVect(matDil,sommet),trans) for sommet in carre]

    TD1questionDilat3.add(TD1questionDilat3.polygon(carre_out, fill='#FF0000', stroke="#000000", opacity=0.5))
    TD1questionDilat3.save()

# == MATRICE DE ROTATION + DILATATION == #

TD1questionDilat5 = svgwrite.Drawing(str(chemin_fic / "TD1questionDilat5.svg"), size=(800, 600))

for i in range(25):

    trans = (100+i**2, 200)

    matDil = Matdilatation(.5)

    matModelisation=prodMatMat(Matrotation(i*.5),matDil)
    carre_out=[translater(prodMatVect(matModelisation,sommet),trans) for sommet in carre]

    TD1questionDilat5.add(TD1questionDilat5.polygon(carre_out, fill='#FF0000', stroke="#000000", opacity=0.4))

TD1questionDilat5.save()

# == MATRICE DE ROTATION + DILATATION == #

TD1questionDilat6 = svgwrite.Drawing(str(chemin_fic / "TD1questionDilat6.svg"), size=(800, 600))

for i in range(9):
    trans = (100 + i * 90, 200)
    
    coef_dilat = 0.85 ** i
    matDil = Matdilatation(coef_dilat)
    
    angle = i * math.pi / 8
    matRot = Matrotation(angle)
    
    # Composition : rotation puis dilatation
    matModelisation = prodMatMat(matDil, matRot)
    
    carre_out = [translater(prodMatVect(matModelisation, sommet), trans) for sommet in carre]
    
    TD1questionDilat6.add(TD1questionDilat6.polygon(carre_out, fill='#FF0000', stroke="#000000", opacity=0.7))

TD1questionDilat6.save()

# == EXERCICE LIMITER LES CALCULS == #

TD1questionLC1 = svgwrite.Drawing(str(chemin_fic / "TD1questionLC1.svg"), size=(800, 600))

aux=100
points=[(aux, aux),(-aux, aux),(-aux, -aux),(aux, -aux)]
triangles=[0,1,2,0,2,3]

i=0
TD1questionLC1.add(TD1questionLC1.polygon((points[triangles[3*i]],points[triangles[3*i+1]],points[triangles[3*i+2]]), fill='blue',  opacity=0.5,stroke='black'))

i=1
TD1questionLC1.add(TD1questionLC1.polygon((points[triangles[3*i]],points[triangles[3*i+1]],points[triangles[3*i+2]]), fill='blue',  opacity=0.5,stroke='black'))

TD1questionLC1.save()

# == EXERCICE du chat == #

TD1questionChat1 = svgwrite.Drawing(str(chemin_fic / "TD1questionChat1.svg"), size=(800, 600))
trans = (400, 300)

points=[
    (0,50), (-100,0), (100,0), 
    (-100, -70), (-50, 0),
    (0, -30), (50, 0),
    (100, 0), (100, -70)
]

triangles=[
    0,1,2,
    1,3,4,
    4,5,6,
    6,7,8
]

rota_chat = [translater(rotation(sommet, pi/8), (0,0)) for sommet in points]
compose(rota_chat,triangles, TD1questionChat1, trans)

TD1questionChat1.save()

# == EXERCICE du chat + dilatation + rotation == #

TD1questionChat2 = svgwrite.Drawing(str(chemin_fic / "TD1questionChat2.svg"), size=(800, 600))
trans = (100, 300)

points=[
    (0,50), (-100,0), (100,0), 
    (-100, -70), (-50, 0),
    (0, -30), (50, 0),
    (100, 0), (100, -70)
]

triangles=[
    0,1,2,
    1,3,4,
    4,5,6,
    6,7,8
]

for i in range(5):

    if i == 0:
        rota_chat = [translater(rotation(sommet, 0), (0,0)) for sommet in points]
    else:
        rota_chat = [translater(rotation(sommet, -pi/(i+1)), (0,0)) for sommet in points]
    
    matDil = Matdilatation(0.8**i)
    rota_chat = [prodMatVect(matDil, sommet) for sommet in rota_chat]
    compose(rota_chat,triangles, TD1questionChat2, trans)
    trans = (trans[0] + 150, trans[1])

TD1questionChat2.save()

## == EXERCICE 3D == #

aux=100
points=[[-aux, -aux, aux], #point 0 (face devant)
    [-aux, aux, aux],#point 1   (face devant)
    [aux, -aux, aux],#point 2   (face devant)
    [aux, aux, aux],#point 3    (face devant)
    [-aux, -aux, -aux],#point 4 (face arrière)
    [-aux, aux, -aux],#point 5  (face arrière)
    [aux, -aux, -aux],#point 6  (face arrière)
    [aux, aux, -aux]#point 7    (face arrière)
    ]

cube=[
    0,1,2,   #triangle 1 face 1 (devant)
    1,2,3,   #triangle 2 face 1 (devant)
    4,5,6,   #triangle 1 face 2 (arriere)
    5,6,7,   #triangle 2 face 2 (arriere)
    0,1,4,   #triangle 1 face 3 (gauche)
    1,4,5,   #triangle 2 face 3 (gauche)
    2,3,6,   #triangle 1 face 4 (droite)
    3,6,7,   #triangle 2 face 4 (droite)
    0,2,4,   #triangle 1 face 5 (bas)
    2,4,6,   #triangle 2 face 5 (bas)
    1,3,5,   #triangle 1 face 6 (haut)
    3,5,7    #triangle 2 face 6 (haut)
]

TD1question3D1 = svgwrite.Drawing(str(chemin_fic / "TD1question3D1.svg"), size=(800, 600))

points_proj = [ projection(sommet) for sommet in points ]
compose(points_proj,cube, TD1question3D1, (400, 300))

TD1question3D1.save()

# == EXERCICE 3D + Dilatation == #

TD1question3D2 = svgwrite.Drawing(str(chemin_fic / "TD1question3D2.svg"), size=(800, 600))

coef = 1.2  # Par exemple, pour agrandir le cube
mat_dilat = Matdilatation3D(coef)

points_dilates = [prodMatVect3D(mat_dilat, p) for p in points]
points_proj_dilates = [projection(sommet) for sommet in points_dilates]
compose(points_proj_dilates, cube, TD1question3D2, (400, 300))
TD1question3D2.save()

#  == EXERCICE 3D + Rotation == #
TD1question3D3 = svgwrite.Drawing(str(chemin_fic / "TD1question3D3.svg"), size=(800, 600))

angle_x = math.radians(25)
angle_y = math.radians(30)
angle_z = math.radians(10)

mat_rot_x = Matrotation3DX(angle_x)
mat_rot_y = Matrotation3DY(angle_y)
mat_rot_z = Matrotation3DZ(angle_z)

points_rotate = [prodMatVect3D(mat_rot_x, p) for p in points]
points_rotate = [prodMatVect3D(mat_rot_y, p) for p in points_rotate]
points_rotate = [prodMatVect3D(mat_rot_z, p) for p in points_rotate]

points_proj_rotate = [projection(sommet) for sommet in points_rotate]
compose(points_proj_rotate, cube, TD1question3D3, (400, 300))
TD1question3D3.save()

## == EXERCICE 3D + chat == #
TD1question3D4 = svgwrite.Drawing(str(chemin_fic / "TD1question3D4.svg"), size=(800, 600))

trans = (100, 300)

for i in range(9):

    matdil = Matdilatation3D(0.8**i)

    angle_x = math.radians((1+i)*25)
    angle_y = math.radians((1+i)*30)
    angle_z = math.radians((1+i)*10)

    mat_rot_x = Matrotation3DX(angle_x)
    mat_rot_y = Matrotation3DY(angle_y)
    mat_rot_z = Matrotation3DZ(angle_z)
    
    points_rotate = [prodMatVect3D(mat_rot_x, p) for p in points]
    points_rotate = [prodMatVect3D(mat_rot_y, p) for p in points_rotate]
    points_rotate = [prodMatVect3D(mat_rot_z, p) for p in points_rotate]

    points_proj_rotate = [projection(sommet) for sommet in points_rotate]

    compose(points_proj_rotate, cube, TD1question3D4, trans)
    trans = (trans[0] + 150, trans[1])

TD1question3D4.save()
