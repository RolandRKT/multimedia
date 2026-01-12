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

#define concat(first, second) first second

#include "config.h"
#include "GLError.h"
#include "repere.h"

#define ENABLE_SHADERS

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define NBMESHES 4

repere rep(1.0);

// Structures de données comme demandé dans le TD6
struct shaderProg
{
    unsigned int progid;
    unsigned int mid;
    unsigned int vid;
    unsigned int pid;
    unsigned int LightID;
} shaders[NBMESHES];

struct maillage
{
    shaderProg shader;
    unsigned int vaoids;
    unsigned int nbtriangles;
    float angle = 0.0f;
    float scale = 0.0f;
    float inc = 0.1f;
    float x, y, z;
} maillages[NBMESHES];

// Matrices 4x4
glm::mat4 model;
glm::mat4 view;
glm::mat4 proj;

std::array<float, 3> eye = {0.0f, 0.0f, 5.0f};

// Fonction 1: Initialisation des shaders
shaderProg initShaders(std::string shadVert, std::string shadFrag)
{
    shaderProg sp;
    unsigned int vsid, fsid;
    int status;
    int logsize;
    std::string log;

    std::ifstream vs_ifs(MY_SHADER_PATH + shadVert);
    std::ifstream fs_ifs(MY_SHADER_PATH + shadFrag);

    if (!vs_ifs || !fs_ifs)
    {
        throw std::runtime_error("Cannot find shader files! Check paths.");
    }

    // Lecture du vertex shader
    auto begin = vs_ifs.tellg();
    vs_ifs.seekg(0, std::ios::end);
    auto end = vs_ifs.tellg();
    vs_ifs.seekg(0, std::ios::beg);
    auto size = end - begin;

    std::string vs;
    vs.resize(size);
    vs_ifs.read(&vs[0], size);

    // Lecture du fragment shader
    begin = fs_ifs.tellg();
    fs_ifs.seekg(0, std::ios::end);
    end = fs_ifs.tellg();
    fs_ifs.seekg(0, std::ios::beg);
    size = end - begin;

    std::string fs;
    fs.resize(size);
    fs_ifs.read(&fs[0], size);

    // Compilation du vertex shader
    vsid = glCreateShader(GL_VERTEX_SHADER);
    char const *vs_char = vs.c_str();
    glShaderSource(vsid, 1, &vs_char, nullptr);
    glCompileShader(vsid);

    glGetShaderiv(vsid, GL_COMPILE_STATUS, &status);
    if (!status)
    {
        std::cerr << "Error: vertex shader compilation failed.\n";
        glGetShaderiv(vsid, GL_INFO_LOG_LENGTH, &logsize);
        log.resize(logsize);
        glGetShaderInfoLog(vsid, log.size(), &logsize, &log[0]);
        std::cerr << log << std::endl;
    }

    // Compilation du fragment shader
    fsid = glCreateShader(GL_FRAGMENT_SHADER);
    char const *fs_char = fs.c_str();
    glShaderSource(fsid, 1, &fs_char, nullptr);
    glCompileShader(fsid);

    glGetShaderiv(fsid, GL_COMPILE_STATUS, &status);
    if (!status)
    {
        std::cerr << "Error: fragment shader compilation failed.\n";
        glGetShaderiv(fsid, GL_INFO_LOG_LENGTH, &logsize);
        log.resize(logsize);
        glGetShaderInfoLog(fsid, log.size(), &logsize, &log[0]);
        std::cerr << log << std::endl;
    }

    // Création du programme shader
    sp.progid = glCreateProgram();
    glAttachShader(sp.progid, vsid);
    glAttachShader(sp.progid, fsid);
    glLinkProgram(sp.progid);

    // Récupération des locations des uniforms
    sp.mid = glGetUniformLocation(sp.progid, "m");
    sp.vid = glGetUniformLocation(sp.progid, "v");
    sp.pid = glGetUniformLocation(sp.progid, "p");
    sp.LightID = glGetUniformLocation(sp.progid, "LightPos");

    return sp;
}

