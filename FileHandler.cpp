#include "FileHandler.h"
#include <fstream>

/**
 * @brief Lee un archivo de texto línea por línea.
 */
std::vector<std::string> FileHandler::leerArchivo(const std::string& ruta){

    std::ifstream archivo(ruta);
    std::vector<std::string> datos;
    std::string linea;

    // Leer cada línea y guardarla en el vector
    while(getline(archivo, linea)){
        datos.push_back(linea);
    }

    archivo.close();
    return datos;

}

/**
 * @brief Escribe datos en un archivo de texto.
 */
void FileHandler::escribirArchivo(const std::string& ruta, const std::vector<std::string>& datos){

    std::ofstream archivo(ruta);

    // Escribir cada línea en el archivo
    for(const auto& l : datos){
        archivo << l << std::endl;
    }

    archivo.close();
}