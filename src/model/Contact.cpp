#include "model/Contact.h"
#include <exception>
#include <iostream>
#include <nlohmann/json.hpp>
#include <regex>
#include <string>

Contact::Contact() {}

Contact::Contact(std::string nombre, std::string telefono, std::string email)
    : expReg("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+") {

  if (!(nombre.length() && telefono.length() && email.length()))
    throw std::exception();

  for (int letra{}; letra < nombre.length(); letra++) {

    // Para el caso del nombre
    if ((nombre[letra] >= 0 && nombre[letra] <= 31) || nombre[letra] == 127) {
      std::cout << "Mal nombre" << std::endl;
      throw std::exception();
    }
  }

  for (int num{}; num < telefono.length(); num++) {

    // Para el caso del telefono
    if (!(telefono[num] >= 48 && telefono[num] <= 57)) {
      std::cout << "Mal telefono" << std::endl;
      throw std::exception();
    }
  }

  bool emailValido = std::regex_match(email, expReg);

  if (!emailValido) {
    std::cout << "Mal email" << std::endl;
    throw std::exception();
  }

  // Si pasa todas las pruebas, los datos son validos

  this->nombre = nombre;
  this->telefono = telefono;
  this->email = email;
}

std::string Contact::getNombre() const { return this->nombre; }

void Contact::setNombre(std::string nombre) {

  if (nombre.length() == 0)
    return;

  for (int letra{}; letra < nombre.length(); letra++) {

    // Para el caso del nombre
    if ((nombre[letra] >= 0 && nombre[letra] <= 31) || nombre[letra] == 127) {
      std::cout << "Mal nombre" << std::endl;
      throw std::exception();
    }
  }

  this->nombre = nombre;
}

std::string Contact::getTelefono() const { return this->telefono; }

void Contact::setTelefono(std::string telefono) {

  if (telefono.length() == 0)
    return;

  for (int num{}; num < telefono.length(); num++) {

    // Para el caso del telefono
    if (!(telefono[num] >= 48 && telefono[num] <= 57)) {
      std::cout << "Mal telefono" << std::endl;
      throw std::exception();
    }
  }

  this->telefono = telefono;
}

std::string Contact::getEmail() const { return this->email; }

void Contact::setEmail(std::string email) {

  bool emailValido = std::regex_match(email, expReg);

  if (!emailValido) {
    throw std::exception();
  }

  this->email = email;
}

void Contact::imprimirInfo() {

  printf("| %-13s | ", this->getNombre().c_str());
  printf(" %-15s | ", this->getTelefono().c_str());
  printf(" %-22s |\n", this->getEmail().c_str());

  return;
}

// Funciones necesarias para que la libreria nlohmann/json pueda exportar e
// importar la estructura a formato json
void to_json(nlohmann::json &J, const Contact &contacto) {

  J = nlohmann::json{{"nombre", contacto.getNombre()},
                     {"telefono", contacto.getTelefono()},
                     {"email", contacto.getEmail()}};
}

/*
void from_json(const nlohmann::json &j, Contact &contacto) {
  contacto.setNombre(j.at("nombre").get<std::string>());
  contacto.setTelefono(j.at("telefono").get<std::string>());
}
*/
