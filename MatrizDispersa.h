#ifndef MATRIZ_DISPERSA_H
#define MATRIZ_DISPERSA_H

#include <unordered_map>
#include <string>
#include "Piloto.h"
#include <iostream>
#include <fstream>

class MatrizDispersa {
private:
    std::unordered_map<std::string, std::unordered_map<std::string, Piloto*>> matriz;

public:
    void asignarPiloto(const std::string& vuelo, const std::string& ciudadDestino, Piloto* piloto) {
        matriz[vuelo][ciudadDestino] = piloto;
    }

    void eliminarPiloto(const std::string& vuelo, const std::string& ciudadDestino) {
        if (matriz[vuelo].count(ciudadDestino)) {
            matriz[vuelo].erase(ciudadDestino);
            if (matriz[vuelo].empty()) {
                matriz.erase(vuelo);
            }
        }
    }

    Piloto* obtenerPiloto(const std::string& vuelo, const std::string& ciudadDestino) {
        if (matriz.count(vuelo) && matriz[vuelo].count(ciudadDestino)) {
            return matriz[vuelo][ciudadDestino];
        }
        return nullptr;
    }

    void imprimirMatriz() const {
        for (const auto& vueloPair : matriz) {
            for (const auto& ciudadPair : vueloPair.second) {
                std::cout << "Vuelo: " << vueloPair.first << ", Ciudad Destino: " << ciudadPair.first << ", Piloto: " << ciudadPair.second->nombre << std::endl;
            }
        }
    }

    void generarDot(const std::string& filename) const {
        std::ofstream dotFile(filename);
        if (!dotFile.is_open()) {
            std::cerr << "Error al abrir el archivo " << filename << " para escritura." << std::endl;
            return;
        }

        dotFile << "digraph MatrizDispersa {" << std::endl;

        // Recorrer la matriz y escribir nodos y conexiones
        for (const auto& vueloPair : matriz) {
            std::string vuelo = vueloPair.first;
            for (const auto& ciudadPair : vueloPair.second) {
                std::string ciudadDestino = ciudadPair.first;
                Piloto* piloto = ciudadPair.second;

                dotFile << "  \"" << vuelo << "\" -> \"" << ciudadDestino << "\" [label=\"Piloto: " << piloto->nombre << "\"];" << std::endl;
            }
        }

        dotFile << "}" << std::endl;
        dotFile.close();

        std::cout << "Archivo " << filename << " generado exitosamente." << std::endl;
    }
};

#endif // MATRIZ_DISPERSA_H
