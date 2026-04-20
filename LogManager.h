#ifndef LOGMANAGER_H
#define LOGMANAGER_H

#include <string>

/**
 * @brief Maneja el registro de acciones en el sistema.
 */
class LogManager{
public:
    /**
     * @brief Guarda una acción en el archivo de logs.
     * @param accion Descripción de la acción realizada.
     */
    static void registrar(const std::string& accion);
};

#endif
