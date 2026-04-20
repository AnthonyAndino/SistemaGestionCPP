#include "LogManager.h"
#include <fstream>
#include <ctime>

/**
 * @brief Registra una acción en el archivo de logs con fecha y hora.
 */
void LogManager::registrar(const std::string& accion){

    std::ofstream archivo("log.txt", std::ios::app);

    if(!archivo.is_open()) return;

    //obtener la fecha y hora actual
    time_t now =  time(0);
    tm *ltm = localtime(&now);

    char fecha[20];
    sprintf(fecha, "%04d-%02d-%02d %02d:%02d:%02d", 1900 + ltm->tm_year, 1 + ltm->tm_mon, ltm->tm_mday, ltm->tm_hour, ltm->tm_min, ltm->tm_sec);

    // Guardar log en formato: [fecha] acción
    archivo << "[" << fecha << "]" << accion << std::endl;

    archivo.close();
}