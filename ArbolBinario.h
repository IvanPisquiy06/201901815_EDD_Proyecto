#include "Piloto.h"
#include <iostream>

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

    void preorden(NodoBST* nodo) {
        if (nodo != nullptr) {
            std::cout << nodo->piloto->nombre << " - " << nodo->piloto->horas_de_vuelo << " horas" << std::endl;
            preorden(nodo->izquierda);
            preorden(nodo->derecha);
        }
    }

    void inorden(NodoBST* nodo) {
        if (nodo != nullptr) {
            inorden(nodo->izquierda);
            std::cout << nodo->piloto->nombre << " - " << nodo->piloto->horas_de_vuelo << " horas" << std::endl;
            inorden(nodo->derecha);
        }
    }

    void postorden(NodoBST* nodo) {
        if (nodo != nullptr) {
            postorden(nodo->izquierda);
            postorden(nodo->derecha);
            std::cout << nodo->piloto->nombre << " - " << nodo->piloto->horas_de_vuelo << " horas" << std::endl;
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
        preorden(raiz);
    }

    void mostrarInorden() {
        inorden(raiz);
    }

    void mostrarPostorden() {
        postorden(raiz);
    }
};