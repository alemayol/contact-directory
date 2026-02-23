#include "../../include/view/Visualizador.h"
#include "data/GestorCSV.h"
#include "data/GestorJSON.h"
#include "model/ArbolB.h"
#include "model/Contact.h"
#include "model/ContactList.h"
#include <ios>
#include <iostream>
#include <limits>
#include <vector>

Visualizador::Visualizador() {}

void Visualizador::limpiarBuffer() const {

  std::cin.ignore(std::numeric_limits<std::streamsize>::max(),
                  '\n'); // Limpiando buffer
}

void Visualizador::displayMenu() {
  std::cout << "\n--- Directorio de Contactos ---" << std::endl;
  std::cout << "1. Operaciones de contactos (CRUD)" << std::endl;
  std::cout << "2. Contactos recientes" << std::endl;
  std::cout << "3. Mostrar todos los contactos" << std::endl;
  std::cout << "4. Exportar contactos a archivo CSV" << std::endl;
  std::cout << "0. Salir" << std::endl;
  std::cout << "Seleccione una opción: ";
}

Contact Visualizador::obtenerContacto() {
  std::string nombre;
  std::string tlf;
  std::string email;

  std::cout << "Nombre: ";
  std::cin >> nombre;

  if (std::cin.fail()) {
    std::cin.clear();
    std::cout << "Por favor, ingrese un nombre valido" << std::endl;
    throw new std::exception();
  }

  std::cout << "Telefono: ";
  std::cin >> tlf;

  if (std::cin.fail()) {
    std::cin.clear();
    std::cout << "Por favor, ingrese un telefono valido" << std::endl;
    throw new std::exception();
  }

  std::cout << "Email: ";
  std::cin >> email;

  if (std::cin.fail()) {
    std::cin.clear();
    std::cout << "Por favor, ingrese un email valido" << std::endl;
    throw new std::exception();
  }

  Contact contacto = {nombre, tlf, email};

  return contacto;
}

Contact Visualizador::actualizarContacto(Contact contacto) {
  std::string nombre;
  std::string tlf;
  std::string email;

  std::cout << "Nombre Previo -> " << contacto.getNombre() << std::endl;
  std::cout << "Nombre: ";
  std::cin >> nombre;

  if (std::cin.fail()) {
    std::cin.clear();
    std::cout << "Por favor, ingrese un nombre valido" << std::endl;
    throw new std::exception();
  }

  std::cout << "Telefono Previo -> " << contacto.getTelefono() << std::endl;
  std::cout << "Telefono: ";
  std::cin >> tlf;

  if (std::cin.fail()) {
    std::cin.clear();
    std::cout << "Por favor, ingrese un telefono valido" << std::endl;
    throw new std::exception();
  }

  std::cout << "Email Previo -> " << contacto.getEmail() << std::endl;
  std::cout << "Email: ";
  std::cin >> email;

  if (std::cin.fail()) {
    std::cin.clear();
    std::cout << "Por favor, ingrese un email valido" << std::endl;
    throw new std::exception();
  }

  contacto = {nombre, tlf, email};

  return contacto;
}

void Visualizador::mostrarContactos(std::vector<Contact> contactos) {

  printf("| %3s Nombre %3s| %3s Telefono %3s | %8s Email %9s|\n", "", "", "",
         "", "", "");

  for (int i{}; i < contactos.size(); i++) {
    printf("| %-13s | ", contactos[i].getNombre().c_str());
    printf(" %-15s | ", contactos[i].getTelefono().c_str());
    printf(" %-22s |\n", contactos[i].getEmail().c_str());
  }
}

