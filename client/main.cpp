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
   #include <string>
   #include <algorithm>
   #include <limits>
   #include <cstdlib>
   #include <optional>
   #include <cmath>
   #include <array>



//////////
// MAIN //
//////////

// Forward declarations
struct DiscState;

// Prototipi
void checkBoundaries();
void specialCallback(int key, int x, int y);
void initHanoiState(Node* root);
void updateSelectedPeg(int delta);
void pickupDisc();
void dropDisc();
int parseDiscSize(const std::string& name);
void placeDisc(DiscState& disc, int pegIdx, int stackIdx);
void updateHeldDiscVisual(float t);
void buildCameraPresets();
void applyCameraPreset(int idx);

// Costanti
const float MAP_LIMIT = 55.0f; // hard-coded
constexpr int GLUT_KEY_LEFT = 100;
constexpr int GLUT_KEY_UP = 101;
constexpr int GLUT_KEY_RIGHT = 102;
constexpr int GLUT_KEY_DOWN = 103;

// Variabili globali client
Eng::Base* engine;
Camera* camera;
List* list;
Node* root;

float camera_angle = 0.0f;
bool isRotationMode = false; // false = MUOVI, true = RUOTA
bool isPresetView = false;   // true quando si è in una delle telecamere fisse
std::array<glm::vec3, 4> camPresets{};
glm::vec3 camTargetCenter{ 0.0f };
glm::mat4 mainCameraHome{ 1.0f };

// Stato Torre di Hanoi
struct DiscState {
   Node* node;
   int sizeRank; // rank alto = disco più grande (Disco1 = 7, Disco7 = 1)
   glm::mat4 baseTransform; // orientamento/scala originale da preservare
};

std::vector<DiscState> pegStacks[3];
glm::vec3 pegPositions[3];
float baseLevelY = 0.0f;
float stackStepY = 0.7f; // valore predefinito, ricalcolato all'avvio
int selectedPeg = 0;
std::optional<DiscState> heldDisc;

void displayCallback() {
   // 1. LOGICA (Animazioni)
   // Esempio: Ruota l'intera scena (nodo root) o un oggetto specifico
   static float angle = 0.0f;
   angle += 0.5f;

   // Aggiorna animazione disco in mano (hover + bobbing)
   updateHeldDiscVisual(angle);

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
   engine->addToScreenText("[M] Cambia Modalita' | [P] Camera principale | [ESC] Esci");
   engine->addToScreenText("Freccia SX/DX: cambia piolo | SU: prendi disco | GIU: lascia");
   std::string status = "Piolo selezionato: " + std::to_string(selectedPeg + 1);
   if (heldDisc.has_value()) {
      status += " | Disco in mano: " + std::to_string(heldDisc->sizeRank);
   }
   else {
      auto& stack = pegStacks[selectedPeg];
      if (!stack.empty()) {
         status += " | Disco in cima: " + std::to_string(stack.back().sizeRank);
      }
      else {
         status += " | Nessun disco su questo piolo";
      }
   }
   engine->addToScreenText(status);


   engine->postRedisplay();
   // Motivo: Dice all'engine "appena hai finito, ricomincia subito". 
   // Senza di questo, l'immagine si aggiorna solo se muovi il mouse.
}

