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
Mesh* triangleMesh = nullptr;

void displayCallback() {
   static float angle = 0.0f;
   angle += 1.0f; // Velocità rotazione

   if (triangleMesh) {
      // Resetta la matrice locale e applica la nuova rotazione
      // (Nota: glm::mat4(1.0f) crea una matrice identità)
      triangleMesh->setM(glm::mat4(1.0f));
      triangleMesh->rotate(angle, glm::vec3(0.0f, 1.0f, 0.0f)); // Ruota su asse Y
   }

   // 2. PREPARA LISTA DI RENDERING
   list->clear();
   // Attraversa il grafo partendo dalla radice con matrice identità
   list->pass(root, glm::mat4(1.0f));

   // 3. COMUNICAZIONE CON L'ENGINE
   // Passiamo i dati aggiornati che l'engine userà per disegnare
   engine->setRenderList(list);
   engine->setMainCamera(camera);
}

void reshapeCallback(int width, int height) {
   if(height == 0) height = 1;
   float aspect = (float)width / (float)height;

   // Aggiorna la matrice di proiezione della camera
   // FOV 45°, Near 0.1, Far 100.0
   camera->setProjectionMatrix(glm::perspective(glm::radians(45.0f), aspect, 0.1f, 2000.0f));
}

int main(int argc, char* argv[]) {
   engine = &Eng::Base::getInstance();

   if (!engine->init(argc, argv)) return -1;

   engine->createWindow(800, 600, 100, 100, "Tower of Hanoi");

   engine->enableFPS();

   // Setup Callback
   engine->setDisplayCallback(displayCallback);
   engine->setReshapeCallback(reshapeCallback);

   // Setup Scena
   camera = new Camera("MainCam");
   camera->translate(glm::vec3(20, 20, 120)); // Sposta indietro la camera

   list = new List();
   root = new Node("Root");

   // Aggiungi Luce
   OmnidirectionalLight* light = new OmnidirectionalLight();
   light->setPosition(glm::vec4(0.0f, 0.0f, 2.0f, 1.0f));
   root->addChild(light);

   Material* myMat = new Material("RedMaterial");
   myMat->setAmbient(glm::vec4(0.1f, 0.0f, 0.0f, 1.0f));
   myMat->setDiffuse(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)); // Rosso
   myMat->setSpecular(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
   myMat->setShininess(64.0f);
  

   triangleMesh = new Mesh("TestMesh");

   // Definiamo i vertici (x, y, z) per un triangolo equilatero
   std::vector<glm::vec3> vertices = {
        glm::vec3(-2.0f, -2.0f, 0.0f),    
        glm::vec3(2.0f, -2.0f, 0.0f),     
        glm::vec3(0.0f,  2.0f, 0.0f)      
   };

   triangleMesh->set_all_vertices(vertices);
   triangleMesh->setMaterial(myMat);

   root->addChild(triangleMesh); // Aggiungi al grafo

   // --- SETUP CALLBACK ---
   engine->setDisplayCallback(displayCallback);
   engine->setReshapeCallback(reshapeCallback);

   // --- AVVIO ---
   std::cout << "Avvio loop..." << std::endl;
   OvoReader ovoreader{};
   Node* tavoloNode = ovoreader.readFile("tavolo.ovo", "texture");

   // 2. CONTROLLA E AGGIUNGI AL GRAFO
   if (tavoloNode) {
      std::cout << "OVO caricato con successo! Aggiungo alla scena." << std::endl;

      // Opzionale: scala o sposta il tavolo se è troppo grande/piccolo
      // tavoloNode->scale(glm::vec3(0.1f)); 

      root->addChild(tavoloNode); // <--- PASSAGGIO FONDAMENTALE MANCANTE
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
   delete myMat;

   return 0;
}
