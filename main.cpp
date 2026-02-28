

#include "data/GestorCSV.h"
#include "data/GestorJSON.h"
#include "model/ArbolB.h"
#include "model/Contact.h"
#include "model/ContactList.h"
#include "view/Visualizador.h"
#include <iostream>
#include <memory>

const std::string ARCHIVO = "directorio.json";

int main(int argc, char *argv[]) {

  auto directorio = std::make_unique<ArbolB>();
  auto tablaContactos = std::make_unique<ContactList>();
  Visualizador menu = Visualizador();
  int opcionMenu = -1;

  bool existeJson = GestorJSON::cargarArchivo("directorio.json", *directorio,
                                              *tablaContactos);

  try {
    if (!existeJson) {
      GestorCSV::cargarArchivo("prueba.csv", *directorio, *tablaContactos);
    }

    while (opcionMenu != 0) {
      menu.displayMenu();

      std::cin >> opcionMenu;

      if (std::cin.fail()) {
        std::cin.clear();
        opcionMenu = -1;
        std::cout << "Opción no válida. Por favor, ingrese un número."
                  << std::endl;
        ;

        menu.limpiarBuffer();
        continue;
      }

      menu.limpiarBuffer();

      switch (opcionMenu) {
      case 0:
        break;

      case 1: {
        // CRUD
        menu.crudLoop(*directorio, *tablaContactos);
        break;
      }
      case 2: {
        // Contactos recientes
        std::cout << "\n------- Contactos recientes -------\n" << std::endl;
        menu.mostrarContactos(directorio->getRecientes());
        std::cout << std::endl;
        break;
      }
      case 3: {
        // Mostrar todos los contactos
        directorio->imprimirContactos();
        break;
      }
      case 4: {
        // Exportar CSV
        std::cout << "Exportando directorio de contactos a formato CSV....\n"
                  << std::endl;

        GestorCSV::exportarContactos("directorio.csv", *directorio);

        std::cout << "\nArchivo directorio.csv guardado exitosamente"
                  << std::endl;
        break;
      }
      case 5: {
        int num = directorio->numNodos();
        std::cout << "El arbol tiene " << num << " nodos\n" << std::endl;
        break;
      }
      case 6: {
        int num = directorio->alturaArbol();
        std::cout << "El arbol tiene una altura de " << num << "\n"
                  << std::endl;
        break;
      }

      default:
        opcionMenu = -1;
        break;
      }
    }
  } catch (std::exception &e) {
    std::cout << "Ocurrio un error: " << e.what() << std::endl;
  }

  // delete directorio;
  directorio.reset();

  return 0;
}
