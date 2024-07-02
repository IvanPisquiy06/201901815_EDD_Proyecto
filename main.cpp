#include <iostream>
#include <fstream>
#include <string>
#include "libs/json.hpp"
#include "Avion.h"
#include "Piloto.h"
#include "ArbolB.h"
#include "ListaCircularDoble.h"
#include "ArbolBinario.h"
#include "TablaHash.h"
#include "Grafo.h"
#include "MatrizDispersa.h"

using json = nlohmann::json;

ListaDoble listaMantenimiento;
ArbolB arbolDisponibles;
BST arbolPilotos;
HashTable tablaHash;
Grafo grafo;
MatrizDispersa matrizVuelos; // Añadir la matriz dispersa
std::unordered_map<std::string, std::string> vueloACiudadDestino; // Mapa para guardar vuelo y ciudad destino

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
        // Guardar la relación vuelo-ciudad destino
        vueloACiudadDestino[nuevoAvion->vuelo] = nuevoAvion->ciudad_destino;

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
        arbolPilotos.insertar(nuevoPiloto);
        tablaHash.insertar(nuevoPiloto);

        // Asignar piloto a la matriz dispersa usando el vuelo y la ciudad destino correspondiente
        std::string ciudadDestino = vueloACiudadDestino[nuevoPiloto->vuelo];
        matrizVuelos.asignarPiloto(nuevoPiloto->vuelo, ciudadDestino, nuevoPiloto);
    }
}

void cargarRutasDesdeArchivo(std::string nombreArchivo) {
    std::ifstream archivo(nombreArchivo);
    std::string linea;
    while (std::getline(archivo, linea)) {
        std::stringstream ss(linea);
        std::string origen, destino, distanciaStr;
        std::getline(ss, origen, '/');
        std::getline(ss, destino, '/');
        std::getline(ss, distanciaStr, ';');
        int distancia = std::stoi(distanciaStr);

        grafo.agregarVertice(origen);
        grafo.agregarVertice(destino);
        grafo.agregarArista(origen, destino, distancia);
    }
}

void cambiarEstado(const std::string& numeroRegistro, const std::string& nuevoEstado) {
    Avion* avion = arbolDisponibles.buscar(numeroRegistro);
    if (avion) {
        avion->estado = nuevoEstado;
        if (nuevoEstado == "Mantenimiento") {
            listaMantenimiento.insertar(avion);
        }
    } else {
        avion = listaMantenimiento.buscar(numeroRegistro);
        if (avion && nuevoEstado == "Disponible") {
            listaMantenimiento.eliminar(numeroRegistro);
            avion->estado = nuevoEstado;
            arbolDisponibles.insertar(avion);
        }
    }
}

void darDeBajaPiloto(const std::string& numeroDeId) {
    Piloto* piloto = tablaHash.buscar(numeroDeId);
    if (piloto) {
        tablaHash.eliminar(piloto->numero_de_id);
        matrizVuelos.eliminarPiloto(piloto->vuelo, vueloACiudadDestino[piloto->vuelo]);
        delete piloto; 
    } else {
        std::cerr << "Piloto con ID " << numeroDeId << " no encontrado." << std::endl;
    }
}

void procesarMovimientos(const std::string& archivoMovimientos) {
    std::ifstream ifs(archivoMovimientos);
    if (!ifs.is_open()) {
        std::cerr << "Error opening file: " << archivoMovimientos << std::endl;
        return;
    }
    std::string linea;
    while (std::getline(ifs, linea)) {
        std::istringstream iss(linea);
        std::string accion, detalle1, detalle2;
        if (std::getline(iss, accion, ',') && std::getline(iss, detalle1, ',') && std::getline(iss, detalle2, ',')) {
            if (accion == "MantenimientoAviones") {
                cambiarEstado(detalle2, detalle1);
            }
        } else if (linea.find("DarDeBaja(") != std::string::npos) {
            size_t start = linea.find("(") + 1;
            size_t end = linea.find(")");
            if (start != std::string::npos && end != std::string::npos) {
                std::string numeroDeId = linea.substr(start, end - start);
                darDeBajaPiloto(numeroDeId);
            }
        }
    }
}

void generarYMostrarReporte(const std::string& nombreArchivoDot, const std::string& nombreArchivoPng, void(*generarDot)(const std::string&)) {
    generarDot(nombreArchivoDot);
    std::string comando = "dot -Tpng " + nombreArchivoDot + " -o " + nombreArchivoPng;
    system(comando.c_str());
    std::cout << "Reporte generado: " << nombreArchivoPng << std::endl;
}

void mostrarMenu() {
    std::cout << "Menu de opciones:" << std::endl;
    std::cout << "1. Cargar aviones" << std::endl;
    std::cout << "2. Cargar Pilotos" << std::endl;
    std::cout << "3. Cargar de Rutas" << std::endl;
    std::cout << "4. Carga de movimientos" << std::endl;
    std::cout << "5. Imprimir aviones en mantenimiento" << std::endl;
    std::cout << "6. Menu Pilotos" << std::endl;
    std::cout << "7. Visualizar reportes" << std::endl;
    std::cout << "8. Salir" << std::endl;
}

