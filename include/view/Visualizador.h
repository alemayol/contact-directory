#pragma once
#include "model/ArbolB.h"
#include "model/Contact.h"
#include "model/ContactList.h"
#define VISUALIZADOR_H

#include <vector>
class Visualizador {
public:
  Visualizador();
  void displayMenu();
  Contact obtenerContacto();
  Contact actualizarContacto(Contact contacto);
  void mostrarContactos(std::vector<Contact> contactos);
  void limpiarBuffer() const;
  void crudLoop(ArbolB &directorio, ContactList &tabla);
};
