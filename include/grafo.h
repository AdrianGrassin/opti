//
// Created by grass on 18/04/2022.
//

#ifndef OPTI_GRAFO_H
#define OPTI_GRAFO_H

#include <vector>

typedef struct {
  unsigned j; // nodo
  int c;      // peso
} ElementoLista;

typedef std::vector<ElementoLista> NodeAdyacence;

class Grafo {
 private:
  bool dirigido;   // 0 -> no dirigido

  unsigned int n;  // numero de nodos
  unsigned int m;  // numero de arcos

  std::vector<NodeAdyacence> ListaSucesores;      // Adyacencia de sucesores.
  std::vector<NodeAdyacence> ListaPredecesores;   // Adyacencia de predecesores.
  std::vector<NodeAdyacence> MatrizAdyacencia;    // Adyacencia y costes.

  void destroy();
  void build(const std::ifstream& file);

 public:
  explicit Grafo(const std::string& filepath);

  // getter
  bool esdirigido() const;
  void actualizargrafo(const std::string& basic_string_1);
};
#endif //OPTI_GRAFO_H