// Fonction 2: Initialisation des VAOs
maillage initVAOs(shaderProg shader, std::string meshFile)
{
    maillage currentMaillage;
    currentMaillage.shader = shader;

    unsigned int vboids[4];

    std::ifstream ifs(MY_SHADER_PATH + meshFile);
    if (!ifs)
    {
        throw std::runtime_error("Cannot find mesh file: " + meshFile);
    }

    std::string off;
    unsigned int nbpoints, tmp;

    ifs >> off;
    ifs >> nbpoints;
    ifs >> currentMaillage.nbtriangles;
    ifs >> tmp;

    std::cout << "Loading " << meshFile << " - Triangles: " << currentMaillage.nbtriangles << std::endl;

    std::vector<float> vertices(nbpoints * 3);
    std::vector<unsigned int> indices(currentMaillage.nbtriangles * 3);
    std::vector<float> normals(nbpoints * 3);

    // Lecture des vertices
    for (unsigned int i = 0; i < vertices.size(); ++i)
    {
        ifs >> vertices[i];
    }

    // Lecture des indices
    for (unsigned int i = 0; i < currentMaillage.nbtriangles; ++i)
    {
        ifs >> tmp;
        ifs >> indices[i * 3];
        ifs >> indices[i * 3 + 1];
        ifs >> indices[i * 3 + 2];
    }

    // Calcul de la boîte englobante
    float dx, dy, dz;
    float xmin, xmax, ymin, ymax, zmin, zmax;

    xmin = xmax = vertices[0];
    ymin = ymax = vertices[1];
    zmin = zmax = vertices[2];

    for (unsigned int i = 1; i < nbpoints; ++i)
    {
        if (xmin > vertices[i * 3])
            xmin = vertices[i * 3];
        if (xmax < vertices[i * 3])
            xmax = vertices[i * 3];
        if (ymin > vertices[i * 3 + 1])
            ymin = vertices[i * 3 + 1];
        if (ymax < vertices[i * 3 + 1])
            ymax = vertices[i * 3 + 1];
        if (zmin > vertices[i * 3 + 2])
            zmin = vertices[i * 3 + 2];
        if (zmax < vertices[i * 3 + 2])
            zmax = vertices[i * 3 + 2];
    }

    // Centre de la boîte
    currentMaillage.x = (xmax + xmin) / 2.0f;
    currentMaillage.y = (ymax + ymin) / 2.0f;
    currentMaillage.z = (zmax + zmin) / 2.0f;

    // Dimensions
    dx = xmax - xmin;
    dy = ymax - ymin;
    dz = zmax - zmin;

    // Scale
    currentMaillage.scale = 1.0f / fmax(dx, fmax(dy, dz));

    // Calcul des normales
    for (std::size_t i = 0; i < indices.size(); i += 3)
    {
        auto x0 = vertices[3 * indices[i]] - vertices[3 * indices[i + 1]];
        auto y0 = vertices[3 * indices[i] + 1] - vertices[3 * indices[i + 1] + 1];
        auto z0 = vertices[3 * indices[i] + 2] - vertices[3 * indices[i + 1] + 2];

        auto x1 = vertices[3 * indices[i]] - vertices[3 * indices[i + 2]];
        auto y1 = vertices[3 * indices[i] + 1] - vertices[3 * indices[i + 2] + 1];
        auto z1 = vertices[3 * indices[i] + 2] - vertices[3 * indices[i + 2] + 2];

        auto x01 = y0 * z1 - y1 * z0;
        auto y01 = z0 * x1 - z1 * x0;
        auto z01 = x0 * y1 - x1 * y0;

        auto norminv = 1.0f / std::sqrt(x01 * x01 + y01 * y01 + z01 * z01);
        x01 *= norminv;
        y01 *= norminv;
        z01 *= norminv;

        normals[3 * indices[i]] += x01;
        normals[3 * indices[i] + 1] += y01;
        normals[3 * indices[i] + 2] += z01;

        normals[3 * indices[i + 1]] += x01;
        normals[3 * indices[i + 1] + 1] += y01;
        normals[3 * indices[i + 1] + 2] += z01;

        normals[3 * indices[i + 2]] += x01;
        normals[3 * indices[i + 2] + 1] += y01;
        normals[3 * indices[i + 2] + 2] += z01;
    }

    // Normalisation des normales
    for (std::size_t i = 0; i < normals.size(); i += 3)
    {
        auto &x = normals[i];
        auto &y = normals[i + 1];
        auto &z = normals[i + 2];

        auto norminv = 1.0f / std::sqrt(x * x + y * y + z * z);
        x *= norminv;
        y *= norminv;
        z *= norminv;
    }

    // Création du VAO - ATTENTION: utiliser & devant vaoids
    glGenVertexArrays(1, &currentMaillage.vaoids);
    glBindVertexArray(currentMaillage.vaoids);

    glGenBuffers(4, vboids);

    // VBO vertices
    glBindBuffer(GL_ARRAY_BUFFER, vboids[0]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    auto pos = glGetAttribLocation(shader.progid, "in_pos");
    glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(pos);

    // VBO indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboids[2]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // VBO normales
    glBindBuffer(GL_ARRAY_BUFFER, vboids[3]);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), normals.data(), GL_STATIC_DRAW);
    auto normal = glGetAttribLocation(shader.progid, "in_normal");
    glVertexAttribPointer(normal, 3, GL_FLOAT, GL_TRUE, 0, 0);
    glEnableVertexAttribArray(normal);

    glBindVertexArray(0);

    return currentMaillage;
}

