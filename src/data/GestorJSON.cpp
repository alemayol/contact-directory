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

  directorio.performPreorderAction(
      [&jArr](const Contact contacto) { jArr.push_back(contacto); });

  J["contactos"] = jArr;

  std::ofstream file(nombreA);

  try {
    if (file.is_open()) {
      file << J.dump(
          4, ' ', false,
          nlohmann::json::error_handler_t::replace); // El 4 es para identacion
                                                     // en el json
      return true;
    }
  } catch (nlohmann::json::type_error &e) {
    std::cerr << "No se pudo codificar el archivo: " << e.what() << std::endl;
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

  std::cout << "Cargando Archivos..." << std::endl;

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
