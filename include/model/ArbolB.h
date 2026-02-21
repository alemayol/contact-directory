#pragma once

#include <functional>
#include <nlohmann/json_fwd.hpp>
#define ARBOLB_H

#include "model/Contact.h"
#include "model/NodoContacto.h"
#include <memory>
#include <string>

class ArbolB {
private:
  std::unique_ptr<NodoContacto> nodoRaiz;
  std::vector<Contact> ultimaBusqueda;
  std::vector<Contact> contactosRecientes;
  static const int CANT_RECIENTES = 10;
  int idxRecientes; // Para manejar un arreglo circular
  void insertar(std::unique_ptr<NodoContacto> &raiz, Contact contacto);
  void actualizar(std::unique_ptr<NodoContacto> &raiz, Contact contacto);
  void consultar(std::unique_ptr<NodoContacto> &raiz, std::string nombre);
  Contact *buscarContacto(std::unique_ptr<NodoContacto> &raiz,
                          std::string nombre);
  bool eliminar(std::unique_ptr<NodoContacto> &raiz, std::string nombre);
  int calcularClave(std::string actual,
                    std::string nuevo); // Para ordenar el arbol de acuerdo al
                                        // nombre del contacto
  NodoContacto *subtreeBiggestLeft(std::unique_ptr<NodoContacto> &raiz);
  int numNodos(std::unique_ptr<NodoContacto> &raiz);
  int alturaArbol(std::unique_ptr<NodoContacto> &raiz);
  // Calcula la distancia de edicion entre dos cadenas, i.e. que tan parecidas
  // son
  double distanciaLevenshtein(std::string cad1, std::string cad2);
  int minValue(int a, int b, int c);

  void imprimirContactos(std::unique_ptr<NodoContacto> &raiz);
  void exportToJSON(std::unique_ptr<NodoContacto> &raiz, nlohmann::json &arr);
  std::string toLowerCase(std::string cad);
  void removeFromRecientes(std::string nombre);
  void
  preorderAction(const std::unique_ptr<NodoContacto> &raiz,
                 std::function<void(const Contact &contacto)> &action) const;

public:
  ArbolB();
  ~ArbolB() { nodoRaiz.reset(); }
  ArbolB(std::unique_ptr<NodoContacto> raiz);
  void insertarContacto(Contact contacto);
  void actualizarContacto(std::string nombreAEliminar, Contact contacto);
  bool eliminarContacto(std::string nombre);
  std::vector<Contact> consultarContacto(std::string nombre);
  Contact *existeContacto(std::string nombre);
  void agregarRecientes(Contact contacto);
  std::vector<Contact> getRecientes() const;
  void const
  performPreorderAction(std::function<void(const Contact &)> action) const;
  int numNodos();
  int alturaArbol();
  void borrarArbol();
  void imprimirContactos();
  bool isLessThan(std::string cad1, std::string cad2);
  bool isEqualTo(std::string cad1, std::string cad2);
  void exportToJSON(nlohmann::json &arr);
};
