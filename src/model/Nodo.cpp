

#include "model/Contact.h"
class Nodo {
private:
  Contact contacto;
  Nodo *izq;
  Nodo *der;

public:
  Nodo(Contact nodo);
};

Nodo::Nodo(Contact nodo) : contacto("", "", "") {

  this->contacto = nodo;
  this->izq = nullptr;
  this->der = nullptr;
}
