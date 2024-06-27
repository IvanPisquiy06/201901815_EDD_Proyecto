#ifndef LISTA_DOBLE_H
#define LISTA_DOBLE_H

#include "Avion.h"

class NodoLista {
public:
    Avion* avion;
    NodoLista* siguiente;
    NodoLista* anterior;

    NodoLista(Avion* avion) : avion(avion), siguiente(nullptr), anterior(nullptr) {}
};

class ListaDoble {
public:
    NodoLista* cabeza;
    NodoLista* cola;

    ListaDoble() : cabeza(nullptr), cola(nullptr) {}

    void insertar(Avion* avion) {
        NodoLista* nuevo = new NodoLista(avion);
        if (!cabeza) {
            cabeza = cola = nuevo;
        } else {
            cola->siguiente = nuevo;
            nuevo->anterior = cola;
            cola = nuevo;
        }
    }

    void eliminar(const std::string& numeroRegistro) {
        NodoLista* actual = cabeza;
        while (actual) {
            if (actual->avion->numero_de_registro == numeroRegistro) {
                if (actual->anterior) actual->anterior->siguiente = actual->siguiente;
                if (actual->siguiente) actual->siguiente->anterior = actual->anterior;
                if (actual == cabeza) cabeza = actual->siguiente;
                if (actual == cola) cola = actual->anterior;
                delete actual;
                return;
            }
            actual = actual->siguiente;
        }
    }

    Avion* buscar(const std::string& numeroRegistro) {
        NodoLista* actual = cabeza;
        while (actual) {
            if (actual->avion->numero_de_registro == numeroRegistro) {
                return actual->avion;
            }
            actual = actual->siguiente;
        }
        return nullptr;
    }

    void imprimir() {
        NodoLista* actual = cabeza;
        while (actual) {
            std::cout << "Avion: " << actual->avion->numero_de_registro << ", Modelo: " << actual->avion->modelo << ", Capacidad: " << actual->avion->capacidad << std::endl;
            actual = actual->siguiente;
        }
    }
};

#endif // LISTA_DOBLE_H
