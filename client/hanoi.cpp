#include "hanoi.h"



Hanoi::Hanoi(Camera* cam, Eng::Base* eng)
    : camera(cam), engine(eng)
{
    camPresets.resize(4);
}


void Hanoi::updateSelectedPeg(int delta) {
    selectedPeg = (selectedPeg + delta + 3) % 3;
    updateSelectionVisuals();
}

int Hanoi::parseDiscSize(const std::string& name) {
    // Disco1 = più grande, Disco7 = più piccolo (rank alto = disco più grande)
    if (name.rfind("Disco", 0) != 0) return 0;
    int n = std::atoi(name.c_str() + 5);
    if (n <= 0) return 0;
    return 8 - n; // Disco1 -> 7 (grande), Disco7 -> 1 (piccolo)
}

void Hanoi::placeDisc(DiscState& disc, int pegIdx, int stackIdx) {
    glm::vec3 pos = pegPositions[pegIdx];
    pos.y = baseLevelY + stackStepY * static_cast<float>(stackIdx);
    glm::mat4 m = disc.baseTransform;
    m[3] = glm::vec4(pos, 1.0f); // sostituisce solo la traslazione, mantiene scala/rotazione
    disc.node->setM(m);
}

void Hanoi::updateHeldDiscVisual(float t) {
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

void Hanoi::buildCameraPresets() {
    // posizioni basate sul centro dei pioli
    glm::vec3 center = camTargetCenter;
    float span = glm::length(pegPositions[2] - pegPositions[0]);
    float dist = std::max(30.0f, span * 1.5f);

    camPresets[0] = center + glm::vec3(0.0f, dist * 0.6f, dist);     // frontale alto
    camPresets[1] = center + glm::vec3(dist, dist * 0.5f, 0.0f);     // laterale destra
    camPresets[2] = center + glm::vec3(0.0f, dist * 0.9f, 0.0f);     // top-down
    camPresets[3] = center + glm::vec3(-dist, dist * 0.3f, -dist);   // isometrica opposta
}

void Hanoi::applyCameraPreset(int idx) {
    if (idx < 0 || idx >= (int)camPresets.size()) return;
    glm::vec3 pos = camPresets[idx];
    glm::vec3 up = (idx == 2) ? glm::vec3(0, 0, -1) : glm::vec3(0, 1, 0); // top-down senza rollare
    glm::mat4 view = glm::lookAt(pos, camTargetCenter, up);
    glm::mat4 world = glm::inverse(view);
    camera->setM(world);
}

void Hanoi::updateSelectionVisuals() {
    // Controllo di sicurezza
    if (selectedPeg < 0 || selectedPeg > 2) return;

    // Prendiamo la posizione X e Z del piolo attuale
    glm::vec3 pegPos = pegPositions[selectedPeg];

    // 1. Sposta la Luce (Spot001)
    if (selectionSpot) {
        glm::mat4 m = selectionSpot->getM();
        // Modifichiamo solo X e Z, lasciando Y (altezza) invariata
        m[3][0] = pegPos.x;
        m[3][2] = pegPos.z;
        selectionSpot->setM(m);
    }

    // 2. Sposta la Piramide (Pyramid001)
    if (selectionPyramid) {
        glm::mat4 m = selectionPyramid->getM();
        m[3][0] = pegPos.x;
        m[3][2] = pegPos.z;
        selectionPyramid->setM(m);
    }
}

void Hanoi::initHanoiState(Node* root) {
    // Trova pioli

    // In caso di Reset della scena [R]: bisogna pulire le var
    for (int i = 0; i < 3; i++) {
        pegStacks[i].clear();
    }
    heldDisc.reset();
    isWon = false; // Reset stato vittoria

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

    selectionSpot = root->findByName("Spot001");
    selectionPyramid = root->findByName("Pyramid001");

    // Aggiorna subito la posizione iniziale
    updateSelectionVisuals();
}

void Hanoi::pickupDisc() {
    if (heldDisc.has_value()) return; // già in mano
    auto& stack = pegStacks[selectedPeg];
    if (stack.empty()) return;
    heldDisc = stack.back();
    stack.pop_back();
}

void Hanoi::dropDisc() {
    if (!heldDisc.has_value()) return;

    auto& stack = pegStacks[selectedPeg];
    // Vietato posare un disco più grande sopra uno più piccolo
    if (!stack.empty() && heldDisc->sizeRank > stack.back().sizeRank) {
        return;
    }
    int idx = static_cast<int>(stack.size());
    stack.push_back(*heldDisc);
    placeDisc(stack.back(), selectedPeg, idx);

    heldDisc.reset();

    if (checkWinCondition()) {
        isWon = true;
        std::cout << "[GAME] Hai vinto!" << std::endl;
    }
}

void Hanoi::specialCallback(int key, int x, int y) {
    if (isWon) return; //Blocca il gioco se hai vinto

    switch (key) {
    case KEY_LEFT:  updateSelectedPeg(-1); break;
    case KEY_RIGHT: updateSelectedPeg(+1); break;
    case KEY_UP:    pickupDisc(); break;
    case KEY_DOWN:  dropDisc(); break;
    default: return;
    }
}

void Hanoi::checkBoundaries() {
    if (!camera) return;

    float y_lower_bound = 5.0f;
    glm::mat4 mat = camera->getM();
    glm::vec3 pos = glm::vec3(mat[3]);
    bool collision = false;

    if (pos.x > MAP_LIMIT) {
        pos.x = MAP_LIMIT;
        collision = true;
    }
    else if (pos.x < -MAP_LIMIT) {
        pos.x = -MAP_LIMIT;
        collision = true;
    }

    if (pos.y > MAP_LIMIT) {
        pos.y = MAP_LIMIT;
        collision = true;
    }
    else if (pos.y < y_lower_bound) {
        pos.y = y_lower_bound;
        collision = true;
    }

    if (pos.z > MAP_LIMIT) {
        pos.z = MAP_LIMIT;
        collision = true;
    }
    else if (pos.z < -MAP_LIMIT) {
        pos.z = -MAP_LIMIT;
        collision = true;
    }

    if (collision) {
        mat[3][0] = pos.x; mat[3][1] = pos.y; mat[3][2] = pos.z;
        camera->setM(mat);
    }
}

bool Hanoi::checkWinCondition() {
    int totalDiscs = pegStacks[0].size() + pegStacks[1].size() + pegStacks[2].size();

    if (heldDisc.has_value()) { // va compreso nel calcolo
        totalDiscs++;
    }
    // Non puoi vincere se stai ancora tenendo un disco
    if (heldDisc.has_value()) {
        return false;
    }

    // Tutti i dischi devono essere sull'ultimo piolo (indice 0 -> Palo3 ).
    // Nota: dipende dal POV (che camera stai usando)
    if (pegStacks[0].size() == totalDiscs && totalDiscs > 0) {
        return true; // == 2 per test
    }

    return false;
}