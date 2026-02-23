#include "model/ArbolB.h"
#include "data/GestorJSON.h"
#include "model/Contact.h"
#include "model/NodoContacto.h"
#include <algorithm>
#include <array>
#include <cctype>
#include <iostream>
#include <memory>
#include <nlohmann/json.hpp>
#include <string>
#include <strings.h>
#include <vector>

ArbolB::ArbolB() {
  this->nodoRaiz = nullptr;
  this->contactosRecientes.reserve(10);
  this->idxRecientes = 0;
}

ArbolB::ArbolB(std::unique_ptr<NodoContacto> nodo) {

  this->nodoRaiz = std::move(nodo);
  this->contactosRecientes.reserve(10);
  this->idxRecientes = 0;
}

void ArbolB::insertarContacto(Contact contacto) {

  insertar(nodoRaiz, contacto);
  GestorJSON::guardarArchivo("directorio.json", *this);
};

void ArbolB::insertar(std::unique_ptr<NodoContacto> &raiz, Contact contacto) {

  if (raiz == nullptr) {

    raiz = std::make_unique<NodoContacto>(contacto);
    agregarRecientes(contacto);

    std::cout << "Contacto agregado!" << std::endl;
    return;
  }

  if (isEqualTo(contacto.getNombre(), raiz->contacto.getNombre())) {
    std::cout << "[ADVERTENCIA] El contacto ya existe. Si desea modificarlo "
                 "elija la opcion de modificacion"
              << std::endl;
    return;
  }

  if (isLessThan(contacto.getNombre(), raiz->contacto.getNombre())) {
    insertar(raiz->nodoI, contacto);
  } else {
    insertar(raiz->nodoD, contacto);
  }

  return;
}

std::vector<Contact> ArbolB::consultarContacto(std::string nombre) {
  this->ultimaBusqueda.clear();
  consultar(nodoRaiz, nombre);

  return this->ultimaBusqueda;
}

void ArbolB::consultar(std::unique_ptr<NodoContacto> &raiz,
                       std::string nombre) {

  if (raiz == nullptr)
    return;

  double lvshteinResult =
      distanciaLevenshtein(raiz->contacto.getNombre(), nombre);

  if (lvshteinResult > 99.9) {
    this->ultimaBusqueda
        .clear(); // Solo mostraremos el contacto exacto encontrado
    this->ultimaBusqueda.push_back(raiz->contacto);
    this->agregarRecientes(raiz->contacto);
    std::cout << "GOT IT" << std::endl;
    return;
  }

  // En caso de no conseguir el contacto especifico, podemos mostrar los mas
  // parecidos (Fuzzy search)
  if (lvshteinResult > 75.0) {
    this->ultimaBusqueda.push_back(raiz->contacto);
  }

  if (isLessThan(nombre, raiz->contacto.getNombre().c_str())) {
    consultar(raiz->nodoI, nombre);
  } else {
    consultar(raiz->nodoD, nombre);
  }

  return;
}

Contact *ArbolB::existeContacto(std::string nombre) {

  return buscarContacto(nodoRaiz, nombre);
}

Contact *ArbolB::buscarContacto(std::unique_ptr<NodoContacto> &raiz,
                                std::string nombre) {

  if (raiz == nullptr) {
    return nullptr;
  }

  if (isEqualTo(nombre, raiz->contacto.getNombre())) {
    return &raiz->contacto;
  }

  if (isLessThan(nombre, raiz->contacto.getNombre())) {
    buscarContacto(raiz->nodoI, nombre);
  } else {
    buscarContacto(raiz->nodoD, nombre);
  }

  return nullptr;
}

void ArbolB::actualizarContacto(std::string nombreAEliminar, Contact contacto) {

  eliminar(nodoRaiz, nombreAEliminar);

  insertar(nodoRaiz, contacto);
}

void ArbolB::actualizar(std::unique_ptr<NodoContacto> &raiz, Contact contacto) {

  if (raiz == nullptr) {
    return;
  }

  if (isEqualTo(contacto.getNombre(), raiz->contacto.getNombre())) {
    raiz->contacto = contacto;
    return;
  }

  if (isLessThan(contacto.getNombre(), raiz->contacto.getNombre())) {
    actualizar(raiz->nodoI, contacto);
  } else {
    actualizar(raiz->nodoD, contacto);
  }

  return;
}

