#include "model/ArbolB.h"
#include "model/Contact.h"
#include "model/ContactList.h"
#include <string>
#include <vector>

class GestorJSON {
public:
  static bool guardarArchivo(std::string nombreA, const ArbolB &directorio);
  static bool cargarArchivo(std::string nombreA, ArbolB &tree,
                            ContactList &tablaHash);

private:
};
