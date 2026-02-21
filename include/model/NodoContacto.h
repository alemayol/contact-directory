#pragma once

#define NODOCONTACTO_H
#include "model/Contact.h"
#include <memory>

class NodoContacto {
public:
  NodoContacto(Contact contacto)
      : contacto(contacto), nodoI(nullptr), nodoD(nullptr) {}

  std::unique_ptr<NodoContacto> nodoI, nodoD;
  Contact contacto;
};
