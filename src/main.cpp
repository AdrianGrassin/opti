/**
 * UNIVERSIDAD DE LA LAGUNA
 * Date: 18-04-2022
 * Author: Adrian Grassin Luis
 * Brief: PRACTICA 1 DE GRAFOS desde 0 basada en la practica original
 *
 * Especificaciones: Programa uniplataforma -> WINDOWS (requiere de la librería windows.h)
 *
 * To do: MENU // AÑADIR FUNCIONES GRAFO
 *       CONSTRUCTOR GRAFO
 *       LOGICA DEL GRAFO
 *
 */


#include <iostream>
#include "utilities.cpp"
#include "../include/grafo.h"

#pragma execution_character_set( "utf-8" )

int main(int argc, char **argv) {
  SetConsoleOutputCP( 65001 );
//typewrite("Grafos 2022: Adrian Grassin -> Practica 1");
  Sleep(500);

  std::string pathtofile;
  char opciones;

  // argument handling
  if (argc > 1) {
    typewrite("Argumentos detectados: Procediendo a cargar datos...");
    pathtofile = argv[1];
    system("CLS");
  } else {
    system("CLS");
    std::cout << "Introduce el fichero correspondiente al grafo:";
    std::cin >> pathtofile;
  }

  try {
    Grafo G(pathtofile);
    while(opciones != 'q'){
      menu(G.esdirigido(), opciones, G.pathto());
      switch(opciones){
        case 'c':
          system("CLS");
          std::cout << "Introduce la ruta al nuevo Grafo: ";
          std::cin >> pathtofile;
          G.actualizargrafo(pathtofile);

          break;
        case 'i':
          G.showinfo();
          break;
        case 'a':
          if(G.esdirigido()){
            std::cout << "No soportado para este tipo de grafo.";
          } else {
            G.showadyacencia();
          }
          break;
        case 's':
          if(G.esdirigido()){
            G.showadyacencia();
          } else {
            std::cout << "No soportado para este tipo de grafo.";
          }
          break;
        case 'p':
          if(G.esdirigido()){
          G.showpredecesores();
          } else {
            std::cout << "No soportado para este tipo de grafo.";
          }
          break;
        case 'r':
          G.RecorridoProfundidad();
          break;
        case 'm':
          G.RecorridoAmplitud();
          break;
        case 'k':
          G.kruskal();
          break;
        case 'q':
          typewrite("Saliendo del programa...");
          break;
        default:
          std::cout << "La opción introducida no es valida, vuelve a intentarlo";
          Sleep(500);
          break;
      }
    }

  }
  catch (std::invalid_argument &e) {
    system("CLS");
    std::cout << e.what();
    typewrite("Saliendo del programa...");
    Sleep(500);
    return -1;
  }



}
