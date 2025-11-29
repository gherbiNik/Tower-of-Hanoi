#include "list.h"
#include <GL/freeglut.h>
#include <glm/gtc/type_ptr.hpp>

ENG_API List::List() : Object(0, "RenderList") {}
List::~List() { clear(); }

void List::pass(Node* node, glm::mat4 parentMatrix) {
   if (!node) return;

   // Calcola la World Matrix corrente (accumulando le trasformazioni)
   glm::mat4 currentWorldMatrix = parentMatrix * node->getM();

   // Aggiungi alla lista di rendering
   Instance inst;
   inst.node = node;
   inst.nodeWorldMatrix = currentWorldMatrix;
   instances.push_back(inst);

   // Ricorsione sui figli
   for (unsigned int i = 0; i < node->getNumChildren(); i++) {
      pass(node->getChild(i), currentWorldMatrix);
   }
}

// Renderizza la lista usando la View Matrix (inversa della camera)
void List::render(glm::mat4 viewMatrix) {
   for (auto& inst : instances) {
      // Calcola ModelView = View * World
      glm::mat4 modelView = viewMatrix * inst.nodeWorldMatrix;

      // Carica la matrice ModelView in OpenGL
      glMatrixMode(GL_MODELVIEW);
      glLoadMatrixf(glm::value_ptr(modelView));

      // Renderizza il nodo (Mesh, Light, ecc.)
      inst.node->render();
   }
}

// Versione overload senza argomenti (se richiesto dall'interfaccia base Object)
void List::render() {
   // Fallback: usa matrice identità se non viene passata la view matrix
   render(glm::mat4(1.0f));
}

void List::clear() {
   instances.clear();
}