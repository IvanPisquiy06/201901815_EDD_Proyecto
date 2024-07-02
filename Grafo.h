#ifndef GRAFO_H
#define GRAFO_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

class NodoAdyacente {
public:
    std::string destino;
    int distancia;
    NodoAdyacente* siguiente;

    NodoAdyacente(std::string destino, int distancia) 
    : destino(destino), distancia(distancia), siguiente(nullptr) {}
};

class Vertice {
public:
    std::string nombre;
    NodoAdyacente* adyacentes;
    Vertice* siguiente;

    Vertice(std::string nombre) 
    : nombre(nombre), adyacentes(nullptr), siguiente(nullptr) {}

    void agregarAdyacente(std::string destino, int distancia) {
        NodoAdyacente* nuevoNodo = new NodoAdyacente(destino, distancia);
        nuevoNodo->siguiente = adyacentes;
        adyacentes = nuevoNodo;
    }
};

class Grafo {
private:
    Vertice* vertices;

    Vertice* buscarVertice(std::string nombre) {
        Vertice* temp = vertices;
        while (temp != nullptr) {
            if (temp->nombre == nombre) {
                return temp;
            }
            temp = temp->siguiente;
        }
        return nullptr;
    }

public:
    Grafo() : vertices(nullptr) {}

    void agregarVertice(std::string nombre) {
        if (buscarVertice(nombre) == nullptr) {
            Vertice* nuevoVertice = new Vertice(nombre);
            nuevoVertice->siguiente = vertices;
            vertices = nuevoVertice;
        }
    }

    void agregarArista(std::string origen, std::string destino, int distancia) {
        Vertice* verticeOrigen = buscarVertice(origen);
        if (verticeOrigen != nullptr) {
            verticeOrigen->agregarAdyacente(destino, distancia);
        } else {
            std::cerr << "Vertice origen no encontrado: " << origen << std::endl;
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

            agregarVertice(origen);
            agregarVertice(destino);
            agregarArista(origen, destino, distancia);
        }
    }

    void mostrarGrafo() {
        Vertice* temp = vertices;
        while (temp != nullptr) {
            std::cout << "Vertice " << temp->nombre << " tiene adyacentes: " << std::endl;
            NodoAdyacente* ady = temp->adyacentes;
            while (ady != nullptr) {
                std::cout << "  -> " << ady->destino << " (" << ady->distancia << " km)" << std::endl;
                ady = ady->siguiente;
            }
            temp = temp->siguiente;
        }
    }

    void generarDot(const std::string& nombreArchivo) {
        std::ofstream archivo(nombreArchivo);
        archivo << "digraph Grafo {\n";
        Vertice* temp = vertices;
        while (temp != nullptr) {
            NodoAdyacente* ady = temp->adyacentes;
            while (ady != nullptr) {
                archivo << "\"" << temp->nombre << "\" -> \"" << ady->destino << "\" [label=\"" << ady->distancia << " km\"];\n";
                ady = ady->siguiente;
            }
            temp = temp->siguiente;
        }
        archivo << "}\n";
        archivo.close();
    }
};

#endif
