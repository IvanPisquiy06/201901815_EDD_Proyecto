#include "Piloto.h"
#include <iostream>
#include <fstream>

class NodoBST {
public:
    Piloto* piloto;
    NodoBST* izquierda;
    NodoBST* derecha;

    NodoBST(Piloto* piloto) : piloto(piloto), izquierda(nullptr), derecha(nullptr) {}
};

class BST {
private:
    NodoBST* raiz;

    void insertar(NodoBST*& nodo, Piloto* piloto) {
        if (nodo == nullptr) {
            nodo = new NodoBST(piloto);
        } else if (piloto->horas_de_vuelo < nodo->piloto->horas_de_vuelo) {
            insertar(nodo->izquierda, piloto);
        } else {
            insertar(nodo->derecha, piloto);
        }
    }

    void preorden(NodoBST* nodo, std::ofstream& archivo) {
        if (nodo != nullptr) {
            archivo << nodo->piloto->horas_de_vuelo << " [label=\"" << nodo->piloto->nombre << " - " << nodo->piloto->horas_de_vuelo << " horas\"];\n";
            if (nodo->izquierda != nullptr) {
                archivo << nodo->piloto->horas_de_vuelo << " -> " << nodo->izquierda->piloto->horas_de_vuelo << ";\n";
                preorden(nodo->izquierda, archivo);
            }
            if (nodo->derecha != nullptr) {
                archivo << nodo->piloto->horas_de_vuelo << " -> " << nodo->derecha->piloto->horas_de_vuelo << ";\n";
                preorden(nodo->derecha, archivo);
            }
        }
    }

    void inorden(NodoBST* nodo, std::ofstream& archivo) {
        if (nodo != nullptr) {
            if (nodo->izquierda != nullptr) {
                inorden(nodo->izquierda, archivo);
                archivo << nodo->izquierda->piloto->horas_de_vuelo << " -> " << nodo->piloto->horas_de_vuelo << ";\n";
            }
            archivo << nodo->piloto->horas_de_vuelo << " [label=\"" << nodo->piloto->nombre << " - " << nodo->piloto->horas_de_vuelo << " horas\"];\n";
            if (nodo->derecha != nullptr) {
                inorden(nodo->derecha, archivo);
                archivo << nodo->piloto->horas_de_vuelo << " -> " << nodo->derecha->piloto->horas_de_vuelo << ";\n";
            }
        }
    }

    void postorden(NodoBST* nodo, std::ofstream& archivo) {
        if (nodo != nullptr) {
            if (nodo->izquierda != nullptr) {
                postorden(nodo->izquierda, archivo);
            }
            if (nodo->derecha != nullptr) {
                postorden(nodo->derecha, archivo);
            }
            archivo << nodo->piloto->horas_de_vuelo << " [label=\"" << nodo->piloto->nombre << " - " << nodo->piloto->horas_de_vuelo << " horas\"];\n";
        }
    }

    void eliminarArbol(NodoBST* nodo) {
        if (nodo != nullptr) {
            eliminarArbol(nodo->izquierda);
            eliminarArbol(nodo->derecha);
            delete nodo->piloto;
            delete nodo;
        }
    }

public:
    BST() : raiz(nullptr) {}

    ~BST() {
        eliminarArbol(raiz);
    }

    void insertar(Piloto* piloto) {
        insertar(raiz, piloto);
    }

    void mostrarPreorden() {
        mostrarPreorden(raiz);
    }

    void mostrarInorden() {
        mostrarInorden(raiz);
    }

    void mostrarPostorden() {
        mostrarPostorden(raiz);
    }

    void mostrarPreorden(NodoBST* nodo) {
        if (nodo != nullptr) {
            std::cout << nodo->piloto->nombre << " - " << nodo->piloto->horas_de_vuelo << " horas" << std::endl;
            mostrarPreorden(nodo->izquierda);
            mostrarPreorden(nodo->derecha);
        }
    }

    void mostrarInorden(NodoBST* nodo) {
        if (nodo != nullptr) {
            mostrarInorden(nodo->izquierda);
            std::cout << nodo->piloto->nombre << " - " << nodo->piloto->horas_de_vuelo << " horas" << std::endl;
            mostrarInorden(nodo->derecha);
        }
    }

    void mostrarPostorden(NodoBST* nodo) {
        if (nodo != nullptr) {
            mostrarPostorden(nodo->izquierda);
            mostrarPostorden(nodo->derecha);
            std::cout << nodo->piloto->nombre << " - " << nodo->piloto->horas_de_vuelo << " horas" << std::endl;
        }
    }

    void generarDotPreorden(const std::string& nombreArchivo) {
        std::ofstream archivo(nombreArchivo);
        archivo << "digraph BST {\n";
        archivo << "node [shape=circle];\n";
        preorden(raiz, archivo);
        archivo << "}\n";
        archivo.close();
    }

    void generarDotInorden(const std::string& nombreArchivo) {
        std::ofstream archivo(nombreArchivo);
        archivo << "digraph BST {\n";
        archivo << "node [shape=circle];\n";
        inorden(raiz, archivo);
        archivo << "}\n";
        archivo.close();
    }

    void generarDotPostorden(const std::string& nombreArchivo) {
        std::ofstream archivo(nombreArchivo);
        archivo << "digraph BST {\n";
        archivo << "node [shape=circle];\n";
        postorden(raiz, archivo);
        archivo << "}\n";
        archivo.close();
    }
};
