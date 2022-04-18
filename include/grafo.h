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

typedef std::vector<ElementoLista> NodeAdyacence;  // vector de nodos adyacentes multiproposito

class Grafo {
 private:
  bool dirigido;   // 0 -> no dirigido
  std::string path;

  unsigned int n;  // numero de nodos
  unsigned int m;  // numero de arcos

  std::vector<NodeAdyacence> ListaSucesores;      // Adyacencia de sucesores.
  std::vector<NodeAdyacence> ListaPredecesores;   // Adyacencia de predecesores.
  std::vector<NodeAdyacence> MatrizAdyacencia;    // Adyacencia y costes.

  void destroy();
  void build(const std::ifstream& file, const std::string& pathname);

 public:
  explicit Grafo(const std::string& filepath);

  // getters
  bool esdirigido() const;
  void actualizargrafo(std::string& nuevografo);
  const std::string &pathto();
};
#endif //OPTI_GRAFO_H