void Visualizador::crudLoop(ArbolB &directorio, ContactList &tabla) {
  int opcion = -1;

  while (opcion != 0) {

    std::cout << "\n--- Operaciones sobre Contactos ---" << std::endl;
    std::cout << "1. Agregar contacto" << std::endl;
    std::cout << "2. Actualizar contacto" << std::endl;
    std::cout << "3. Consultar contacto" << std::endl;
    std::cout << "4. Eliminar contacto" << std::endl;
    std::cout << "0. Volver" << std::endl;
    std::cout << "Seleccione una opción: ";

    std::cin >> opcion;

    if (std::cin.fail()) {
      std::cin.clear();
      opcion = -1;
      std::cout << "Opción no válida. Por favor, ingrese un número."
                << std::endl;
      ;

      this->limpiarBuffer();
      continue;
    }

    this->limpiarBuffer();
    try {
      switch (opcion) {
      case 0:
        break;

      case 1: {
        // Agregar contacto
        Contact contacto = this->obtenerContacto();

        directorio.insertarContacto(contacto);
        tabla.insertarContacto(contacto.getEmail(), contacto);
        GestorJSON::guardarArchivo("directorio.json", directorio);

        std::cout << "Contacto agregado!" << std::endl;
        break;
      }
      case 2: {
        // Actualizar contacto
        std::cout << "Nombre del contacto: ";
        std::string nombre;
        std::cin >> nombre;
        Contact *contacto = directorio.existeContacto(nombre);
        if (!contacto) {
          std::cout << "El contacto no existe" << std::endl;
          break;
        };

        std::cout << "\n------- Ingrese los nuevos datos -------\n"
                  << std::endl;
        Contact nuevoContacto = this->actualizarContacto(*contacto);

        directorio.actualizarContacto(nombre, nuevoContacto);
        tabla.actualizarContacto(contacto->getEmail(), nuevoContacto);
        GestorJSON::guardarArchivo("directorio.json", directorio);

        break;
      }
      case 3: {
        // Consultar contacto

        int eleccion = 0;
        std::cout << "Criterio de busqueda: " << std::endl;
        std::cout << "1. Nombre" << std::endl;
        std::cout << "2. Email" << std::endl;
        std::cout << "Opcion: ";
        std::cin >> eleccion;

        if (eleccion > 2 || eleccion < 1) {
          std::cout << "Criterio no especificado. Elija una opcion en pantalla"
                    << std::endl;
          break;
        }

        if (eleccion == 1) {
          std::cout << "Nombre del contacto: ";
          std::string nombre;
          std::cin >> nombre;

          if (std::cin.fail()) {
            std::cin.clear();
            opcion = -1;
            std::cout << "Nombre invalido" << std::endl;

            this->limpiarBuffer();
            break;
          }

          std::vector<Contact> encontrados =
              directorio.consultarContacto(nombre);

          if (encontrados.size() == 0) {
            std::cout << "No se encontraron contactos que coincidan con el "
                         "nombre ingresado"
                      << std::endl;
            break;
          }

          this->mostrarContactos(encontrados);

        } else if (eleccion == 2) {
          std::cout << "Email del contacto: ";
          std::string email;
          std::cin >> email;

          if (std::cin.fail()) {
            std::cin.clear();
            opcion = -1;
            std::cout << "Email invalido" << std::endl;

            this->limpiarBuffer();
            break;
          }

          Contact *contacto = tabla.consultarContacto(email);

          if (contacto == nullptr) {
            std::cout << "Contacto no encontrado" << std::endl;
            break;
          } else {

            printf("| %3s Nombre %3s| %3s Telefono %3s | %8s Email %9s|\n", "",
                   "", "", "", "", "");

            printf("| %-13s | ", contacto->getNombre().c_str());
            printf(" %-15s | ", contacto->getTelefono().c_str());
            printf(" %-22s |\n", contacto->getEmail().c_str());
          }
        }

        break;
      }
      case 4: {
        // Eliminar contacto

        std::cout << "Nombre del contacto: ";
        std::string nombre;
        std::cin >> nombre;

        if (!directorio.eliminarContacto(nombre)) {
          std::cout << "[INFO] No se encontro ningun contacto. Ningun contacto "
                       "ha sido eliminado."
                    << std::endl;
        } else {
          std::cout << "Contacto " << nombre << " eliminado" << std::endl;
        }
        GestorJSON::guardarArchivo("directorio.json", directorio);

        std::cout << std::endl;

        break;
      }
      default:
        opcion = -1;
        break;
      }
    } catch (std::exception &e) {
      std::cout << "Ocurrio un error" << std::endl;
    }
  }
}
