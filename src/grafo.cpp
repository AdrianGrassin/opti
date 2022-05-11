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

  while (!cola.empty()) {                   // creo una cola en la que voy a ir metiendo cada piso
    unsigned k = cola.front();              // cada vez que se ejecute el while se van a ir obteniendo los hijos y
    cola.pop();                             // poniendo al final de la cola, además con las variables de pisoactual
    // y pisosig sabremos cuantos elementos hay por piso y así ir explorando
    for (auto &j : L[k]) {                  // el árbol de nodos.
      if (!visitado[j.j - 1]) {
        visitado[j.j - 1] = true;
        cola.push(j.j - 1);
        pred[j.j - 1] = k;
        dist[j.j - 1] = distancia;
        pisosig++;
      }
    }

    pisoactual--;
    if (pisoactual == 0) {                  // si no quedan nodos del piso actual se incrementa la distancia y el
      distancia++;                          // contador de elementos en el piso se cambia por los del piso siguiente
      pisoactual = pisosig;                 // y el piso siguiente se pone a 0 ya que aún no se ha explorado
      pisosig = 0;                          // y así hasta terminar de encontrar los nodos en el grafo
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
      pila.push((int) initialnode);
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
  std::vector<unsigned> sye;      // vector en el que se encuentran los nodos del camino + la arista a insertar

  std::vector<unsigned> repe;     // Lista de adyacencia de los nodos del camino minimo
  repe.resize(n);
  int rh = 0;                     //repe head//

  std::vector<unsigned> ady1;     // Lista de adyacencia de un posible camino no conexo

  do {
    /* Implementamos el algoritmo de Kruskal */

    for (auto &Arista : Aristas) {    // bucle principal que recorre cada arista
      for (unsigned int &a : repe) {      // se recorre repe para comprobar que la arista
        if (Arista.extremo1 == a) {
          comp[0] = true;
          break;
        }
      }

      if (!comp[0]) {
        repe[rh] = Arista.extremo1;
        rh++;
      } // si el primer extremo de la arista no está en el vector repe, se añade en la cabeza de repe y se sube rh de 1

      sye.push_back(Arista.extremo1); // añadimos el primer extremo

      for (unsigned int &b : repe) {
        if (Arista.extremo2 == b) {
          comp[1] = true;
          break;
        }
      }

      if (!comp[1]) {
        repe[rh] = Arista.extremo2;
        rh++;
      }

      sye.push_back(Arista.extremo2); // añadimos el segundo extremo

      /**
       * @brief :
       *
       * 1)  si ambos nodos están en la lista de adyacencia del camino habrá que comprobar si no crean un bucle mínimo
       *     en este bucle se comprueba que ambos extremos correspondan a la vez, en ese caso se descarta la arista.
       *
       * 2)  si se ha cumplido en algún momento la condición de arriba se activa comp2 lo que indica que si insertásemos
       *     la arista como solución se forma un bucle. Más tarde comprobaremos comp2 para descartar o no la arista.
       *
       * 3)  Empezamos la lista de adyacencia del primer extremo haciendo un push back del mismo
       *
       * 4)  Este bucle hará una lista de adyacencia ignorando los dos últimos elementos introducidos
       *
       * 5)  La lógica tras las siguientes comparaciones posicionales con el vector de sye es que:
       *         - En las posiciones pares están los extremos1(inicio).
       *         - En las posiciones impares están los extremos2 (destino).
       *     Es así que obtenemos la adyacencia que conforma un camino desde el extremo1.
       *     Si en algún momento el nuevo nodo que hemos insertado equivale al extremo2 quiere decir que hay un bucle
       *     y que podemos desechar la arista con lo que pasamos al paso 6)
       *
       * 6)  Popeamos de la lista los elementos que estábamos comprobando y activamos el break.
       *
       * 7)  Limpiamos el vector de adyacencia en caso de haber metido un elemento repetido en el final.
       *
       * 8)  Se comprueba si hay que seguir haciendo comprobaciones
       *
       * 9)  Si se ha llegado al final de ady y al final de sye (ignorando los elementos introducidos) hemos llegado al
       *     final y se puede salir. ( normalmente en este caso la arista ha sido aceptada ).
       *
       * 10) Cuando se llega al final de sye simplemente se pasa al siguiente elemento y se reincia f.
       *
       * -|El resto ya es imprimir la arista en el caso de que las comprobaciones hayan resultado correctas|-
       */

      if (comp[0] && comp[1]) {                                         /*  1)  */

        for (unsigned f = 0; f < sye.size() - 2; f++) {
          if (Arista.extremo1 == sye[f]) {
            comp[3] = true;
            if (Arista.extremo2 == sye[f + 1])
              comp[4] = true;
          }

          if (comp[3] && comp[4])                                       /*  2)  */
            comp[2] = true;
        }

        ady1.push_back(Arista.extremo1);                                /*  3)  */
        unsigned o = 0;

        for (unsigned f = 0; f < sye.size() - 2; f++) {                 /*  4)  */
          if (ady1[o] == sye[f]) {                                      /*  5)  */
            if ((f + 1) % 2 == 0) {
              ady1.push_back(sye[(f - 1)]);
            } else {
              ady1.push_back(sye[(f + 1)]);
            }

            if (ady1.back() == Arista.extremo2) {                       /*  6)  */
              comp[2] = true;
              sye.pop_back();
              sye.pop_back();
              br = 1;
            }

            if (br == 1)
              break;

            for (unsigned v = 0; v < ady1.size() - 1; v++) {            /*  7)  */
              if (ady1.back() == ady1[v])
                ady1.pop_back();
            }
          }
          if (br == 1) break;                                           /*  8)  */
          if (o == ady1.size() && f == sye.size() - 3) break;           /*  9)  */
          if (f == sye.size() - 3) {                                    /*  10) */
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

void Grafo::kruskal2() {
  unsigned q = 0;
  int pesoMST = 0;
  std::vector<AristaPesada> Aristas;
  Aristas.resize(m);
// creamos el vec
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
// creamos el vector de componentes conexas
  std::vector<unsigned> Raiz;
  Raiz.resize(n);
  for (unsigned q = 0; q < n; q++) {
    Raiz[q] = q;
  }

// ordenamos el vec y aplicamos el criterio de seleccion
  AristaPesada aux;  // buff
  for (unsigned l = 0; l < Aristas.size(); l++) {
    for (unsigned x = l + 1; x < Aristas.size(); x++) {
      if (Aristas[l].peso > Aristas[x].peso) {
        aux = Aristas[l];
        Aristas[l] = Aristas[x];
        Aristas[x] = aux;
      }
    }

    if (Raiz[Aristas[l].extremo1 - 1] != Raiz[Aristas[l].extremo2 - 1]) {
      unsigned cc_extremo2 = Raiz[Aristas[l].extremo2 - 1];
      unsigned cc_extremo1 = Raiz[Aristas[l].extremo1 - 1];
      for (auto &Arista : Raiz) {
        if (Arista == cc_extremo2) {
          Arista = cc_extremo1;
        }
      }
      std::cout << "Arista numero " << l << " incorporada (" << Aristas[l].extremo1 << ", " << Aristas[l].extremo2
                << "), con peso " << Aristas[l].peso << std::endl;
      pesoMST += Aristas[l].peso;
      q++;
    }
  }

  if (q == n - 1) {
    std::cout << "El peso del arbol generador de minimo coste es " << pesoMST << std::endl;
  } else {
    std::cout << "El grafo no es conexo, y el bosque generador de minimo coste tiene peso " << pesoMST << std::endl;
  }

  system("pause");
}
// --------------------------------------------- //
