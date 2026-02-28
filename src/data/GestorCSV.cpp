#include "../../include/data/GestorCSV.h"
#include "model/ArbolB.h"
#include "model/ContactList.h"
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

bool GestorCSV::cargarArchivo(std::string nombreA, ArbolB &directorio,
                              ContactList &tablaHash) {

  std::ifstream file(nombreA);

  if (!file.is_open()) {
    std::cout << "No ha sido posible cargar el archivo con formato CSV"
              << std::endl;
    return false;
  }

  std::string
      linea; // Para guardar cada linea del archivo eje: nombre, tlf, email\n

  std::getline(file, linea); // Obtenemos la primera linea pero no la utilizamos
                             // ya que es la cabecera

  while (std::getline(file, linea)) {

    if (linea.empty())
      continue;

    std::stringstream tempLinea(linea);
    std::string nombre, tlf, email;

    if (std::getline(tempLinea, nombre, ',')) {

      if (std::getline(tempLinea, tlf, ',')) {

        if (std::getline(tempLinea, email)) {
          if (!email.empty() || email.back() == '\r') {
            email.pop_back(); // Por si hay un salto de linea proveniente de
                              // Windows
          }

          // std::cout << nombre << " " << tlf << " " << email << std::endl;
          Contact contacto = {nombre, tlf, email};
          directorio.insertarContacto(contacto);
          tablaHash.insertarContacto(email, std::move(contacto));
        }
      }
    }
  }

  file.close();

  return true;
}
