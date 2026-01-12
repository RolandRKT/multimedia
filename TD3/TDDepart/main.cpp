#include <iostream>
#include <vector>
#include <array>
#include <fstream>

#if defined(__APPLE__)
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl3.h>
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/freeglut.h>
#endif

#include "config.h"

#define ENABLE_SHADERS

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Identifiant du programme envoyé à la carte graphique.
unsigned int progid;
// Identifiant de la matrice de transformation (3D->2D) envoyée à la carte graphique.
unsigned int mvpid;
// variable globale contenant l’angle de rotation à appliquer
float r = 0.0f;

// Matrices de transformation.
glm::mat4 view;
glm::mat4 proj;
glm::mat4 mvp;

// Identifiant des tableaux passés à la carte graphique.
unsigned int vaoids[1];

// fonction void idle() qui sera appelée en boucle pour par exemple incrémenter l’angle de rotation
void idle()
{
  // Incrémentation de l'angle de rotation.
  r >= 360.0f ? r = 0.0f : r += 0.3f;
  // Ré-affichage de la scène.
  glutPostRedisplay();
}

/**
 * Fonction d'affichage.
 */
void display()
{
  // 1) nettoie les buffers couleur + profondeur.
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // =========================================================
  // 2) Je construis d'abord les matrices "caméra / monde"
  // =========================================================

  // Je place la caméra en reculant la scène de 5 sur -Z.
  glm::mat4 translateCamera = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f));
  
  // Scale général pour les cubes
  glm::mat4 scaleCubes = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.5f));

  // Je crée une rotation autour de Y pour animer les deux premiers cubes.
  glm::mat4 rotateY = glm::rotate(glm::mat4(1.0f), glm::radians(r), glm::vec3(0.0f, 1.0f, 0.0f));

  // view_base : repère commun (caméra seulement, sans rotation d’animation)
  glm::mat4 view_base = translateCamera * scaleCubes;

  // view1 : repère pour les cubes animés (caméra + rotation)
  glm::mat4 view1 = view_base * rotateY;

  // =========================================================
  // 3) Cube 1 : cube central qui tourne sur lui-même
  // =========================================================

  // Pour ce cube, je n’applique que view1 (pas de translation locale en plus).
  glm::mat4 modelCube1 = glm::mat4(1.0f); // modèle identité (pas de déplacement propre)
  glm::mat4 mvpCube1 = proj * view1 * modelCube1;

  glUniformMatrix4fv(mvpid, 1, GL_FALSE, &mvpCube1[0][0]);
  glBindVertexArray(vaoids[0]);
  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);

  // =========================================================
  // 4) Cube 2 : cube au-dessus du premier, qui tourne avec lui (orbite)
  // =========================================================

  // Je crée une translation locale pour placer le cube 2 au-dessus du cube 1.
  glm::mat4 translateHCube2 = glm::translate(glm::mat4(1.0f), glm::vec3(3.0f, 0.0f, 0.0f));

  // Son modèle = translation locale au-dessus du cube 1.
  glm::mat4 modelCube2 = translateHCube2;

  // Il est toujours dans le repère qui tourne (view1), donc il tourne avec le premier.
  glm::mat4 mvpCube2 = proj * view1 * modelCube2;

  glUniformMatrix4fv(mvpid, 1, GL_FALSE, &mvpCube2[0][0]);
  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);

  // =========================================================
  // 5) Cube 3 : cube fixe au-dessus des deux autres
  // =========================================================
  
  // Je crée une translation générale au-dessus des deux premiers cubes.
  glm::mat4 translateUpCubes = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 3.0f, 0.0f));

  glm::mat4 view_base_2 = view_base * translateUpCubes;

  // Je crée une mise à l’échelle pour ce cube (plus petit).
  glm::mat4 scaleCube3 = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.5f));

  // (scale à droite -> appliquée en premier sur le cube).
  glm::mat4 modelCube3 = scaleCube3;

  // Son modèle = d’abord l’échelle, puis après la translation locale.
  glm::mat4 mvpCube3 = proj * view_base_2 * modelCube3;

  glUniformMatrix4fv(mvpid, 1, GL_FALSE, &mvpCube3[0][0]);
  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);

  // =========================================================
  // 5) Cube 4 : petit cube qui tourne sur lui-même axe x en orbite du cube 3
  // =========================================================

  glm::mat4 scaleCube4 = glm::scale(glm::mat4(1.0f), glm::vec3(0.3f, 0.3f, 0.3f));
  glm::mat4 rotateCube4 = glm::rotate(glm::mat4(1.0f), glm::radians(r), glm::vec3(1.0f, 0.0f, 0.0f));

  glm::mat4 modelCube4 = scaleCube4;

  // Son modèle = d’abord l’échelle, puis après la translation locale.
  glm::mat4 mvpCube4 = proj * view_base_2 * modelCube4;

  glUniformMatrix4fv(mvpid, 1, GL_FALSE, &mvpCube4[0][0]);
  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);

  // =========================================================
  // 6) Je swap les buffers
  // =========================================================
  glutSwapBuffers();
}

