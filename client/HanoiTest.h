#pragma once

#include "hanoi.h"
#include <cassert>
#include <iostream>
#include <string>

/**
 * Classe per i test unitari della classe Hanoi
 * Verifica il corretto funzionamento della logica di gioco
 */
class HanoiTest {
public:
    HanoiTest();
    ~HanoiTest() = default;

    // Metodo principale per eseguire tutti i test
    void runAllTests();

private:
    int passedTests;
    int failedTests;

    void assertTrue(bool condition, const std::string& testName);

    void testPickupAndDrop();
    void testInvalidMoves();
    void testBoundaries();
    void testStackOrdering();
    void testResetState();

    // Helper per creazione mock
    Hanoi* createMockHanoi();
    Node* createMockScene();
    void cleanupMockScene(Node* root);

    int testParseDiscSize(const std::string& name);
    int testUpdateSelectedPeg(int currentPeg, int delta);
    void testDoublePickup();
        
    void testSelectionWrapAround(); // Test selezione circolare (0 -> sx -> 2)
    void testSimpleWin();           // Test vittoria con scenario semplificato
    

    // Stampa risultati
    void printResults();
};