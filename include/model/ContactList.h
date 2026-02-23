#pragma once

#define CONTACTLIST_H

#include "Contact.h"
#include <cstring>
#include <list>
#include <string>

class ContactList {
private:
  static const int CLASES = 10;
  std::list<std::pair<std::string, Contact>> tabla[CLASES];
  int hashing(std::string key);

public:
  ContactList();
  bool isEmpty() const;
  void insertarContacto(std::string key, Contact contacto);
  Contact *consultarContacto(std::string key);
  void actualizarContacto(std::string key, Contact contacto);
  void eliminarContacto(std::string key);
  void imprimirTabla();
};
