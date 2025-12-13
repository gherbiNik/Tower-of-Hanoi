#pragma once

#include "hanoi.h"
#include "HanoiTest.h"

#include <cassert>
#include <iostream>



HanoiTest::HanoiTest() : passedTests(0), failedTests(0) {}

void HanoiTest::runAllTests() {
    std::cout << "\n========== AVVIO TEST SUITE ==========\n" << std::endl;

    testPickupAndDrop();
    testInvalidMoves();
    testBoundaries();
    testStackOrdering();
    testResetState();
    testSelectionWrapAround();
    testSimpleWin();
    testDoublePickup();
    printResults();
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
    assertTrue(game->hasHeldDisc(), "Dovrebbe esserci un disco in mano");

    // Simula rilascio su altro piolo
    game->specialCallback(KEY_RIGHT, 0, 0);
    game->specialCallback(KEY_DOWN, 0, 0);
    assertTrue(!game->hasHeldDisc(), "Non dovrebbe esserci più disco in mano");

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
    assertTrue(!game->hasHeldDisc(), "Non dovrebbe poter prendere da piolo vuoto");

    // Tenta di posare disco grande su disco piccolo
    game->specialCallback(KEY_LEFT, 0, 0);  // torna al piolo con dischi
    game->specialCallback(KEY_UP, 0, 0);    // prendi disco

    std::cout << "[INFO] Test mosse invalide completato" << std::endl;

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
    Node* root = new Node("Root");

    // 1 Palo
    Node* p1 = new Node("Palo1"); root->addChild(p1);
    root->addChild(new Node("Palo2")); root->addChild(new Node("Palo3"));

    // Creiamo 2 dischi: Disco1 (alto/piccolo) e Disco2 (basso/grande)
    Node* dA = new Node("Disco1"); dA->translate(glm::vec3(0, 10, 0)); // Y alta
    Node* dB = new Node("Disco2"); dB->translate(glm::vec3(0, 0, 0));  // Y bassa

    root->addChild(dA);
    root->addChild(dB);

    game->initHanoiState(root);

    // Lo stack 0 deve essere ordinato per altezza: indice 0 = basso, indice 1 = alto
    if (game->pegStacks[0].size() >= 2) {
        assertTrue(game->pegStacks[0][0].node->getName() == "Disco2", "Disco2 (basso) deve essere alla base");
        assertTrue(game->pegStacks[0][1].node->getName() == "Disco1", "Disco1 (alto) deve essere in cima");
    }
    else {
        assertTrue(false, "Errore setup stack ordering");
    }

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

void HanoiTest::testSelectionWrapAround() {
    std::cout << "\n--- Test: Selezione Circolare (Wrap-around) ---" << std::endl;

    Hanoi* game = createMockHanoi();
    // Non serve una scena complessa, basta che selectedPeg sia inizializzato
    game->selectedPeg = 0;

    // Test Sinistra: 0 -> 2
    game->specialCallback(KEY_LEFT, 0, 0);
    assertTrue(game->selectedPeg == 2, "Da peg 0, Sinistra deve portare a peg 2");

    // Test Destra: 2 -> 0
    game->specialCallback(KEY_RIGHT, 0, 0);
    assertTrue(game->selectedPeg == 0, "Da peg 2, Destra deve portare a peg 0");

    // Test Destra: 0 -> 1 -> 2
    game->specialCallback(KEY_RIGHT, 0, 0);
    assertTrue(game->selectedPeg == 1, "Da peg 0, Destra deve portare a peg 1");
    game->specialCallback(KEY_RIGHT, 0, 0);
    assertTrue(game->selectedPeg == 2, "Da peg 1, Destra deve portare a peg 2");

    delete game;
}

void HanoiTest::testSimpleWin() {
    std::cout << "\n--- Test: Scenario Vittoria (1 Disco) ---" << std::endl;

    Hanoi* game = createMockHanoi();
    Node* root = new Node("Root");

    // Configurazione Scena Mock:
    // Creiamo i 3 pali standard distanziati sull'asse X.
    Node* p1 = new Node("Palo1"); p1->translate(glm::vec3(-10, 0, 0));
    Node* p2 = new Node("Palo2"); p2->translate(glm::vec3(0, 0, 0));
    Node* p3 = new Node("Palo3"); p3->translate(glm::vec3(10, 0, 0));
    root->addChild(p1); root->addChild(p2); root->addChild(p3);

    // Verifica di sicurezza: il gioco non deve mai iniziare in stato "Vinto"
    assertTrue(!game->isGameWon(), "Il gioco non dovrebbe essere vinto all'inizio");

    // Posizionamento disco:
    // Creiamo un solo disco ("Disco1") e lo mettiamo sul Palo 2 (indice 1).
    // In questo modo basta una sola mossa per vincere.
    Node* d1 = new Node("Disco1");
    d1->translate(glm::vec3(0, 0, 0));
    root->addChild(d1);

    // Inizializza la logica interna: il motore scansiona la posizione geometrica dei nodi
    game->initHanoiState(root);

    // Controlliamo che il disco sia sullo stack centrale
    assertTrue(game->pegStacks[1].size() == 1, "Il disco deve essere rilevato sul Palo 2");
    assertTrue(game->pegStacks[0].empty(), "Il Palo 1 (target) deve essere inizialmente vuoto");


    // Forziamo la selezione sul piolo sorgente
    game->selectedPeg = 1;

    // Simuliamo l'input di presa (Freccia SU) e verifichiamo che il disco sia "in mano"
    game->specialCallback(KEY_UP, 0, 0);
    assertTrue(game->hasHeldDisc(), "Il disco deve essere stato prelevato con successo");

    // Spostiamo la selezione verso il piolo di destinazione (Palo 1 / Indice 0)
    game->specialCallback(KEY_LEFT, 0, 0);
    assertTrue(game->selectedPeg == 0, "La selezione deve essersi spostata sul Palo 1");

    game->specialCallback(KEY_DOWN, 0, 0);

    // --- VERIFICA FINALE ---
    assertTrue(game->isGameWon(), "Il gioco deve risultare vinto spostando l'unico disco sul target");

    cleanupMockScene(root);
    delete game;
}

void HanoiTest::testDoublePickup() {
    std::cout << "\n--- Test: Robustezza Doppia Presa ---" << std::endl;

    Hanoi* game = createMockHanoi();
    Node* root = createMockScene();
    game->initHanoiState(root);

    size_t initialSize = game->pegStacks[0].size();

    // Prima Presa (Valida)
    game->specialCallback(KEY_UP, 0, 0);
    assertTrue(game->pegStacks[0].size() == initialSize - 1, "Stack decrementato");

    // Seconda Presa (Invalida - ho già un disco)
    game->specialCallback(KEY_UP, 0, 0);
    assertTrue(game->hasHeldDisc(), "Disco ancora in mano");
    assertTrue(game->pegStacks[0].size() == initialSize - 1, "Stack NON decrementato la seconda volta");

    cleanupMockScene(root);
    delete game;
}


void HanoiTest::printResults() {
    std::cout << "\n========================================" << std::endl;
    std::cout << "RISULTATI TEST:" << std::endl;
    std::cout << "  Test passati: " << passedTests << std::endl;
    std::cout << "  Test falliti: " << failedTests << std::endl;
    std::cout << "  Totale: " << (passedTests + failedTests) << std::endl;
    std::cout << "========================================\n" << std::endl;
}
