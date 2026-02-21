#include "../../include/data/GestorCSV.h"
#include "model/ArbolB.h"
#include "model/NodoContacto.h"
#include <cstdio>
#include <fstream>
#include <iostream>
#include <memory>

bool GestorCSV::exportarContactos(std::string nombreA,
                                  const ArbolB &directorio) {

  std::ofstream file(nombreA);

  if (!file.is_open()) {
    std::cout << "[ADVERTENCIA] No se pudo abrir el archivo CSV" << std::endl;
    return false;
  }

  file << "Nombre,Telefono,Email\n";

  directorio.performPreorderAction([&file](const Contact &contacto) {
    /*
  std::string nombre = GestorCSV::prepareTextCSV(contacto.getNombre());
  std::string tlf = GestorCSV::prepareTextCSV(contacto.getTelefono());
  std::string email = GestorCSV::prepareTextCSV(contacto.getEmail());
*/
    // file << nombre << "," << tlf << "," << email << "\n";
    file << contacto.getNombre() << "," << contacto.getTelefono() << ","
         << contacto.getEmail() << "\n";
  });
  // inorderWrite(directorio, file);

  file.close();

  return false;
}

/*
void GestorCSV::inorderWrite(const ArbolB &directorio, std::ostream &file) {

  std::string nombre = GestorCSV::prepareTextCSV(raiz->contacto.getNombre());
  std::string tlf = GestorCSV::prepareTextCSV(raiz->contacto.getTelefono());
  std::string email = GestorCSV::prepareTextCSV(raiz->contacto.getEmail());

  file << nombre << "," << tlf << "," << email << "\n";

  inorderWrite(raiz->nodoI, file);
}
*/

std::string GestorCSV::prepareTextCSV(std::string cad) {
  // En caso de que contenga comillas, tenemos que agregar otras adicionales
  size_t pos = cad.find("\"");
  while (pos != std::string::npos) {
    cad.replace(pos, 1, "\"\"");
    pos = cad.find("\"", pos + 2);
  }

  // Colocamos doble comillas a todo
  return "\"" + cad + "\"";
}

bool GestorCSV::cargarArchivo(std::string nombreA) {
  std::ifstream file(nombreA);

  if (!file.is_open())
    return false;

  nlohmann::json j;

  file >> j;

  return true;
}
