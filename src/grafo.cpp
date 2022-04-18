//
// Created by grass on 18/04/2022.
//
#include <fstream>
#include <iostream>
#include "../include/grafo.h"

void Grafo::destroy() {
  if (!ListaPredecesores.empty()) {
    ListaPredecesores.resize(0);
  }

  if (!ListaSucesores.empty()) {
    ListaSucesores.resize(0);
  }

  if (!MatrizAdyacencia.empty()) {
    MatrizAdyacencia.resize(0);
  }

}

void Grafo::build(const std::ifstream &file, const std::string& pathname) {
  path = pathname;
}

Grafo::Grafo(const std::string &filepath) {
  std::ifstream file(filepath);

  if (file.is_open()) {
    build(file, filepath);
  } else {
    throw std::invalid_argument("El archivo no existe o la dirección del fichero: " + filepath + " es erronea.\n >");
  }

}

bool Grafo::esdirigido() const {
  return dirigido;
}

void Grafo::actualizargrafo(std::string& nuevografo) {
  std::ifstream file(nuevografo);

  if (file.is_open()) {
    destroy();
    build(file, nuevografo);
  } else {
    system("CLS");
    std::cout << "La ruta especificada no coincide con ningún archivo, puedes volver a intentarlo o salir con \"q\":\n >";
    std::cin >> nuevografo;

    if (nuevografo == "q") {
      return;
    } else {
      actualizargrafo(nuevografo);
    }
  }
}
const std::string &Grafo::pathto() {
  return path;
}

