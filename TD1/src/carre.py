#!/usr/bin/env python

import svgwrite

dessin = svgwrite.Drawing('../assets/svg/carre.svg', size=(800,600))

carre=[(-100, -100),(100,-100),(100,100),(-100,100)]

dessin.add(dessin.polygon(carre, fill="#FF0000",\
                          stroke="#000000", opacity=0.7 ))

dessin.save()
