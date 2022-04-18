//
// Created by grass on 18/04/2022.
//

// ║
//

#include <iostream>
#include <windows.h>

void menu(bool dirigido, char &opcion, const std::string &grafo) {
  system("CLS");
  if (dirigido) {
    std::cout << "GRAFO CARGADO:" + grafo + "\n"
    "╔═════════════════════════════════════════════╗\n"
    "║ OPTIMIZACION 2022 ---- GRAFOS               ║\n"
    "║                                             ║\n"
    "║ MENU DE OPCIONES                            ║\n"
    "║ c. [c]argar grafo desde fichero             ║\n"
    "║ q. [q]uit / finalizar el programa           ║\n"
    "║                                             ║\n"
    "║ MENU DE GRAFO                               ║\n"
    "║ i. Mostrar [i]nfo                           ║\n"
    "║ s. Mostrar [s]ucesores                      ║\n"
    "║ p. Mostrar [p]redecesores                   ║\n"
    "╚═════════════════════════════════════════════╝\n"
    "OPCION > ";
    std::cin >> opcion;
  } else {
    "╔═════════════════════════════════════════════╗\n"
    "║ OPTIMIZACION 2022 ---- GRAFOS               ║\n"
    "║                                             ║\n"
    "║ MENU DE OPCIONES                            ║\n"
    "║ c. [c]argar grafo desde fichero             ║\n"
    "║ q. [q]uit / finalizar el programa           ║\n"
    "║                                             ║\n"
    "║ MENU DE GRAFO                               ║\n"
    "║ i. Mostrar [i]nfo                           ║\n"
    "║ a. Mostrar [a]dyacencia                     ║\n"
    "╚═════════════════════════════════════════════╝\n"
    "OPCION > ";
    std::cin >> opcion;
  }
}

void typewrite(const std::string &text) {
  for (auto const &letter : text) {
    std::cout << letter;
    Sleep(100);
  }
  std::cout << std::endl;
}