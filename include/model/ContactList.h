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

public:
  ContactList();
  bool isEmpty() const;
  int hashing(std::string key);
  void insertarContacto(std::string key, Contact contacto);
  Contact consultarContacto(std::string key);
  void eliminarContacto(std::string key);
  void imprimirTabla();
};
