#include "SocioManager.h"

/**
 * @brief Genera un nuevo ID basado en el último registro existente.
 */
int SocioManager::generarNuevoID(const std::vector<std::string>& datos){

    int ultimoID = 0;

    // Buscar el ID más alto en los datos existentes
    for(const auto&  linea : datos){
        size_t pos = linea.find(",");
        if (pos != std::string::npos){
            int id = std::stoi(linea.substr(0, pos));
            if(id > ultimoID){
                ultimoID = id;
            }
        }
    }

    return ultimoID + 1;
}

/**
 * @brief Agrega un nuevo socio al sistema.
 */
void SocioManager::agregarSocio(std::vector<std::string>& datos, const std::string& nombre, double saldo){

    int nuevoID = generarNuevoID(datos);

    datos.push_back(std::to_string(nuevoID) + "," + nombre + "," + std::to_string(saldo));
}

/**
 * @brief Elimina un socio por ID.
 */
void SocioManager::eliminarSocio(std::vector<std::string>& datos, const std::string& id){

    std::vector<std::string> nuevos;

    for(const auto& linea : datos){
        if(linea.find(id + ",") != 0){
            nuevos.push_back(linea);
        }
    }

    datos = nuevos;
}

/**
 * @brief Actualiza los datos de un socio existente.
 */
void SocioManager::actualizarSocio(std::vector<std::string>& datos, const std::string& id, const std::string& nombre, double saldo){

    for(auto& linea : datos){
        size_t pos1 = linea.find(",");
        size_t pos2 = linea.find(",", pos1 + 1);

        if(pos1 == std::string::npos || pos2 == std::string::npos) continue;

        std::string idArchivo = linea.substr(0, pos1);

        if(idArchivo == id){
            linea = id + "," + nombre + "," + std::to_string(saldo);
            break;
        }
    }
}