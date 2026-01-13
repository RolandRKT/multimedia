Pour compiler ce caneva:

installer les paquetages suivants:

sudo apt-get  install  cmake freeglut3-dev build-essential
sudo apt-get install libglew-dev
sudo apt-get install -y libglm-dev

puis: 
mkdir BUILD
cd BUILD
cmake ..
make 


Pour executer:
make check

ou bien:
./exo_3D

# TD6

1. j'ai ajouté les structures.
2. j'ai remplacé la fonction display par celle donné dans le TD.
3. je supprime/commente les variables qui font doublons.
4. j'ajoute la variable globale #define NBMESHES 4
5. création de la fonction displayMesh
6. mise à jour de la fonction shaderProg initShaders(std::string shadVert, std::string shadFrag)
7. initVAOs mis à jour
8. Modification du main






