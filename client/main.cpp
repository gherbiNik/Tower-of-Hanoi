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

// Prototipi
void checkBoundaries();

// Costanti
const float MAP_LIMIT = 55.0f; // hard-coded

// Variabili globali client
Eng::Base* engine;
Camera* camera;
List* list;
Node* root;

float camera_angle = 0.0f;
bool isRotationMode = false; // false = MUOVI, true = RUOTA

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

   // --- DISEGNO INTERFACCIA  ---

   engine->clearScreenText();

   // Aggiungi le righe che vuoi scrivere
   if (isRotationMode) {
       engine->addToScreenText("MODALITA': ROTAZIONE (WASD per ruotare la visuale)");
   }
   else {
       engine->addToScreenText("MODALITA': MOVIMENTO (WASD per muoverti)");
   }
   engine->addToScreenText("[M] Cambia Modalita' | [ESC] Esci");


   engine->postRedisplay();
   // Motivo: Dice all'engine "appena hai finito, ricomincia subito". 
   // Senza di questo, l'immagine si aggiorna solo se muovi il mouse.
}

// Funzione callback per la tastiera
void keyboardCallback(unsigned char key, int x, int y) {
    float moveSpeed = 5.0f;     // Velocità movimento
    float rotSpeed = 2.0f;      // Velocità rotazione 

    switch (key) {
        // --- CAMBIO MODALITÀ ---
    case 'm':
    case 'M':
        isRotationMode = !isRotationMode; 
        std::cout << "Cambio modalita': " << (isRotationMode ? "ROTAZIONE" : "MOVIMENTO") << std::endl;
        break;

        // --- MOVIMENTO / ROTAZIONE ---
    case 'w':
        if (isRotationMode) camera->rotate(rotSpeed, glm::vec3(1, 0, 0)); // Guarda su 
        else                camera->translate(glm::vec3(0, 0, -moveSpeed)); // Vai avanti
        break;

    case 's':
        if (isRotationMode) camera->rotate(-rotSpeed, glm::vec3(1, 0, 0)); // Guarda giù
        else                camera->translate(glm::vec3(0, 0, moveSpeed)); // Vai indietro
        break;

    case 'a':
        if (isRotationMode) camera->rotate(rotSpeed, glm::vec3(0, 1, 0)); // Ruota sx 
        else                camera->translate(glm::vec3(-moveSpeed, 0, 0)); // Vai sx
        break;

    case 'd':
        if (isRotationMode) camera->rotate(-rotSpeed, glm::vec3(0, 1, 0)); // Ruota dx 
        else                camera->translate(glm::vec3(moveSpeed, 0, 0)); // Vai dx
        break;

        
    case 'q': // Alzati / Ruota asse Z (Roll)
        if (isRotationMode) camera->rotate(rotSpeed, glm::vec3(0, 0, 1));
        else                camera->translate(glm::vec3(0, moveSpeed, 0));
        break;

    case 'e': // Abbassati / Ruota asse Z
        if (isRotationMode) camera->rotate(-rotSpeed, glm::vec3(0, 0, 1));
        else                camera->translate(glm::vec3(0, -moveSpeed, 0));
        break;

    case 27: exit(0); break; // ESC
    }

    // Evita di uscire dalla scena (muri)
    if (!isRotationMode) {
        checkBoundaries();
    }

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
   // --- SETUP VISTA FRONTALE ---

    // Hard-coded
   camera->translate(glm::vec3(0.0f, 25.0f, 50.0f));
   
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

void checkBoundaries() {
    float y_lower_bound = 5.0f;

    glm::mat4 mat = camera->getM();
    glm::vec3 pos = glm::vec3(mat[3]);

    bool collision = false;

    // Controllo ASSE X 
    if (pos.x > MAP_LIMIT) {
        pos.x = MAP_LIMIT;
        collision = true;
    }
    else if (pos.x < -MAP_LIMIT) {
        pos.x = -MAP_LIMIT;
        collision = true;
    }

    // Controllo ASSE Y 
    if (pos.y > MAP_LIMIT) {
        pos.y = MAP_LIMIT;
        collision = true;
    }
    else if (pos.y < y_lower_bound) {
        pos.y = y_lower_bound;
        collision = true;
    }

    // Controllo ASSE Z 
    if (pos.z > MAP_LIMIT) {
        pos.z = MAP_LIMIT;
        collision = true;
    }
    else if (pos.z < -MAP_LIMIT) {
        pos.z = -MAP_LIMIT;
        collision = true;
    }

    // Se c'è stata una collisione, aggiorniamo la matrice della camera
    if (collision) {
        mat[3][0] = pos.x; 
        mat[3][1] = pos.y;
        mat[3][2] = pos.z; 

        camera->setM(mat); 
    }
}