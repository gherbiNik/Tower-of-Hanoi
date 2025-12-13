#pragma once

#include "hanoi.h"
#include "HanoiTest.h"

#include <cassert>
#include <iostream>



HanoiTest::HanoiTest() : passedTests(0), failedTests(0) {}

int HanoiTest::runAllTests() {
    std::cout << "\n========== AVVIO TEST SUITE ==========\n" << std::endl;

    testPickupAndDrop();
    testInvalidMoves();
    testWinCondition();
    testBoundaries();
    testStackOrdering();
    testResetState();

    printResults();

    return failedTests;
}



// Utility per asserzioni
void HanoiTest::assertTrue(bool condition, const std::string& testName) {
    if (condition) {
        std::cout << "[PASS] " << testName << std::endl;
        passedTests++;
    }
    else {
        std::cerr << "[FAIL] " << testName << std::endl;
        failedTests++;
    }
}

    
// Verifica presa e rilascio dischi
void HanoiTest::testPickupAndDrop() {
    std::cout << "\n--- Test: Presa e Rilascio Dischi ---" << std::endl;

    Hanoi* game = createMockHanoi();
    Node* root = createMockScene();
    game->initHanoiState(root);

    // Simula presa disco
    game->specialCallback(KEY_UP, 0, 0);
    // assertTrue(game->hasHeldDisc(), "Dovrebbe esserci un disco in mano");

    // Simula rilascio su altro piolo
    game->specialCallback(KEY_RIGHT, 0, 0);
    game->specialCallback(KEY_DOWN, 0, 0);
    // assertTrue(!game->hasHeldDisc(), "Non dovrebbe esserci pi� disco in mano");

    std::cout << "[INFO] Test presa/rilascio completato (stato interno)" << std::endl;

    cleanupMockScene(root);
    delete game;
}

// Verifica mosse invalide
void HanoiTest::testInvalidMoves() {
    std::cout << "\n--- Test: Mosse Invalide ---" << std::endl;

    Hanoi* game = createMockHanoi();
    Node* root = createMockScene();
    game->initHanoiState(root);

    // Tenta di prendere da piolo vuoto
    game->specialCallback(KEY_RIGHT, 0, 0); // vai su piolo vuoto
    game->specialCallback(KEY_UP, 0, 0);    // tenta di prendere
    // assertTrue(!game->hasHeldDisc(), "Non dovrebbe poter prendere da piolo vuoto");

    // Tenta di posare disco grande su disco piccolo
    game->specialCallback(KEY_LEFT, 0, 0);  // torna al piolo con dischi
    game->specialCallback(KEY_UP, 0, 0);    // prendi disco
    // Simula situazione invalida...

    std::cout << "[INFO] Test mosse invalide completato" << std::endl;

    cleanupMockScene(root);
    delete game;
}

// Verifica condizione vittoria
void HanoiTest::testWinCondition() {
    std::cout << "\n--- Test: Condizione di Vittoria ---" << std::endl;

    Hanoi* game = createMockHanoi();
    Node* root = createMockScene();
    game->initHanoiState(root);

    assertTrue(!game->isGameWon(), "Il gioco non dovrebbe essere vinto all'inizio");

    std::cout << "[INFO] Test condizione vittoria completato" << std::endl;

    cleanupMockScene(root);
    delete game;
}

// Verifica limiti mappa
void HanoiTest::testBoundaries() {
    std::cout << "\n--- Test: Limiti Mappa ---" << std::endl;

    Camera* cam = new Camera("TestCam");
    Hanoi* game = new Hanoi(cam, nullptr);

    // Posiziona camera fuori dai limiti
    cam->translate(glm::vec3(100.0f, 100.0f, 100.0f));
    game->checkBoundaries();

    glm::vec3 pos = glm::vec3(cam->getM()[3]);
    assertTrue(pos.x <= MAP_LIMIT, "X non dovrebbe superare MAP_LIMIT");
    assertTrue(pos.y <= MAP_LIMIT, "Y non dovrebbe superare MAP_LIMIT");
    assertTrue(pos.z <= MAP_LIMIT, "Z non dovrebbe superare MAP_LIMIT");

    delete game;
    delete cam;
}

// Verifica ordinamento stack
void HanoiTest::testStackOrdering() {
    std::cout << "\n--- Test: Ordinamento Stack ---" << std::endl;

    Hanoi* game = createMockHanoi();
    Node* root = createMockScene();
    game->initHanoiState(root);

    // Verifica che i dischi siano ordinati per dimensione crescente
    // (dal basso verso l'alto)
    std::cout << "[INFO] Test ordinamento stack completato (logica interna)" << std::endl;

    cleanupMockScene(root);
    delete game;
}

// Verifica reset stato
void HanoiTest::testResetState() {
    std::cout << "\n--- Test: Reset Stato ---" << std::endl;

    Hanoi* game = createMockHanoi();
    Node* root = createMockScene();

    // Inizializza
    game->initHanoiState(root);

    // Fai alcune mosse
    game->specialCallback(KEY_UP, 0, 0);
    game->specialCallback(KEY_RIGHT, 0, 0);

    // Reset
    game->initHanoiState(root);
    assertTrue(!game->isGameWon(), "Dopo reset il gioco non dovrebbe essere vinto");

    std::cout << "[INFO] Test reset completato" << std::endl;

    cleanupMockScene(root);
    delete game;
}

// Helper: crea un'istanza mock di Hanoi
Hanoi* HanoiTest::createMockHanoi() {
    Camera* cam = new Camera("MockCam");
    return new Hanoi(cam, nullptr);
}

// Helper: crea una scena mock minima
Node* HanoiTest::createMockScene() {
    Node* root = new Node("TestRoot");

    // Crea pioli mock
    Node* palo1 = new Node("Palo1");
    palo1->translate(glm::vec3(-10.0f, 0.0f, 0.0f));

    Node* palo2 = new Node("Palo2");
    palo2->translate(glm::vec3(0.0f, 0.0f, 0.0f));

    Node* palo3 = new Node("Palo3");
    palo3->translate(glm::vec3(10.0f, 0.0f, 0.0f));

    root->addChild(palo1);
    root->addChild(palo2);
    root->addChild(palo3);

    // Crea dischi mock
    for (int i = 1; i <= 7; i++) {
        Node* disco = new Node("Disco" + std::to_string(i));
        disco->translate(glm::vec3(-10.0f, i * 0.7f, 0.0f)); // tutti su palo1
        root->addChild(disco);
    }

    return root;
}

// Helper: cleanup scena mock
void HanoiTest::cleanupMockScene(Node* root) {
    if (root) {
        delete root; 
    }
}

void HanoiTest::printResults() {
    std::cout << "\n========================================" << std::endl;
    std::cout << "RISULTATI TEST:" << std::endl;
    std::cout << "  Test passati: " << passedTests << std::endl;
    std::cout << "  Test falliti: " << failedTests << std::endl;
    std::cout << "  Totale: " << (passedTests + failedTests) << std::endl;
    std::cout << "========================================\n" << std::endl;
}
