#include "mesh.h"
#include <GL/freeglut.h> // Include OpenGL

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
      material->render(); // Imposta colori, shader o texture
   }
   else {
      // Fallback se non c'è materiale
      glDisable(GL_LIGHTING);
      glDisable(GL_TEXTURE_2D);
      glColor3f(1.0f, 1.0f, 1.0f);
   }

   // 2. Disegna Geometria
   if (!vertices.empty()) {
      glBegin(GL_TRIANGLES);
      // Itera a passi di 3 (x, y, z)
      for (size_t i = 0; i < vertices.size(); i += 3) {
         // Nota: se hai le normali nel vettore, dovresti chiamare glNormal3f qui
         glVertex3f(vertices[i], vertices[i + 1], vertices[i + 2]);
      }
      glEnd();
   }

   // 3. Ripristina stato (opzionale ma consigliato)
   if (!material) glEnable(GL_LIGHTING);
}