void ArbolB::agregarRecientes(Contact contacto) {

  removeFromRecientes(contacto.getNombre());

  if (contactosRecientes.size() < CANT_RECIENTES) {
    contactosRecientes.push_back(contacto);
    return;
  }
  contactosRecientes[idxRecientes % CANT_RECIENTES] = contacto;
  idxRecientes++;
}

bool ArbolB::eliminarContacto(std::string nombre) {

  return eliminar(nodoRaiz, nombre);
}

bool ArbolB::eliminar(std::unique_ptr<NodoContacto> &raiz, std::string nombre) {

  if (raiz == nullptr)
    return false;

  std::cout << "Comparing: " << raiz->contacto.getNombre() << " -> " << nombre
            << std::endl;

  if (isLessThan(nombre, raiz->contacto.getNombre().c_str())) {
    eliminar(raiz->nodoI, nombre);
  } else if (strcasecmp(nombre.c_str(), raiz->contacto.getNombre().c_str()) >
             0) {
    eliminar(raiz->nodoD, nombre);
  } else {

    // Lo removemos de los contacto recientes en caso de encontrarse alli
    removeFromRecientes(raiz->contacto.getNombre());

    // Verificamos que los nodos hijos no sean nulos, si ambos existen significa
    // que el nodo tiene dos hijos y procedemos a encontrar el nodo mayor entre
    // los menores
    if (!raiz->nodoI) {
      raiz = std::move(raiz->nodoD);
    } else if (!raiz->nodoD) {
      raiz = std::move(raiz->nodoI);
    } else {
      NodoContacto *nodoReemplazante = subtreeBiggestLeft(raiz);

      raiz->contacto = nodoReemplazante->contacto;
      eliminar(raiz->nodoI, raiz->contacto.getNombre());
    }

    std::cout << "Contacto eliminado!" << std::endl;
    return true;
  }

  return false;
}

void ArbolB::removeFromRecientes(std::string nombre) {

  for (int i{}; i < contactosRecientes.size(); i++) {

    if (isEqualTo(contactosRecientes[i].getNombre(), nombre)) {
      contactosRecientes.erase(contactosRecientes.begin() + i);
    }
  }
}

std::vector<Contact> ArbolB::getRecientes() const {
  return this->contactosRecientes;
}

NodoContacto *ArbolB::subtreeBiggestLeft(std::unique_ptr<NodoContacto> &raiz) {

  NodoContacto *temp = raiz->nodoI.get();

  while (temp != nullptr) {

    if (temp->nodoD != nullptr) {
      temp = temp->nodoD.get();
      continue;
    }
  }

  return temp;
}

int ArbolB::numNodos(std::unique_ptr<NodoContacto> &raiz) {
  if (raiz == nullptr)
    return 0;

  return numNodos(raiz->nodoI) + numNodos(raiz->nodoD) + 1;
}

int ArbolB::numNodos() { return numNodos(this->nodoRaiz); }

int ArbolB::alturaArbol(std::unique_ptr<NodoContacto> &raiz) {
  if (raiz == nullptr)
    return -1;

  // Calcula la altura de los subarboles
  int altI = alturaArbol(raiz->nodoI);
  int altD = alturaArbol(raiz->nodoD);

  return std::max(altI, altD) + 1;
}

int ArbolB::alturaArbol() { return alturaArbol(this->nodoRaiz); }

