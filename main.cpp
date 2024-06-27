#include <iostream>
#include <fstream>
#include <string>
#include "libs/json.hpp"
#include "Avion.h"
#include "Piloto.h"
#include "ArbolB.h"
#include "ListaCircularDoble.h"


using json = nlohmann::json;

ListaDoble listaMantenimiento;
ArbolB arbolDisponibles;

void cargarAviones(const std::string& archivo) {
    std::ifstream ifs(archivo);
    if (!ifs.is_open()) {
        std::cerr << "Error opening file: " << archivo << std::endl;
        return;
    }
    json obj;
    ifs >> obj; 

    for (const auto& avion : obj) {
        Avion* nuevoAvion = new Avion(
            avion["vuelo"].get<std::string>(),
            avion["numero_de_registro"].get<std::string>(),
            avion["modelo"].get<std::string>(),
            avion["capacidad"].get<int>(),
            avion["aerolinea"].get<std::string>(),
            avion["ciudad_destino"].get<std::string>(),
            avion["estado"].get<std::string>()
        );
        if (nuevoAvion->estado == "Disponible") {
            arbolDisponibles.insertar(nuevoAvion);
        } else if (nuevoAvion->estado == "Mantenimiento") {
            listaMantenimiento.insertar(nuevoAvion);
        }
    }
}

void cargarPilotos(const std::string& archivo) {
    std::ifstream ifs(archivo);
    if (!ifs.is_open()) {
        std::cerr << "Error opening file: " << archivo << std::endl;
        return;
    }
    json obj;
    ifs >> obj;

    for (const auto& piloto : obj) {
        Piloto* nuevoPiloto = new Piloto(
            piloto["nombre"].get<std::string>(),
            piloto["nacionalidad"].get<std::string>(),
            piloto["numero_de_id"].get<std::string>(),
            piloto["vuelo"].get<std::string>(),
            piloto["horas_de_vuelo"].get<int>(),
            piloto["tipo_de_licencia"].get<std::string>()
        );
        if (nuevoPiloto->vuelo == "Disponible") {
            std::cout << "Insertando piloto " << nuevoPiloto->nombre << " en lista circular doble" << std::endl;
        }
    }
}

void cambiarEstado(const std::string& numeroRegistro, const std::string& nuevoEstado) {

    Avion* avion = arbolDisponibles.buscar(numeroRegistro);
    if (avion) {
        avion->estado = nuevoEstado;
        arbolDisponibles.insertar(new Avion(*avion));
    } else {
        avion = listaMantenimiento.buscar(numeroRegistro);
        if (avion) {
            listaMantenimiento.eliminar(numeroRegistro);
            avion->estado = nuevoEstado;
            arbolDisponibles.insertar(new Avion(*avion));
        }
    }
}

void mostrarMenu() {
    std::cout << "Menu de opciones:" << std::endl;
    std::cout << "1. Cargar aviones" << std::endl;
    std::cout << "2. Imprimir aviones disponibles" << std::endl;
    std::cout << "3. Imprimir aviones en mantenimiento" << std::endl;
    std::cout << "4. Salir" << std::endl;
}

int main() {
    int opcion;
    std::string archivoAviones = "..//archivos_prueba//aviones.json";

    do {
        mostrarMenu();
        std::cin >> opcion;

        switch (opcion) {
        case 1:
            cargarAviones(archivoAviones);
            break;
        case 2:
            arbolDisponibles.imprimir();
            break;
        case 3:
            listaMantenimiento.imprimir();
            break;
        case 4:
            std::cout << "Saliendo..." << std::endl;
            break;
        default:
            std::cout << "Opcion invalida, intenta de nuevo." << std::endl;
        }
    } while (opcion != 4);

    return 0;
}