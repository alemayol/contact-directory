#pragma once
#include "model/ArbolB.h"
#include "model/Contact.h"
#define VISUALIZADOR_H

#include <string>
#include <vector>
class Visualizador {
public:
  Visualizador();
  void displayMenu();
  Contact obtenerContacto();
  Contact actualizarContacto(Contact contacto);
  void mostrarContactos(std::vector<Contact> contactos);
  std::string buscarPalabraClave();
  void limpiarBuffer() const;
  void crudLoop(ArbolB &directorio);
};
