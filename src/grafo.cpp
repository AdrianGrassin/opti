//
// Created by grass on 18/04/2022.
//
#include <fstream>
#include <iostream>
#include <iomanip>
#include <queue>
#include <stack>
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
  std::vector<bool> visitado(ListaSucesores.size(), false);
  std::vector<unsigned> prenum;
  std::vector<unsigned> postnum;

  // Si hace falta cambiarlo, aqui se puede hacer un if para poner por que lista iterar.
  std::vector<NodeAdyacence> List(ListaSucesores);

  std::cout << "Introduce el nodo desde donde partira la busqueda [" << " 1 - " << ListaSucesores.size() << "] :";
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

void Grafo::bfs(int i, std::vector<NodeAdyacence> L, std::vector<int> &dist, std::vector<int> &pred) {
  std::vector<bool> visitado(n, false);
  visitado[i - 1] = true;

  std::queue<unsigned> cola;
  cola.push(i - 1);

  unsigned distancia = 1;
  unsigned pisoactual(1), pisosig(0);

  while (!cola.empty()) {
    unsigned k = cola.front();
    cola.pop();

    for (auto &j : L[k]) {
      if (!visitado[j.j - 1]) {
        visitado[j.j - 1] = true;
        cola.push(j.j - 1);
        pred[j.j - 1] = k;
        dist[j.j - 1] = distancia;
        pisosig++;
      }
    }

    pisoactual--;
    if (pisoactual == 0) {
      distancia++;
      pisoactual = pisosig;
      pisosig = 0;
    }
  }
}

void Grafo::RecorridoAmplitud() {
  int i;
  std::vector<int> dist(n, -1);
  std::vector<int> pred(n, -1);

  // Si hace falta cambiarlo, aqui se puede hacer un if para poner por que lista iterar.
  std::vector<NodeAdyacence> List(ListaSucesores);

  std::cout << "Introduce el nodo desde donde partira la busqueda [" << " 1 - " << ListaSucesores.size() << "] :";
  std::cin >> i;

  int initialnode(i - 1);

  dist[i - 1] = 0;

  bfs(i, List, dist, pred);

  int maxdist(0);
  // bucle para encontrar la mayor distancia
  for (i = 0; i < n; i++) {
    if (dist[i] > maxdist)
      maxdist = dist[i];
  }

  std::cout << "Distancia entre nodos: \n";
  for (i = 0; i <= maxdist; i++) {
    std::cout << "[" << i << "] :";
    for (int j = 0; j < n; j++) {
      if (dist[j] == i) {
        std::cout << "| " << j + 1 << " | ";
      }
    }
    std::cout << std::endl;
  }

  std::cout << "\nRamas para cada nodo:\n";
  for (i = 0; i < n; i++) {
    int element = i;
    std::stack<int> pila;
    std::cout << "nodo [" << i + 1 << "] :";

    while (element != initialnode && pred[element] != -1) {
      pila.push(element);
      element = pred[element];
    }

    if (pila.empty()) {
      std::cout << "no conecta \n";
    } else {
      pila.push((int)initialnode);
      while (pila.size() > 1) {
        std::cout << pila.top() + 1 << " <- ";
        pila.pop();
      }
      std::cout << pila.top() + 1 << " \n";
      pila.pop();
    }
  }

  system("pause");

}