// Fonction 3: Affichage d'un maillage
void displayMesh(maillage &m, glm::mat4 modelMatrix)
{
    // Activation du shader
    glUseProgram(m.shader.progid);

    // Application des transformations de centrage et scaling
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-m.x, -m.y, -m.z));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(m.scale));

    // Envoi des matrices uniform
    glUniformMatrix4fv(m.shader.mid, 1, GL_FALSE, &modelMatrix[0][0]);
    glUniformMatrix4fv(m.shader.vid, 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(m.shader.pid, 1, GL_FALSE, &proj[0][0]);

    // Bind du VAO et dessin
    glBindVertexArray(m.vaoids);
    glDrawElements(GL_TRIANGLES, m.nbtriangles * 3, GL_UNSIGNED_INT, 0);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    view = glm::lookAt(glm::vec3(eye[0], eye[1], eye[2]),
                       glm::vec3(eye[0], eye[1], eye[2] - 1.0f),
                       glm::vec3(0.0f, 1.0f, 0.0f));

    float decal = 1.25f;

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-decal, -decal, 0.0f));
    displayMesh(maillages[0], model);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(decal, decal, 0.0f));
    displayMesh(maillages[1], model);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-decal, decal, 0.0f));
    displayMesh(maillages[2], model);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(decal, -decal, 0.0f));
    model = glm::scale(model, glm::vec3(0.70f));
    displayMesh(maillages[3], model);

    glutSwapBuffers();
}

void idle()
{
    glutPostRedisplay();
}

void reshape(int w, int h)
{
    glViewport(0, 0, w, h);
    proj = glm::perspective(45.0f, w / static_cast<float>(h), 0.1f, 100.0f);
}

void special(int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_LEFT:
        eye[0] -= 0.1f;
        break;
    case GLUT_KEY_RIGHT:
        eye[0] += 0.1f;
        break;
    case GLUT_KEY_UP:
        eye[2] -= 0.1f;
        break;
    case GLUT_KEY_DOWN:
        eye[2] += 0.1f;
        break;
    }
    glutPostRedisplay();
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
#if defined(__APPLE__) && defined(ENABLE_SHADERS)
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_3_2_CORE_PROFILE);
#else
    glutInitContextVersion(3, 2);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glewInit();
#endif

    glutInitWindowSize(640, 480);
    glutCreateWindow(argv[0]);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);
    glutSpecialFunc(special);

#if not defined(__APPLE__)
    glewInit();
#endif

    glEnable(GL_DEPTH_TEST);

    check_gl_error();

    // Initialisation selon les consignes du TD6
    shaders[0] = initShaders("/shaders/phong.vert.glsl", "/shaders/phong.frag.glsl");
    maillages[0] = initVAOs(shaders[0], "/meshes/space_shuttle2.off");

    shaders[1] = initShaders("/shaders/phong.vert.glsl", "/shaders/toon.frag.glsl");
    maillages[1] = initVAOs(shaders[1], "/meshes/space_station2.off");

    shaders[2] = initShaders("/shaders/phong.vert.glsl", "/shaders/phongVert.frag.glsl");
    maillages[2] = initVAOs(shaders[2], "/meshes/milleniumfalcon.off");

    shaders[3] = initShaders("/shaders/phong.vert.glsl", "/shaders/phongRouge.frag.glsl");
    maillages[3] = initVAOs(shaders[3], "/meshes/rabbit.off");

    check_gl_error();

    rep.init();

    check_gl_error();

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    glutMainLoop();

    return 0;
}