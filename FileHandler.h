#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <vector>
#include <string>

/**
 * @brief Maneja la lectura y escritura de archivos de texto.
 */


class FileHandler{
public:
    /**
     * @brief Lee un archivo y retorna sus líneas.
     * @param ruta Ruta del archivo.
     */
    static std::vector<std::string> leerArchivo(const std::string& ruta);

    /**
     * @brief Escribe datos en un archivo.
     * @param ruta Ruta del archivo.
     * @param datos Contenido a guardar.
     */
    static void escribirArchivo(const std::string& ruta, const std::vector<std::string>& datos);
};

#endif