double ArbolB::distanciaLevenshtein(std::string base, std::string destino) {

  // Agregamos uno para contar la operacion de cadena vacia en el algoritmo
  int a = base.length() + 1;
  int b = destino.length() + 1;

  std::vector<std::vector<int>> operations(a, std::vector<int>(b, 0));

  // Llenamos la primera fila con los indices, representando la operacion de
  // eliminacion
  for (int i = 0; i < b; i++) {
    // operations[0].insert(operations[0].begin() + i, i);
    operations[0][i] = i;
  }

  for (int i{}; i < a; i++) {
    // Insertamos el indice en la primera posicion de cada fila, esto es la
    // operacion de insercion
    // operations[i].insert(operations[i].begin(), i);
    operations[i][0] = i;
  }

  // Ahora iteramos de acuerdo a la longitud real de las cadenas

  std::string baseCI = toLowerCase(base);
  std::string destinoCI = toLowerCase(destino);

  for (int i = 1; i < a; i++) {

    for (int j = 1; j < b; j++) {

      if (baseCI[i - 1] == destinoCI[j - 1]) {
        operations[i][j] = operations[i - 1][j - 1];
        continue;
      }

      operations[i][j] = minValue(operations[i - 1][j - 1],
                                  operations[i][j - 1], operations[i - 1][j]) +
                         1;
    }
  }

  for (int i = 0; i < a; i++) {
    if (i == 0)
      std::cout << " " << "X" << " ";
    else if (i >= 1)
      std::cout << base[i - 1] << " ";
  }

  std::cout << std::endl;

  for (int i = 0; i < a; i++) {

    if (i == 0) {
      std::cout << "X" << " ";
    } else if (i >= 1) {
      std::cout << destino[i - 1] << " ";
    }

    for (int j = 0; j < b; j++) {

      std::cout << operations[i][j] << " ";
    }

    std::cout << std::endl;
  }

  std::cout << std::endl;

  // Porcentaje. e.g. cadena.length = 8 -> 8 - 5(operaciones) = 3 -> 3 / 8 *
  // 100%

  std::cout << "Longitud de cadena: " << base.length() << std::endl;
  std::cout << "Numero de operaciones a realizar: " << operations[a - 1][b - 1]
            << std::endl;

  double result =
      (static_cast<double>((base.length() - operations[a - 1][b - 1])) /
       static_cast<double>(base.length())) *
      100.0;

  std::cout << "Resultado DLevenshtein: " << result << std::endl;

  return result;
}

void ArbolB::imprimirContactos() {
  std::cout << "\n--- Lista de Contactos (Orden Alfabetico) ---\n" << std::endl;

  printf("| %3s Nombre %3s| %3s Telefono %3s | %8s Email %9s|\n", "", "", "",
         "", "", "");

  imprimirContactos(this->nodoRaiz);
}

// Recorrido In-Order
void ArbolB::imprimirContactos(std::unique_ptr<NodoContacto> &raiz) {

  if (raiz == nullptr)
    return;

  imprimirContactos(raiz->nodoI);

  raiz->contacto.imprimirInfo();

  imprimirContactos(raiz->nodoD);
}

void ArbolB::exportToJSON(std::unique_ptr<NodoContacto> &raiz,
                          nlohmann::json &arr) {
  if (raiz == nullptr)
    return;

  arr.push_back(raiz->contacto);
  exportToJSON(raiz->nodoI, arr);
  exportToJSON(raiz->nodoD, arr);
}

void ArbolB::preorderAction(
    const std::unique_ptr<NodoContacto> &raiz,
    std::function<void(const Contact &contacto)> &action) const {

  if (raiz == nullptr)
    return;

  action(raiz->contacto);
  preorderAction(raiz->nodoI, action);
  preorderAction(raiz->nodoD, action);
}

void const ArbolB::performPreorderAction(
    std::function<void(const Contact &contacto)> action) const {

  preorderAction(nodoRaiz, action);
}

// Utilidades

std::string ArbolB::toLowerCase(std::string cad) {

  std::transform(cad.begin(), cad.end(), cad.begin(),
                 [](unsigned char c) { return std::tolower(c); });

  return cad;
}

int ArbolB::minValue(int a, int b, int c) {
  int temp = std::min(a, b);

  return std::min(temp, c);
}

bool ArbolB::isLessThan(std::string cad1, std::string cad2) {

  // Comparamos los nombres en miniscula, si la cadena cad1 es
  // alfabeticamente menor a cad2
  // cad1 ira a la izquierda del arbol
  return strcasecmp(cad1.c_str(), cad2.c_str()) < 0;
}

bool ArbolB::isEqualTo(std::string cad1, std::string cad2) {

  // Comparamos los nombres en miniscula, si la respuesta es cero, son iguales
  return strcasecmp(cad1.c_str(), cad2.c_str()) == 0;
}
