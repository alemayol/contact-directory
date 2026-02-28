#include "model/ArbolB.h"
#include "model/ContactList.h"
#include "model/NodoContacto.h"
#include <memory>
#include <ostream>
#include <string>

class GestorCSV {
public:
  static bool exportarContactos(std::string nombreA, const ArbolB &directorio);
  static void inorderWrite(const ArbolB &directorio, std::ostream &file);
  static bool cargarArchivo(std::string nombreA, ArbolB &directorio,
                            ContactList &tablaHash);

  static std::string prepareTextCSV(std::string cad);

private:
};