void mostrarMenuPilotos(){
    std::cout << "----Seleccione el recorrido----" << std::endl;
    std::cout << "1. Preorden" << std::endl;
    std::cout << "2. Inorden" << std::endl;
    std::cout << "3. Postorden" << std::endl;
    std::cout << "4. Salir" << std::endl;
}

void menuPilotos(){
    int opcion;
    while (true)
    {
        mostrarMenuPilotos();
        std::cin >> opcion;
        switch (opcion)
        {
        case 1:
            std::cout << "Pilotos en preorden:" << std::endl;
            arbolPilotos.mostrarPreorden();
            generarYMostrarReporte("arbol_pilotos.dot", "arbol_pilotos.png", [](const std::string& archivo){ arbolPilotos.generarDotPreorden(archivo); });

            break;
        case 2:
            std::cout << "Pilotos en inorden:" << std::endl;
            arbolPilotos.mostrarInorden();
            generarYMostrarReporte("arbol_pilotos.dot", "arbol_pilotos.png", [](const std::string& archivo){ arbolPilotos.generarDotInorden(archivo); });

            break;
        case 3:
            std::cout << "Pilotos en postorden:" << std::endl;
            arbolPilotos.mostrarPostorden();
            generarYMostrarReporte("arbol_pilotos.dot", "arbol_pilotos.png", [](const std::string& archivo){ arbolPilotos.generarDotPostorden(archivo); });

            break;
        case 4:
            return;
        default:
            std::cout << "Opcion invalida, intenta de nuevo." << std::endl;
            break;
        }
    }
    
}

void visualizarReportes() {
    int opcion;
    std::cout << "----Visualizar Reportes----" << std::endl;
    std::cout << "1. Arbol B con aviones disponibles" << std::endl;
    std::cout << "2. Lista de aviones en mantenimiento" << std::endl;
    std::cout << "3. Menu pilotos" << std::endl;
    std::cout << "4. Tabla hash de pilotos" << std::endl;
    std::cout << "5. Grafo dirigido con las rutas" << std::endl;
    std::cout << "6. Matriz dispersa de vuelos y ciudades" << std::endl;
    std::cout << "7. Salir" << std::endl;

    std::cin >> opcion;
    switch (opcion) {
        case 1:
            generarYMostrarReporte("arbol_aviones.dot", "arbol_aviones.png", [](const std::string& archivo){ arbolDisponibles.generarDot(archivo); });
            break;
        case 2:
            generarYMostrarReporte("lista_mantenimiento.dot", "lista_mantenimiento.png", [](const std::string& archivo){ listaMantenimiento.generarDot(archivo); });
            break;
        case 3:
            menuPilotos();
            break;
        case 4:
            generarYMostrarReporte("tabla_hash.dot", "tabla_hash.png", [](const std::string& archivo){ tablaHash.generarDot(archivo); });
            break;
        case 5:
            generarYMostrarReporte("grafo_rutas.dot", "grafo_rutas.png", [](const std::string& archivo){ grafo.generarDot(archivo); });
            std::cout << "Funcionalidad no implementada." << std::endl;
            break;
        case 6:
            generarYMostrarReporte("grafo_rutas.dot", "grafo_rutas.png", [](const std::string& archivo){ matrizVuelos.generarDot(archivo); });
            matrizVuelos.imprimirMatriz();
            break;
        case 7:
            return;
        default:
            std::cout << "Opcion invalida, intenta de nuevo." << std::endl;
            break;
    }
}

int main() {
    int opcion;
    std::string archivo;

    do {
        mostrarMenu();
        std::cin >> opcion;

        switch (opcion) {
        case 1:
            std::cout << "Ingrese el nombre del archivo de aviones: ";
            std::cin >> archivo;
            cargarAviones(archivo);
            break;
        case 2:
            std::cout << "Ingrese el nombre del archivo de pilotos: ";
            std::cin >> archivo;
            cargarPilotos(archivo);
            break;
        case 3:
            std::cout << "Ingrese el nombre del archivo de rutas: ";
            std::cin >> archivo;
            cargarRutasDesdeArchivo(archivo);
            std::cout << "Mapa de rutas:" << std::endl;
            grafo.mostrarGrafo();
            break;
        case 4:
            std::cout << "Ingrese el nombre del archivo de movimientos: ";
            std::cin >> archivo;
            procesarMovimientos(archivo);
            break;
        case 5:
            listaMantenimiento.imprimir();
            break;
        case 6:
            menuPilotos();
            break;
        case 7:
            visualizarReportes();
            break;
        case 8:
            std::cout << "Saliendo..." << std::endl;
            break;
        default:
            std::cout << "Opcion invalida, intenta de nuevo." << std::endl;
        }
    } while (opcion != 8);

    return 0;
}
