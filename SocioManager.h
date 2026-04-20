#ifndef SOCIOMANAGER_H
#define SOCIOMANAGER_H

#include <vector>
#include <string>

/**
 * @brief Gestiona las operaciones relacionadas con los socios.
 */
class SocioManager{
public:

    /// Genera un nuevo ID basado en los datos existentes
    static int generarNuevoID(const std::vector<std::string>& datos);

    /// Agrega un nuevo socio
    static void agregarSocio(std::vector<std::string>& datos, const std::string& nombre, double saldo);

    /// Elimina un socio por ID
    static void eliminarSocio(std::vector<std::string>& datos, const std::string& id);

    /// Actualiza los datos de un socio
    static void actualizarSocio(std::vector<std::string>& datos, const std::string& id, const std::string& nombre, double saldo);
};


#endif