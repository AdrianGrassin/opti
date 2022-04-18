//
// Created by grass on 18/04/2022.
//
#include <fstream>
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

void Grafo::build(const std::ifstream &file) {

}

Grafo::Grafo(const std::string &filepath) {
  std::ifstream file(filepath);

  if (file.is_open()) {
    build(file);
  } else {
    throw std::invalid_argument("El archivo no existe o la dirección del fichero: " + filepath + " es erronea.\n");
  }

}

bool Grafo::esdirigido() const {
  return dirigido;
}

void Grafo::actualizargrafo(const std::string& nuevografo) {


}

