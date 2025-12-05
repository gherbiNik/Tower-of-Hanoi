/**
 * @file		main.cpp
 * @brief	Client application (that uses the graphics engine)
 *
 * @authors	 Andrea Perlini, Nicholas Gherbi, Andrea Domenico Trapani 
 */



//////////////
// #INCLUDE //
//////////////

   // Library header:
   #include "engine.h"

   #include "camera.h"
   #include "light.h"
   #include "mesh.h"
   #include "material.h"
   #include "omnidirectionalLight.h"
   #include "ovoReader.h"

   

   // C/C++:
   #include <iostream>
   #include <vector>



//////////
// MAIN //
//////////

// Variabili globali client
Eng::Base* engine;
Camera* camera;
List* list;
Node* root;


void displayCallback() {
   // 1. LOGICA (Animazioni)
   // Esempio: Ruota l'intera scena (nodo root) o un oggetto specifico
   static float angle = 0.0f;
   angle += 0.5f;

   // Se vuoi ruotare un oggetto specifico (es. il tavolo), assicurati di avere il puntatore corretto.
   // Altrimenti, per test, puoi non ruotare nulla o ruotare la root:
   // root->setM(glm::mat4(1.0f));
   // root->rotate(angle, glm::vec3(0.0f, 1.0f, 0.0f));

   // 2. PREPARA LA LISTA DI RENDERING
   list->clear();
   // Attraversa il grafo per raccogliere mesh e luci
   list->pass(root, glm::mat4(1.0f));

   // 3. PASSA I DATI ALL'ENGINE
   engine->setRenderList(list);
   engine->setMainCamera(camera);

   // --- CORREZIONI IMPORTANTI ---

   // A. RIMUOVI QUESTA RIGA: engine->render(camera, list); 
   // Motivo: L'engine chiama render() da solo appena questa funzione finisce. 
   // Lasciandola, disegni due volte per frame (sfarfallio).

   // B. AGGIUNGI QUESTA RIGA:
   engine->postRedisplay();
   // Motivo: Dice all'engine "appena hai finito, ricomincia subito". 
   // Senza di questo, l'immagine si aggiorna solo se muovi il mouse.
}

// Funzione callback per la tastiera
void keyboardCallback(unsigned char key, int x, int y) {
   float speed = 5.0f; // Velocità movimento

   switch (key) {
   case 'w': camera->translate(glm::vec3(0, 0, -speed)); break; // Avanti
   case 's': camera->translate(glm::vec3(0, 0, speed)); break;  // Indietro
   case 'a': camera->translate(glm::vec3(-speed, 0, 0)); break; // Sinistra
   case 'd': camera->translate(glm::vec3(speed, 0, 0)); break;  // Destra
   case 'q': camera->translate(glm::vec3(0, speed, 0)); break;  // Su
   case 'e': camera->translate(glm::vec3(0, -speed, 0)); break; // Giù
   case 27: exit(0); break; // ESC per uscire
   }
   // Richiede aggiornamento
   engine->postRedisplay();
}


void reshapeCallback(int width, int height) {
   if (height == 0) height = 1;
   float aspect = (float)width / (float)height;

   // MODIFICA QUI:
   // zNear = 1.0f (evita flickering su oggetti vicini)
   // zFar = 5000.0f (aumentato da 100.0f a 5000.0f per vedere tutta la stanza)
   camera->setProjectionMatrix(glm::perspective(glm::radians(45.0f), aspect, 1.0f, 5000.0f));
}

#include <iomanip> // Serve per std::setprecision

void printSceneGraphWithPosition(Node* node, int level = 0) {
   if (!node) return;

   // 1. Indentazione grafica
   std::string indent(level * 4, ' ');
   std::string branch = (level == 0) ? "ROOT " : "|__ ";

   // 2. Calcolo Posizione Assoluta (World Matrix)
   // getWorldFinalMatrix() calcola ricorsivamente la posizione reale nel mondo 3D
   glm::mat4 worldMatrix = node->getWorldFinalMatrix();

   // In GLM (Column-Major), la posizione è nella quarta colonna (index 3)
   float x = worldMatrix[3][0];
   float y = worldMatrix[3][1];
   float z = worldMatrix[3][2];

   // 3. Stampa Dati
   std::cout << indent << branch
      << "ID:" << node->getId() << " '" << node->getName() << "'";

   // Tipo di Nodo
   if (dynamic_cast<Mesh*>(node)) {
      Mesh* m = dynamic_cast<Mesh*>(node);
      std::cout << " [MESH V:" << m->get_all_vertices().size() << "]";
   }
   else if (dynamic_cast<Light*>(node)) {
      std::cout << " [LIGHT]";
   }
   else if (dynamic_cast<Camera*>(node)) {
      std::cout << " [CAMERA]";
   }

   // --- AGGIUNTA: Stampa Coordinate ---
   std::cout << std::fixed << std::setprecision(1)
      << " -> Pos: (" << x << ", " << y << ", " << z << ")" << std::endl;

   // 4. Ricorsione
   for (unsigned int i = 0; i < node->getNumChildren(); i++) {
      printSceneGraphWithPosition(node->getChild(i), level + 1);
   }
}

int main(int argc, char* argv[]) {
   engine = &Eng::Base::getInstance();

   if (!engine->init(argc, argv)) return -1;

   engine->createWindow(800, 600, 100, 100, "Tower of Hanoi");

   engine->enableFPS();

   // Setup Callback
   engine->setKeyboardCallback(keyboardCallback);

   engine->setDisplayCallback(displayCallback);
   engine->setReshapeCallback(reshapeCallback);

   camera = new Camera("MainCam");
   // Sposta indietro di 3 metri (300cm) e in alto di 1.5 metri (150cm)
   camera->translate(glm::vec3(0.0f, 150.0f, 0.0f));
   // Inclina per guardare in basso
   camera->rotate(-90.0f, glm::vec3(1.0f, 0.0f, 0.0f));



   list = new List();
   root = new Node("Root");

   // Aggiungi Luce
   OmnidirectionalLight* light = new OmnidirectionalLight();
   light->setPosition(glm::vec4(0.0f, 200.0f, 0.0f, 1.0f));
   

   
  

   

   // --- SETUP CALLBACK ---
   engine->setDisplayCallback(displayCallback);
   engine->setReshapeCallback(reshapeCallback);

   // --- AVVIO ---
   std::cout << "Avvio loop..." << std::endl;
   OvoReader ovoreader{};
   Node* tavoloNode = ovoreader.readFile("tavolo.ovo", "texture/");
   

   // 2. CONTROLLA E AGGIUNGI AL GRAFO
   if (tavoloNode) {
      std::cout << "OVO caricato con successo! Aggiungo alla scena." << std::endl;

      // Opzionale: scala o sposta il tavolo se è troppo grande/piccolo
      // tavoloNode->scale(glm::vec3(0.1f)); 

      root = tavoloNode;
      root->addChild(light);
      root->addChild(camera);

      std::cout << "\n--- STRUTTURA SCENA ---" << std::endl;
      printSceneGraphWithPosition(root);
      std::cout << "-----------------------\n" << std::endl;
   }
   else {
      std::cerr << "Errore critico: impossibile caricare tavolo.ovo" << std::endl;
   }



   engine->update(); // Avvia il loop infinito di FreeGLUT

   // Pulizia (raggiunta solo se si forza l'uscita dal loop)
   engine->free();

   // Pulizia memoria puntatori raw (opzionale per esercizio, obbligatorio in prod)
   delete root; // Dovrebbe cancellare ricorsivamente se implementato bene
   delete list;
   delete camera;

   return 0;
}
