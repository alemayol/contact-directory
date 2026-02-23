#include "../../include/data/GestorJSON.h"
#include "model/ArbolB.h"
#include "model/Contact.h"
#include "model/ContactList.h"
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

bool GestorJSON::guardarArchivo(std::string nombreA, const ArbolB &directorio) {

  nlohmann::json J;
  nlohmann::json jArr = nlohmann::json::array();

  J["recientes"] = directorio.getRecientes();

  directorio.performPreorderAction([&jArr](const Contact contacto) {
    // std::cout << "Guardando email -> " << contacto.getEmail() << std::endl;
    jArr.push_back(contacto);
  });

  J["contactos"] = jArr;

  std::ofstream file(nombreA);

  if (file.is_open()) {
    file << J.dump(4); // El 4 es para identacion en el json
    return true;
  }

  return false;
}

bool GestorJSON::cargarArchivo(std::string nombreA, ArbolB &tree,
                               ContactList &tablaHash) {
  std::ifstream file(nombreA);

  if (!file.is_open()) {
    std::cout << "No ha sido posible cargar los contactos previos" << std::endl;
    return false;
  }

  nlohmann::json j;

  file >> j;

  nlohmann::json arr = nlohmann::json::array();

  arr = j.at("contactos");

  for (auto &c : arr) {
    Contact contacto;
    from_json(c, contacto);
    tree.insertarContacto(contacto);
    std::string key = contacto.getEmail();
    tablaHash.insertarContacto(key, std::move(contacto));
  }

  for (auto &rec : j.at("recientes")) {
    Contact reciente;
    from_json(rec, reciente);
    tree.agregarRecientes(reciente);
  }

  return true;
}