void Grafo::kruskal() {
  std::vector<AristaPesada> Aristas;

  //Cargamos todas las aristas de la lista de adyacencia en el vector auxiliar aristas


  Aristas.resize(m);

  unsigned k = 0;
  for (unsigned i = 0; i < n; i++) {
    for (unsigned j = 0; j < ListaSucesores[i].size(); j++) {
      if (i < ListaSucesores[i][j].j) {
        Aristas[k].extremo1 = i + 1;
        Aristas[k].extremo2 = ListaSucesores[i][j].j;
        Aristas[k++].peso = ListaSucesores[i][j].c;
      }
    }
  }

// esto es para el caso en el que también se quiera hacer en grafos dirigidos pero sin tomar en cuenta que son dirigidos
  if (dirigido) {
    for (unsigned i = 0; i < n; i++) {
      for (unsigned j = 0; j < ListaPredecesores[i].size(); j++) {
        if (i < ListaPredecesores[i][j].j) {
          Aristas[k].extremo1 = i + 1;
          Aristas[k].extremo2 = ListaPredecesores[i][j].j;
          Aristas[k++].peso = ListaPredecesores[i][j].c;
        }
      }
    }
  }
// ---------------------------------------------------------------------------------------------------------------------

  AristaPesada aux;  // Esto es un buffer para hacer un algoritmo de ordenación por intercambio

  for (unsigned l = 0; l < Aristas.size(); l++) {
    for (unsigned x = l + 1; x < Aristas.size(); x++) {
      if (Aristas[l].peso > Aristas[x].peso) {
        aux = Aristas[l];
        Aristas[l] = Aristas[x];
        Aristas[x] = aux;

      }
    }
  }

  int pesoMST = 0; // Inicializamos el peso de la solución
  int br = 0;      // Break


  std::vector<bool> comp; // Vector de booleanos para comprobar una serie de condiciones
  comp.resize(4, false);

  // comp[0] = comprobación de que el nodo del primer extremo ya se haya usado
  // comp[1] = comprobación de que el nodo del segundo extremo ya se haya usado
  // comp[2] = comprobación de que la arista a añadir no crea un bucle
  // comp[3] = comprobación de que el primer extremo de la arista no coincide con un elemento sye
  // comp[4] = comprobación de que el segundo extremo no coincide con el siguiente elemento de sye


  int q = 1;                      // Numero de aristas visitado
  std::vector<unsigned> sye;      // Vector salidas y entradas

  std::vector<unsigned> repe;     // Vector para comprobación de bucles
  repe.resize(n);
  int rh = 0;                     //repe head//

  std::vector<unsigned> ady1;     // Vector que utilizaré para comprobar las adyacencias del extremo

  do {
    /* Implementamos el algoritmo de Kruskal */

    for (auto &Arista : Aristas) {    // bucle principal que recorre cada arista
      for (unsigned int& a : repe) {   // se recorre repe para comprobar que la arista
        if (Arista.extremo1 == a) {
          comp[0] = true;
          break;
        }
      }

      if (!comp[0]) {
        repe[rh] = Arista.extremo1;
        rh++;
      } // si el primer extremo de la arista no está en el vector repe, se añade en la cabeza de repe y se sube rh de 1

      sye.push_back(Arista.extremo1); // añadimos el primer

      for (unsigned int& b : repe) {
        if (Arista.extremo2 == b) {
          comp[1] = true;
          break;
        }
      }

      if (!comp[1]) {
        repe[rh] = Arista.extremo2;
        rh++;
      }

      sye.push_back(Arista.extremo2);

      if (comp[0] && comp[1]) {                         // si ambos han sido "activados" se tendrán que hacer otras
        // comprobaciones.
        for (unsigned f = 0; f < sye.size() - 2; f++) { // en este bucle se comprueba que los extremos no correspondan
          // a ningún elemento dentro del vector repe

          if (Arista.extremo1 == sye[f]) {              //  si el primer nodo corresponde a un elemento del vector sye
            comp[3] = true;                             //  se activa comp3,
            if (Arista.extremo2 == sye[f + 1])          //  en ese caso, se comprueba que el siguiente elemento de sye
              comp[4] = true;                           //  y en caso de que se corresponda comp4 se activa
          }
          if (comp[3] && comp[4])                       //  si se ha cumplido en algún momento la condición de arriba
            comp[2] = true;                             //  se activa comp2 lo que indica que si insertasemos la arista
        }                                               //  como solución se forma un bucle. Más tarde comprobaremos
        //  comp2 para descartar o no la arista.

        ady1.push_back(Arista.extremo1);                //  se hace push back en ady1
        unsigned o = 0;                                 //  instanciamos o que es

        for (unsigned f = 0; f < sye.size() - 2; f++) {  // este bucle hará una lista de adyacencia

          if (ady1[o] == sye[f]) {                      //

            if ((f + 1) % 2 == 0) {                     // si f + 1 es par se hace push back de sye f - 1
              ady1.push_back(sye[(f - 1)]);
            } else {
              ady1.push_back(sye[(f + 1)]);             // en caso distinto se hace pushback de sye f + 1
            }

            if (ady1.back() == Arista.extremo2) {       // si el nodo insertado es igual al extremo2 tendremos un bucle
              comp[2] = true;                           // y quitaremos de la lista de salidas y entradas los extremos
              sye.pop_back();                           // de la arista introducida
              sye.pop_back();
              br = 1;                                   // se activa el break que va a salir de este procedimiento y
            }                                           // pasar al siguiente nodo

            if (br == 1)
              break;

            for (unsigned v = 0; v < ady1.size() - 1; v++) {  // se hace popback en ady
              if (ady1.back() == ady1[v])
                ady1.pop_back();
            }
          }
          // aquí se hacen comprobaciones
          if (br == 1) { break; }
          if (o == ady1.size() && f == sye.size() - 3) { break; }
          if (f == sye.size() - 3) {
            f = -1;
            o++;
          }
        }

        if (!comp[2]) {
          std::cout << "Arista numero " << q << " incorporada (" << Arista.extremo1 << ", " << Arista.extremo2
                    << "), con peso " << Arista.peso << std::endl;
          pesoMST += Arista.peso;
          q++;
          break;
        }
        q++;
      } else {
        std::cout << "Arista numero " << q << " incorporada (" << Arista.extremo1 << ", " << Arista.extremo2
                  << "), con peso " << Arista.peso << std::endl;
        pesoMST += Arista.peso;
        q++;
      }

      std::fill(comp.begin(), comp.end(), false);
      ady1.clear();
      br = 0;

    }

  } while (q < n && q <= m);

  if (q == n) {
    std::cout << "El peso del arbol generador de minimo coste es " << pesoMST << std::endl;
  } else {
    std::cout << "El grafo no es conexo, y el bosque generador de minimo coste tiene peso " << pesoMST << std::endl;
  }

  system("pause");
}
// --------------------------------------------- //
