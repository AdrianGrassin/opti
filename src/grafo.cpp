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

  // siempre vamos a crear el vector de LP porque nos sera util en ambos casos

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

  // si el grafo no es dirigido entonces se tomaran los predecesores y se pondran al final de cada lista de 'sucesores'.
  // luego simplemente la borramos y listo.
  if (!dirigido) {
    for (i = 0; i < ListaPredecesores.size(); i++) {
      for (int j = 0; j < ListaPredecesores[i].size(); j++) {
        ListaSucesores[i].push_back(ListaPredecesores[i][j]);
      }
    }
    ListaPredecesores.resize(0);
  }

  // en caso contrario tenemos creada la lista de predecesores y no hace falta hacer nada mas.
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

void Grafo::dfs(unsigned i, std::vector<NodeAdyacence> L, std::vector<bool> &visitado, std::vector<unsigned> &prenum,
                unsigned &prenum_ind, std::vector<unsigned> &postnum, unsigned &postnum_ind) {

  visitado[i] = true;
  prenum.push_back(i + 1);

  for (unsigned j = 0; j < L[i].size(); j++) {
    if (!visitado[L[i][j].j - 1]) {
      dfs(L[i][j].j - 1, L, visitado, prenum, prenum_ind, postnum, postnum_ind);
    }
  }
  postnum.push_back(i + 1);
}

void Grafo::RecorridoProfundidad() {
  unsigned i(0), prenum_ind(0), postnum_ind(0);
  std::vector<bool> visitado(ListaPredecesores.size(), false);
  std::vector<unsigned> prenum;
  std::vector<unsigned> postnum;

  // Si hace falta cambiarlo, aqui se puede hacer un if para poner por que lista iterar.
  std::vector<NodeAdyacence> List(ListaSucesores);


  std::cout << "Introduce el nodo desde donde partira la busqueda [" << " 1 - " << ListaPredecesores.size() << "] :";
  std::cin >> i;

  dfs(i - 1, List, visitado, prenum, prenum_ind, postnum, postnum_ind);

  std::cout << "Orden de visita de los nodos en preorden: \n";
  bool first = true;
  for (const auto &node : prenum) {
    if (first) {
      std::cout << "[" << node << "]";
      first = false;
    } else
      std::cout << " -> [" << node << "]";
  }

  std::cout << std::endl;
  first = true;
  std::cout << "Orden de visita de los nodos en postorden: \n";
  for (const auto &node : postnum) {
    if (first) {
      std::cout << "[" << node << "]";
      first = false;
    } else
      std::cout << " -> [" << node << "]";
  }

  std::cout << std::endl;

  system("pause");

}

void Grafo::bfs( unsigned i, std::vector<NodeAdyacence> L, std::vector<unsigned> &pred, std::vector<unsigned> &dist) {


}

// --------------------------------------------- //