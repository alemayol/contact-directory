#pragma once

#include <nlohmann/json.hpp>
#include <regex>
#include <string>
#define CONTACT_H

class Contact {
public:
  Contact();
  Contact(std::string nombre, std::string telefono, std::string email);
  void imprimirInfo();
  std::string getNombre() const;
  void setNombre(std::string nombre);
  std::string getTelefono() const;
  void setTelefono(std::string telefono);
  std::string getEmail() const;
  void setEmail(std::string email);
  friend void to_json(nlohmann::json &J, const Contact &contacto);
  friend void from_json(const nlohmann::json &j, Contact &contacto);

private:
  std::string nombre;
  std::string telefono;
  std::string email;
  // Expresion regular para verificar email
  std::regex expReg;
};
