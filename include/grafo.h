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

  unsigned n;  // numero de nodos
  unsigned m;  // numero de arcos

  std::vector<NodeAdyacence> ListaSucesores;      // Adyacencia de sucesores o lista de adyacencia.
  std::vector<NodeAdyacence> ListaPredecesores;   // Adyacencia de predecesores.
  std::vector<NodeAdyacence> MatrizAdyacencia;    // Adyacencia y costes.

  void destroy();
  void build(std::ifstream& file, const std::string& pathname);

 public:
  //Constructores y modificadores de la clase
  explicit Grafo(const std::string& filepath);
  void actualizargrafo(std::string& nuevografo);

  // getters
  bool esdirigido() const;
  const std::string &pathto();

  // funciones del menu
  void showadyacencia();
  void showinfo();

};
#endif //OPTI_GRAFO_H