// Funzione callback per la tastiera
void keyboardCallback(unsigned char key, int x, int y) {
    float moveSpeed = 5.0f;     // Velocit� movimento
    float rotSpeed = 2.0f;      // Velocit� rotazione 

    switch (key) {
        // --- CAMBIO MODALIT� ---
    case 'm':
    case 'M':
        isRotationMode = !isRotationMode; 
        std::cout << "Cambio modalita': " << (isRotationMode ? "ROTAZIONE" : "MOVIMENTO") << std::endl;
        break;

    // --- CAMERA PRINCIPALE ---
    case 'p':
    case 'P':
        camera->setM(mainCameraHome);
        isPresetView = false;
        std::cout << "Camera principale attiva (mobile)" << std::endl;
        break;

        // --- MOVIMENTO / ROTAZIONE ---
    case 'w':
        if (isPresetView) break;
        if (isRotationMode) camera->rotate(rotSpeed, glm::vec3(1, 0, 0)); // Guarda su 
        else                camera->translate(glm::vec3(0, 0, -moveSpeed)); // Vai avanti
        break;

    case 's':
        if (isPresetView) break;
        if (isRotationMode) camera->rotate(-rotSpeed, glm::vec3(1, 0, 0)); // Guarda gi�
        else                camera->translate(glm::vec3(0, 0, moveSpeed)); // Vai indietro
        break;

    case 'a':
        if (isPresetView) break;
        if (isRotationMode) camera->rotate(rotSpeed, glm::vec3(0, 1, 0)); // Ruota sx 
        else                camera->translate(glm::vec3(-moveSpeed, 0, 0)); // Vai sx
        break;

    case 'd':
        if (isPresetView) break;
        if (isRotationMode) camera->rotate(-rotSpeed, glm::vec3(0, 1, 0)); // Ruota dx 
        else                camera->translate(glm::vec3(moveSpeed, 0, 0)); // Vai dx
        break;

        
    case 'q': // Alzati / Ruota asse Z (Roll)
        if (isPresetView) break;
        if (isRotationMode) camera->rotate(rotSpeed, glm::vec3(0, 0, 1));
        else                camera->translate(glm::vec3(0, moveSpeed, 0));
        break;

    case 'e': // Abbassati / Ruota asse Z
        if (isPresetView) break;
        if (isRotationMode) camera->rotate(-rotSpeed, glm::vec3(0, 0, 1));
        else                camera->translate(glm::vec3(0, -moveSpeed, 0));
        break;

    // --- TELECAMERE FISSE ---
    case '1': applyCameraPreset(0); isPresetView = true; break;
    case '2': applyCameraPreset(1); isPresetView = true; break;
    case '3': applyCameraPreset(2); isPresetView = true; break;
    case '4': applyCameraPreset(3); isPresetView = true; break;

    case 27: exit(0); break; // ESC
    }

    // Evita di uscire dalla scena (muri)
    if (!isRotationMode && !isPresetView) {
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

   // In GLM (Column-Major), la posizione � nella quarta colonna (index 3)
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
   engine->setSpecialCallback(specialCallback);

   engine->setDisplayCallback(displayCallback);
   engine->setReshapeCallback(reshapeCallback);

   camera = new Camera("MainCam");
   // --- SETUP VISTA FRONTALE ---

    // Hard-coded
   camera->translate(glm::vec3(0.0f, 25.0f, 50.0f));
   mainCameraHome = camera->getM(); // salva posizione iniziale della camera mobile
   
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

      // Opzionale: scala o sposta il tavolo se � troppo grande/piccolo
      // tavoloNode->scale(glm::vec3(0.1f)); 

      root = tavoloNode;
      root->addChild(light);
      root->addChild(camera);

      initHanoiState(root);


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

void updateSelectedPeg(int delta) {
   selectedPeg = (selectedPeg + delta + 3) % 3;
}

int parseDiscSize(const std::string& name) {
   // Disco1 = più grande, Disco7 = più piccolo (rank alto = disco più grande)
   if (name.rfind("Disco", 0) != 0) return 0;
   int n = std::atoi(name.c_str() + 5);
   if (n <= 0) return 0;
   return 8 - n; // Disco1 -> 7 (grande), Disco7 -> 1 (piccolo)
}

void placeDisc(DiscState& disc, int pegIdx, int stackIdx) {
   glm::vec3 pos = pegPositions[pegIdx];
   pos.y = baseLevelY + stackStepY * static_cast<float>(stackIdx);
   glm::mat4 m = disc.baseTransform;
   m[3] = glm::vec4(pos, 1.0f); // sostituisce solo la traslazione, mantiene scala/rotazione
   disc.node->setM(m);
}

void updateHeldDiscVisual(float t) {
   if (!heldDisc.has_value()) return;
   auto& stack = pegStacks[selectedPeg];
   int topIdx = static_cast<int>(stack.size()); // posizione sopra la cima
   glm::vec3 pos = pegPositions[selectedPeg];
   float lift = 1.0f;
   float bob = 0.2f * std::sin(t * 0.05f);
   pos.y = baseLevelY + stackStepY * static_cast<float>(topIdx) + lift + bob;
   glm::mat4 m = heldDisc->baseTransform;
   m[3] = glm::vec4(pos, 1.0f);
   heldDisc->node->setM(m);
}

void buildCameraPresets() {
   // posizioni basate sul centro dei pioli
   glm::vec3 center = camTargetCenter;
   float span = glm::length(pegPositions[2] - pegPositions[0]);
   float dist = std::max(30.0f, span * 1.5f);

   camPresets[0] = center + glm::vec3(0.0f, dist * 0.6f, dist);    // frontale alto
   camPresets[1] = center + glm::vec3(dist, dist * 0.5f, 0.0f);    // laterale destra
   camPresets[2] = center + glm::vec3(0.0f, dist * 1.8f, 0.0f);    // top-down
   camPresets[3] = center + glm::vec3(-dist, dist * 0.7f, -dist);  // isometrica opposta
}

void applyCameraPreset(int idx) {
   if (idx < 0 || idx >= (int)camPresets.size()) return;
   glm::vec3 pos = camPresets[idx];
   glm::vec3 up = (idx == 2) ? glm::vec3(0, 0, -1) : glm::vec3(0, 1, 0); // top-down senza rollare
   glm::mat4 view = glm::lookAt(pos, camTargetCenter, up);
   glm::mat4 world = glm::inverse(view);
   camera->setM(world);
}

void initHanoiState(Node* root) {
   // Trova pioli
   Node* palo1 = root->findByName("Palo1");
   Node* palo2 = root->findByName("Palo2");
   Node* palo3 = root->findByName("Palo3");
   if (!palo1 || !palo2 || !palo3) {
      std::cerr << "Impossibile trovare i pioli (Palo1/2/3)" << std::endl;
      return;
   }
   pegPositions[0] = glm::vec3(palo1->getWorldFinalMatrix()[3]);
   pegPositions[1] = glm::vec3(palo2->getWorldFinalMatrix()[3]);
   pegPositions[2] = glm::vec3(palo3->getWorldFinalMatrix()[3]);
   camTargetCenter = (pegPositions[0] + pegPositions[1] + pegPositions[2]) / 3.0f;

   // Trova dischi
   std::vector<DiscState> discs;
   for (int i = 1; i <= 7; ++i) {
      std::string name = "Disco" + std::to_string(i);
      Node* d = root->findByName(name);
      if (!d) continue;
      DiscState ds{ d, parseDiscSize(name), d->getM() };
      discs.push_back(ds);
   }

   // Assegna a peg più vicino (XZ)
   for (auto& ds : discs) {
      glm::vec3 pos = glm::vec3(ds.node->getWorldFinalMatrix()[3]);
      float best = std::numeric_limits<float>::max();
      int bestPeg = 0;
      for (int p = 0; p < 3; ++p) {
         glm::vec2 a{ pos.x, pos.z };
         glm::vec2 b{ pegPositions[p].x, pegPositions[p].z };
         float d2 = glm::dot(a - b, a - b);
         if (d2 < best) {
            best = d2;
            bestPeg = p;
         }
      }
      pegStacks[bestPeg].push_back(ds);
   }

   // Ordina ciascun peg per Y crescente (dal basso al top)
   for (int p = 0; p < 3; ++p) {
      std::sort(pegStacks[p].begin(), pegStacks[p].end(),
         [](const DiscState& a, const DiscState& b) {
            float ya = glm::vec3(a.node->getWorldFinalMatrix()[3]).y;
            float yb = glm::vec3(b.node->getWorldFinalMatrix()[3]).y;
            return ya < yb;
         });
   }

   // Calcola base e passo usando il peg più popolato
   int pegMax = 0;
   for (int p = 1; p < 3; ++p) {
      if (pegStacks[p].size() > pegStacks[pegMax].size()) pegMax = p;
   }
   if (pegStacks[pegMax].size() > 1) {
      float minY = glm::vec3(pegStacks[pegMax].front().node->getWorldFinalMatrix()[3]).y;
      float maxY = glm::vec3(pegStacks[pegMax].back().node->getWorldFinalMatrix()[3]).y;
      stackStepY = (maxY - minY) / float(pegStacks[pegMax].size() - 1);
      baseLevelY = minY;
   }
   else if (!pegStacks[pegMax].empty()) {
      baseLevelY = glm::vec3(pegStacks[pegMax].front().node->getWorldFinalMatrix()[3]).y;
      stackStepY = 0.7f;
   }

   // Riallinea dischi agli stack
   for (int p = 0; p < 3; ++p) {
      for (int i = 0; i < (int)pegStacks[p].size(); ++i) {
         placeDisc(pegStacks[p][i], p, i);
      }
   }

   // Prepara telecamere fisse
   buildCameraPresets();

   selectedPeg = pegMax;
   heldDisc.reset();
}

void pickupDisc() {
   if (heldDisc.has_value()) return; // già in mano
   auto& stack = pegStacks[selectedPeg];
   if (stack.empty()) return;
   heldDisc = stack.back();
   stack.pop_back();
}

void dropDisc() {
   if (!heldDisc.has_value()) return;
   auto& stack = pegStacks[selectedPeg];
   // Vietato posare un disco più grande sopra uno più piccolo
   if (!stack.empty() && heldDisc->sizeRank > stack.back().sizeRank) {
      return;
   }
   int idx = static_cast<int>(stack.size());
   placeDisc(*heldDisc, selectedPeg, idx);
   stack.push_back(*heldDisc);
   heldDisc.reset();
}

void specialCallback(int key, int x, int y) {
   switch (key) {
   case GLUT_KEY_LEFT:  updateSelectedPeg(-1); break;
   case GLUT_KEY_RIGHT: updateSelectedPeg(+1); break;
   case GLUT_KEY_UP:    pickupDisc(); break;
   case GLUT_KEY_DOWN:  dropDisc(); break;
   default: return;
   }
   engine->postRedisplay();
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

    // Se c'� stata una collisione, aggiorniamo la matrice della camera
    if (collision) {
        mat[3][0] = pos.x; 
        mat[3][1] = pos.y;
        mat[3][2] = pos.z; 

        camera->setM(mat); 
    }
}