/**
 * Fonction appelée à chaque redimensionnement de la fenêtre.
 */
void reshape(int w, int h)
{
  // Modification de la zone d'affichage OpenGL.
  glViewport(0, 0, w, h);
  // Matrice de projection perspective.
  proj = glm::perspective(glm::radians(60.0f), w / static_cast<float>(h), 0.1f, 100.0f);
}

/**
 * Fonction définissant le maillage et effectuant son envoi sur la carte graphique.
 */
void initVAOs()
{
  // Identifiant du maillage.
  unsigned int vboids[3];

  // Sommets d’un cube (6 faces * 2 triangles * 3 sommets = 36 sommets).
  std::vector<float> vertices = {
      // Face avant (z = 0.5)
      -0.5f, -0.5f, 0.5f,
      0.5f, -0.5f, 0.5f,
      0.5f, 0.5f, 0.5f,
      -0.5f, 0.5f, 0.5f,
      // Face arrière (z = -0.5)
      -0.5f, -0.5f, -0.5f,
      0.5f, -0.5f, -0.5f,
      0.5f, 0.5f, -0.5f,
      -0.5f, 0.5f, -0.5f};

  // Couleurs par sommet (8 sommets).
  std::vector<float> colors = {
      1.0f, 0.0f, 0.0f,
      1.0f, 1.0f, 0.0f,
      0.0f, 1.0f, 1.0f,
      0.0f, 0.0f, 1.0f,
      1.0f, 0.0f, 1.0f,
      0.0f, 1.0f, 0.0f,
      1.0f, 1.0f, 1.0f,
      0.0f, 0.0f, 0.0f};

  // Indices pour 12 triangles (cube fermé).
  std::vector<unsigned short> indices = {
      // Avant
      0, 1, 2, 2, 3, 0,
      // Arrière
      5, 4, 7, 7, 6, 5,
      // Gauche
      4, 0, 3, 3, 7, 4,
      // Droite
      1, 5, 6, 6, 2, 1,
      // Haut
      3, 2, 6, 6, 7, 3,
      // Bas
      4, 5, 1, 1, 0, 4};

  glGenVertexArrays(1, &vaoids[0]);
  glBindVertexArray(vaoids[0]);

  glGenBuffers(3, vboids);

  // Positions
  glBindBuffer(GL_ARRAY_BUFFER, vboids[0]);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
  auto pos = glGetAttribLocation(progid, "in_pos");
  glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(pos);

  // Couleurs
  glBindBuffer(GL_ARRAY_BUFFER, vboids[1]);
  glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(float), colors.data(), GL_STATIC_DRAW);
  auto color = glGetAttribLocation(progid, "in_color");
  glVertexAttribPointer(color, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(color);

  // Indices
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboids[2]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), indices.data(), GL_STATIC_DRAW);

  glBindVertexArray(0);
}

/**
 * Initialisation des vertex et fragment shaders.
 */
void initShaders()
{
  unsigned int vsid, fsid;

  std::string vs(
      "#version 150 core\n"
      "in vec3 in_pos;\n"
      "in vec3 in_color;\n"
      "uniform mat4 mvp;\n"
      "out vec3 color;\n"
      "void main(void)\n"
      "{\n"
      "  gl_Position = mvp * vec4( in_pos, 1.0 );\n"
      "  color = in_color;\n"
      "}\n");

  std::string fs(
      "#version 150 core\n"
      "in vec3 color;\n"
      "out vec4 frag_color;\n"
      "void main(void)\n"
      "{\n"
      "  frag_color = vec4( color, 1.0 );\n"
      "}\n");

  vsid = glCreateShader(GL_VERTEX_SHADER);
  const char *vs_char = vs.c_str();
  glShaderSource(vsid, 1, &vs_char, nullptr);
  glCompileShader(vsid);

  fsid = glCreateShader(GL_FRAGMENT_SHADER);
  const char *fs_char = fs.c_str();
  glShaderSource(fsid, 1, &fs_char, nullptr);
  glCompileShader(fsid);

  progid = glCreateProgram();
  glAttachShader(progid, vsid);
  glAttachShader(progid, fsid);
  glLinkProgram(progid);
  glUseProgram(progid);

  mvpid = glGetUniformLocation(progid, "mvp");
}

int main(int argc, char *argv[])
{
  glutInit(&argc, argv);

#if defined(__APPLE__) && defined(ENABLE_SHADERS)
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_3_2_CORE_PROFILE);
#else
  glutInitContextVersion(3, 2);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
#endif

  glutInitWindowSize(1024, 768);
  glutCreateWindow(argv[0]);

#if not defined(__APPLE__)
  glewInit();
#endif

  // Test de profondeur activé
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutIdleFunc(idle);

  initShaders();
  initVAOs();

  glClearColor(0.5f, 0.7f, 0.1213451f, 0.3f);

  glutMainLoop();
  return 0;
}
