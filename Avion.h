#ifndef AVION
#define AVION

#include <string>

class Avion{
public:
    std::string vuelo;
    std::string numero_de_registro;
    std::string modelo;
    int capacidad;
    std::string aerolinea;
    std::string ciudad_destino;
    std::string estado;

    Avion(std::string vuelo, std::string numero_de_registro, std::string modelo, int capacidad, std::string aerolinea, std::string ciudad_destino, std::string estado)
    : vuelo(vuelo), numero_de_registro(numero_de_registro), modelo(modelo), capacidad(capacidad), aerolinea(aerolinea), ciudad_destino(ciudad_destino), estado(estado) {};
};

#endif