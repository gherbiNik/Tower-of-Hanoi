#pragma once

#include <vector>
#include <string>
#include <optional>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <limits>

#include "ovoReader.h"
#include "engine.h"


// Costanti
const float MAP_LIMIT = 50.0f; 
const int KEY_LEFT = 100;      
const int KEY_RIGHT = 102;
const int KEY_UP = 101;
const int KEY_DOWN = 103;

struct DiscState {
    Node* node;
    int sizeRank;
    glm::mat4 baseTransform;
};

class Hanoi {
public:
    Hanoi(Camera* cam, Eng::Base* eng);
    ~Hanoi() = default;

    // Metodi principali chiamati dal Main
    void initHanoiState(Node* root);
    void updateHeldDiscVisual(float t);
    void specialCallback(int key, int x, int y);
    void checkBoundaries();
    void applyCameraPreset(int idx);

    // Getters / Setters utili
    bool isGameWon() const { return isWon; }

private:
    // Dipendenze esterne
    Camera* camera;
    Eng::Base* engine;

    // Stato del gioco
    int selectedPeg = 0;
    bool isWon = false;
    std::vector<DiscState> pegStacks[3];
    std::optional<DiscState> heldDisc;

    // Parametri spaziali
    glm::vec3 pegPositions[3];
    glm::vec3 camTargetCenter;
    float baseLevelY = 0.0f;
    float stackStepY = 0.0f;

    Node* selectionSpot = nullptr;
    Node* selectionPyramid = nullptr;

    std::vector<glm::vec3> camPresets;

    // Metodi di logica interna
    void updateSelectedPeg(int delta);
    int parseDiscSize(const std::string& name);
    void placeDisc(DiscState& disc, int pegIdx, int stackIdx);
    void buildCameraPresets();
    void updateSelectionVisuals();
    void pickupDisc();
    void dropDisc();
    bool checkWinCondition();

};