//
// Created by grass on 18/04/2022.
//
#include <fstream>
#include <iostream>
#include <iomanip>
#include "../include/grafo.h"

// -----------CONSTRUCTORES-Y-MAS--------------- //

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

void Grafo::build(std::ifstream &file, const std::string &pathname) {
  path = pathname;

  unsigned i, k;
  file >> (unsigned &) n >> (unsigned &) m >> (bool &) dirigido;

  ListaSucesores.resize(n);

  for (k = 0; k < m; k++) {
    ElementoLista buffer;
    file >> (unsigned &) i >> (unsigned &) buffer.j >> (int &) buffer.c;
    ListaSucesores[i - 1].push_back(buffer);  // (i - 1) porque se tiene que la posición 0 = nodo 1
  }

  if (!dirigido) {
    for (i = 0; i < ListaSucesores.size(); i++) {
      if (ListaSucesores[i].empty())
        continue;
      else
        for (const auto &elemento : ListaSucesores[i]) {
          ElementoLista buffer;
          buffer.j = i + 1;
          buffer.c = elemento.c;
          ListaSucesores[elemento.j - 1].push_back(buffer);
        }
    }

    // esto es un poco feo pero lo que hace es por cada fila contar las veces que se repiten los elementos y luego
    // se les hace pop back (ya que siempre van a ser los ultimos elementos).

    unsigned popbackcount;
    for (i = 0; i < ListaSucesores.size(); i++) {
      for (int j = 0; j < ListaSucesores[i].size(); j++) {
        for (k = j + 1; k < ListaSucesores[i].size(); k++) {
          if(ListaSucesores[i][j].j == ListaSucesores[i][k].j){
            popbackcount++;
          }
        }
      }
      for(int l = 0; l < popbackcount; l++){
        ListaSucesores[i].pop_back();
      }
      popbackcount = 0;
    }
  }

  else { // construir LP
    ListaPredecesores.resize(n);
    for (i = 0; i < ListaSucesores.size(); i++) {
      if (ListaSucesores[i].empty())
        continue;
      else
        for (const auto &elemento : ListaSucesores[i]) {
          ElementoLista buffer;
          buffer.j = i + 1;
          buffer.c = elemento.c;
          ListaPredecesores[elemento.j - 1].push_back(buffer);
        }
    }
  }


}

Grafo::Grafo(const std::string &filepath) {
  std::ifstream file(filepath);

  if (file.is_open()) {
    build(file, filepath);
  } else {
    throw std::invalid_argument("El archivo no existe o la dirección del fichero: " + filepath + " es erronea.\n >");
  }

}

void Grafo::actualizargrafo(std::string &nuevografo) {
  std::ifstream file(nuevografo);

  if (file.is_open()) {
    destroy();
    build(file, nuevografo);
  } else {
    system("CLS");
    std::cout << "La ruta especificada no coincide con ningún archivo, "
                 "puedes volver a intentarlo o volver al menú con \"q\":\n"
                 " >";
    std::cin >> nuevografo;

    if (nuevografo == "q") {
      return;
    } else {
      actualizargrafo(nuevografo);
    }
  }
}

// -----------------GETTERS--------------------- //

const std::string &Grafo::pathto() {
  return path;
}

bool Grafo::esdirigido() const {
  return dirigido;
}

// -----------------FUNCIONES------------------- //

void Grafo::showadyacencia() {
  std::cout << "NODO-> Nodo:Coste\n";
  for (int i = 0; i < ListaSucesores.size(); i++) {
    std::cout << "[" << i + 1 << "] -> ";
    if (ListaSucesores[i].empty()) {
      std::cout << "null";
    }
    for (const auto &Elementos : ListaSucesores[i]) {
      std::cout << "| " << Elementos.j << " :" << std::setw(3) << Elementos.c << " |";
    }
    std::cout << std::endl;
  }
  system("pause");
}
void Grafo::showinfo() {
  std::cout << "Grafo ";
  if (!dirigido)
    std::cout << "no ";
  std::cout << "dirigido | nodos: " << n << " | arcos: " << m << std::endl;
  system("pause");
}
void Grafo::showpredecesores() {
  std::cout << "NODO<- Nodo:Coste\n";
  for (int i = 0; i < ListaPredecesores.size(); i++) {
    std::cout << "[" << i + 1 << "] <- ";
    if (ListaPredecesores[i].empty()) {
      std::cout << "null";
    }
    for (const auto &Elementos : ListaPredecesores[i]) {
      std::cout << "| " << Elementos.j << " :" << std::setw(3) << Elementos.c << " |";
    }
    std::cout << std::endl;
  }
  system("pause");
}


// --------------------------------------------- //