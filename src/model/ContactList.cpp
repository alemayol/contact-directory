#include "model/ContactList.h"
#include "model/Contact.h"
#include <iostream>
#include <iterator>
#include <string>

bool ContactList::isEmpty() const {

  int suma{};

  for (int i{}; i < CLASES; i++) {
    suma += tabla[i].size();
  }

  if (!suma)
    return true;

  return false;
};

int ContactList::hashing(std::string key) {

  int suma{};

  for (int letra{}; letra < key.length(); letra++) {

    suma += key[letra] * letra;
  }

  return suma % CLASES;
}

void ContactList::insertarContacto(std::string key, Contact contacto) {

  int hashValue = hashing(key);

  // std::list<std::pair<std::string, Contact>> &cell = tabla[hashValue];
  auto &listaActual = tabla[hashValue];
  auto itrNodo = std::begin(listaActual);
  bool keyExists = false; // Asumimos que la llave no existe

  for (; itrNodo != std::end(listaActual); itrNodo++) {

    // Si la llave existe, reemplazamos el valor, no queremos llaves duplicadas
    // (podriamos ofrecer una funcion distinta para esta funcionalidad)
    if (itrNodo->first == key) {
      keyExists = true;
      itrNodo->second = contacto;
      std::cout
          << "[ADVERTENCIA] La llave existe. El contacto ha sido actualizado"
          << std::endl;
      break;
    }
  }

  if (!keyExists) {
    tabla->emplace_back(key, contacto);
  }

  return;
}

void ContactList::eliminarContacto(std::string key) {
  int hashValue = hashing(key);

  auto &listaActual = tabla[hashValue];
  auto itrNodo = std::begin(listaActual);
  bool keyExists = false; // Asumimos que la llave no existe

  for (; itrNodo != std::end(listaActual); itrNodo++) {

    // Si la llave existe, reemplazamos el valor, no queremos llaves duplicadas
    // (podriamos ofrecer una funcion distinta para esta funcionalidad)
    if (itrNodo->first == key) {
      keyExists = true;
      itrNodo = listaActual.erase(
          itrNodo); // Retorna un iterador al siguiente elemento, lo asignamos
                    // para evitar errores pero no se utilizara
      std::cout << "[INFO] La llave existe. El contacto ha sido eliminado"
                << std::endl;

      break;
    }
  }

  if (!keyExists) {

    std::cout
        << "[ADVERTENCIA] La llave no existe. Ningun contacto fue eliminado"
        << std::endl;
  }

  return;
}

void ContactList::imprimirTabla() {

  for (int i{}; i < CLASES; i++) {

    if (tabla[i].size() == 0)
      continue; // Lista vacia

    auto itrNodo = tabla[i].begin();

    for (; itrNodo != tabla[i].end(); itrNodo++) {
      std::cout << "[INFO] Key: " << itrNodo->first << std::endl;
      itrNodo->second.imprimirInfo();
    }
  }

  return;
}
