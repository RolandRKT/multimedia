from math import *

ANGLE = pi / 4

def translater(input, vectTrans):
    x, y = input
    tx, ty = vectTrans
    return (x + tx, y + ty)


def rotation(point, angle):
    x, y = point
    out = (x * cos(angle) - y * sin(angle), x * sin(angle) + y * cos(angle))
    return out


def prodMatVect(Mat, Vect):
    x, y = Vect
    ( (a11,a12), (a21, a22) ) = Mat

    x2 = a11 * x + a12 * y
    y2 = a21 * x + a22 * y

    return (x2,y2)


def Matrotation(angle):
    return ((cos(angle), -sin(angle)),
            (sin(angle), cos(angle)))


def Matdilatation(coefDilatation):
    return ((coefDilatation,0),
        (0, coefDilatation))

def prodMatMat(MatA, MatB):
    ( (a11, a12), (a21, a22) ) = MatA
    ( (b11, b12), (b21, b22) ) = MatB

    return (
        (a11 * b11 + a12 * b21, a11 * b12 + a12 * b22),
        (a21 * b11 + a22 * b21, a21 * b12 + a22 * b22)
    )

# Il ne reste plus qu'à faire la multiplication de matrices et puis l'utiliser:

# Le chat

def compose(points, triangles, dessin, trans=(0,0)):
  liste_point=[]
  colors=("blue","red","green","purple","yellow","white","coral","darkblue")
  for i in range(0,len(triangles)//3):
      print(triangles[3*i],triangles[3*i+1],triangles[3*i+2])
      out = translater(points[triangles[3*i]], trans)
      dessin.add(dessin.polygon((out,translater(points[triangles[3*i+1]], trans),translater(points[triangles[3*i+2]], trans)), fill=colors[(i%(len(colors)*2))//2],  opacity=0.5,stroke='black'))

## == EXERCICE 3D == #

def projection(point3d):
    return((point3d[0],point3d[1]))

def deplacement_pt(point, direction):
    x, y, z = point
    dx, dy, dz = direction
    return (x + dx, y + dy, z + dz)

def prodMatVect3D(Mat, Vect ):
    x, y,z = Vect
    ( (a11,a12,a13), (a21, a22,a23), (a31, a32,a33) ) = Mat

    x2 = a11 * x + a12 * y+ a13 * z
    y2 = a21 * x + a22 * y+ a23 * z
    z2 = a31 * x + a32 * y+ a33 * z

    return (x2,y2,z2)

def prodMatMat3D(MatA, MatB ):

    ( (a11,a12,a13),
    (a21, a22,a23),
    (a31, a32,a33) )  = MatA

    ( (b11,b12,b13),
    (b21, b22,b23),
    (b31, b32,b33) )  = MatB

    return ( (a11*b11+a12* b21+a13*b31 , a11*b12+a12* b22+a13*b32,a11*b13+a12* b23+a13*b33  ),
     (  a21*b11+a22* b21+ a23*b31, a21*b12+a22* b22 +a23*b32,  a21*b13+a22* b23+a23*b33),
     (  a31*b11+a32* b21+ a33*b31, a31*b12+a32* b22 +a33*b32,  a31*b13+a32* b23+a33*b33))

def Matdilatation3D(coefDilatation):
         return ((coefDilatation,0,0),
                 (0, coefDilatation , 0),
                 (0, 0, coefDilatation))

def Matrotation3DX(angle):
    return (
        (1,0,0),
        (0,cos(angle), -sin(angle)),
        (0,sin(angle), cos(angle))
    )

def Matrotation3DY(angle):
     return (
            (cos(angle), 0,sin(angle)),
            (0,1,0,),
            (-sin(angle),0, cos(angle)))

def Matrotation3DZ(angle):
    return (
        (cos(angle), -sin(angle),0),
        (sin(angle), cos(angle),0),
        (0,0,1)
    )
