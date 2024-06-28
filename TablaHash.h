#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <string>
#include <iostream>
#include "Piloto.h"

class HashNode {
public:
    Piloto* piloto;
    HashNode* siguiente;

    HashNode(Piloto* piloto) : piloto(piloto), siguiente(nullptr) {}
};

class HashTable {
private:
    int capacidad;
    int numElementos;
    HashNode** tabla;

    int hash(std::string llave) {
        int suma = 0;
        for (char c : llave) {
            suma += c;
        }
        return suma % capacidad;
    }

    void expandir() {
        int nuevaCapacidad = capacidad * 2;
        HashNode** nuevaTabla = new HashNode*[nuevaCapacidad]();
        for (int i = 0; i < capacidad; i++) {
            HashNode* nodo = tabla[i];
            while (nodo != nullptr) {
                int nuevaPos = hash(nodo->piloto->numero_de_id) % nuevaCapacidad;
                HashNode* siguiente = nodo->siguiente;
                nodo->siguiente = nuevaTabla[nuevaPos];
                nuevaTabla[nuevaPos] = nodo;
                nodo = siguiente;
            }
        }
        delete[] tabla;
        tabla = nuevaTabla;
        capacidad = nuevaCapacidad;
    }

public:
    HashTable(int tamano = 37) : capacidad(tamano), numElementos(0) {
        tabla = new HashNode*[capacidad]();
    }

    ~HashTable() {
        for (int i = 0; i < capacidad; i++) {
            HashNode* nodo = tabla[i];
            while (nodo != nullptr) {
                HashNode* temp = nodo;
                nodo = nodo->siguiente;
                delete temp->piloto;
                delete temp;
            }
        }
        delete[] tabla;
    }

    void insertar(Piloto* piloto) {
        if (numElementos >= capacidad * 0.75) {
            expandir();
        }

        int pos = hash(piloto->numero_de_id);
        HashNode* nuevoNodo = new HashNode(piloto);
        nuevoNodo->siguiente = tabla[pos];
        tabla[pos] = nuevoNodo;
        numElementos++;
    }

    Piloto* buscar(std::string numero_de_id) {
        int pos = hash(numero_de_id);
        HashNode* nodo = tabla[pos];
        while (nodo != nullptr) {
            if (nodo->piloto->numero_de_id == numero_de_id) {
                return nodo->piloto;
            }
            nodo = nodo->siguiente;
        }
        return nullptr;
    }

    void eliminar(std::string numero_de_id) {
        int pos = hash(numero_de_id);
        HashNode* nodo = tabla[pos];
        HashNode* previo = nullptr;
        while (nodo != nullptr && nodo->piloto->numero_de_id != numero_de_id) {
            previo = nodo;
            nodo = nodo->siguiente;
        }
        if (nodo == nullptr) {
            return;
        }
        if (previo == nullptr) {
            tabla[pos] = nodo->siguiente;
        } else {
            previo->siguiente = nodo->siguiente;
        }
        delete nodo->piloto;
        delete nodo;
        numElementos--;
    }

    void mostrar() {
        for (int i = 0; i < capacidad; i++) {
            HashNode* nodo = tabla[i];
            if (nodo != nullptr) {
                std::cout << "Posición " << i << ": ";
                while (nodo != nullptr) {
                    std::cout << nodo->piloto->nombre << " (" << nodo->piloto->numero_de_id << ") -> ";
                    nodo = nodo->siguiente;
                }
                std::cout << "nullptr" << std::endl;
            }
        }
    }
};

#endif
