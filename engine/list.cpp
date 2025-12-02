#include "list.h"
#include "light.h"
#include <GL/freeglut.h>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

ENG_API List::List() : Object("RenderList") {}
List::~List() { clear(); }

void List::pass(Node* node, glm::mat4 parentMatrix) {
   if (!node) return;

   // Calcola matrice mondo
   glm::mat4 currentWorldMatrix = parentMatrix * node->getM();

   // Aggiungi istanza
   Instance inst;
   inst.node = node;
   inst.nodeWorldMatrix = currentWorldMatrix;

   // Se è una luce, la mettiamo in testa alla lista per elaborarla prima
   if (dynamic_cast<Light*>(node) != nullptr)
      instances.push_front(inst);
   else
      instances.push_back(inst);

   // Ricorsione
   for (unsigned int i = 0; i < node->getNumChildren(); i++) {
      pass(node->getChild(i), currentWorldMatrix);
   }
}

void List::render(glm::mat4 viewMatrix) {
   int lightCounter = 0;
   const int MAX_HARDWARE_LIGHTS = 8;

   // Spegni tutte le luci per sicurezza all'inizio del frame
   for (int i = 0; i < MAX_HARDWARE_LIGHTS; i++) glDisable(GL_LIGHT0 + i);

   for (auto& inst : instances) {
      // Calcola ModelView = View * World
      glm::mat4 modelView = viewMatrix * inst.nodeWorldMatrix;
      glMatrixMode(GL_MODELVIEW);
      glLoadMatrixf(glm::value_ptr(modelView));

      // Controllo se è una luce
      Light* lightNode = dynamic_cast<Light*>(inst.node);

      if (lightNode) {
         if (lightCounter < MAX_HARDWARE_LIGHTS) {
            // Assegna slot hardware e renderizza
            lightNode->setLightID(GL_LIGHT0 + lightCounter);
            lightNode->render();
            lightCounter++;
         }
         else {
            // Limite superato
            std::cout << "NUMERO MAX RAGGIUNTO: Luce '" << lightNode->getName() << "' ignorata." << std::endl;
            lightNode->setLightID(-1); // Disabilita
         }
      }
      else {
         // È una mesh o altro nodo, renderizza normalmente
         inst.node->render();
      }
   }
}

void List::render() {
   // Fallback: se chiamato senza matrice vista, usa l'identità (nessuna trasformazione camera)
   render(glm::mat4(1.0f));
}

void List::clear() {
   instances.clear();
}