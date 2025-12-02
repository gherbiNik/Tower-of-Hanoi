#include "mesh.h"
#include <GL/freeglut.h>
#include <iostream>

Mesh::Mesh(const std::string& name)
   : Node(name), material(nullptr)
{
}

const std::vector<float>& Mesh::getVertices() const { return vertices; }
Material* Mesh::getMaterial() const { return material; }

void Mesh::setVertices(const std::vector<float>& v) { vertices = v; }
void Mesh::setMaterial(Material* m) { material = m; }

void Mesh::render() {
   // 1. Applica Materiale
   if (material) {
      material->render(); // Attiva luci e setta i coefficienti kA, kD, kS
   }
   else {
      glDisable(GL_LIGHTING);
      glDisable(GL_TEXTURE_2D);
      glColor3f(1.0f, 1.0f, 1.0f);
   }

   // 2. Disegna Geometria
   if (!vertices.empty()) {
      glBegin(GL_TRIANGLES);

      // --- FIX IMPORTANTE ---
      // Diciamo a OpenGL che questa faccia "guarda" verso Z positivo (verso di noi).
      // Senza questo, il calcolo della luce (Dot Product) fallisce e dà nero.
      glNormal3f(0.0f, 0.0f, 1.0f);
      // ----------------------

      for (size_t i = 0; i < vertices.size(); i += 3) {
         glVertex3f(vertices[i], vertices[i + 1], vertices[i + 2]);
      }
      glEnd();
   }

   // 3. Ripristina stato
   if (!material) glEnable(GL_LIGHTING);
}