#ifndef ARBOL_B_H
#define ARBOL_B_H

#include <iostream>
#include <fstream>
#include <map>
#include "Avion.h"

class NodoB {
public:
    bool hoja;
    int n;
    std::map<std::string, Avion*> llaves;
    std::map<int, NodoB*> hijos;

    NodoB(bool hoja) : hoja(hoja), n(0) {}
};

class ArbolB {
public:
    NodoB* raiz;

    ArbolB() : raiz(nullptr) {}

    void insertar(Avion* avion) {
        if (!raiz) {
            raiz = new NodoB(true);
            raiz->llaves[avion->numero_de_registro] = avion;
            raiz->n = 1;
        } else {
            if (raiz->n == 9) { // Si la raíz está llena
                NodoB* nuevaRaiz = new NodoB(false);
                nuevaRaiz->hijos[0] = raiz;
                dividirHijo(nuevaRaiz, 0, raiz);
                insertarNoLleno(nuevaRaiz, avion);
                raiz = nuevaRaiz;
            } else {
                insertarNoLleno(raiz, avion);
            }
        }
    }

    void dividirHijo(NodoB* nodo, int i, NodoB* y) {
        NodoB* z = new NodoB(y->hoja);
        z->n = 4;
        auto it = y->llaves.begin();
        std::advance(it, 5);
        for (int j = 0; j < 4; ++j) {
            z->llaves[it->first] = it->second;
            it = y->llaves.erase(it);
        }
        if (!y->hoja) {
            auto it_h = y->hijos.begin();
            std::advance(it_h, 5);
            for (int j = 0; j < 5; ++j) {
                z->hijos[j] = it_h->second;
                it_h = y->hijos.erase(it_h);
            }
        }
        y->n = 4;
        nodo->hijos[i + 1] = z;
        nodo->llaves[y->llaves.begin()->first] = y->llaves.begin()->second;
        nodo->n++;
    }

    void insertarNoLleno(NodoB* nodo, Avion* avion) {
        if (nodo->hoja) {
            nodo->llaves[avion->numero_de_registro] = avion;
            nodo->n++;
        } else {
            auto it = nodo->llaves.upper_bound(avion->numero_de_registro);
            int i = std::distance(nodo->llaves.begin(), it);
            if (nodo->hijos[i]->n == 9) {
                dividirHijo(nodo, i, nodo->hijos[i]);
                if (avion->numero_de_registro > nodo->llaves.begin()->first) {
                    i++;
                }
            }
            insertarNoLleno(nodo->hijos[i], avion);
        }
    }

    Avion* buscar(const std::string& numeroRegistro) {
        return buscarEnNodo(raiz, numeroRegistro);
    }

    Avion* buscarEnNodo(NodoB* nodo, const std::string& numeroRegistro) {
        auto it = nodo->llaves.find(numeroRegistro);
        if (it != nodo->llaves.end()) {
            return it->second;
        }
        if (nodo->hoja) {
            return nullptr;
        }
        auto it_h = nodo->llaves.upper_bound(numeroRegistro);
        int i = std::distance(nodo->llaves.begin(), it_h);
        return buscarEnNodo(nodo->hijos[i], numeroRegistro);
    }

    void imprimir() {
        imprimirNodo(raiz);
    }

private:
    void imprimirNodo(NodoB* nodo) {
        if (!nodo) return;
        for (const auto& llave : nodo->llaves) {
            std::cout << "Avion: " << llave.second->numero_de_registro << ", Modelo: " << llave.second->modelo << ", Capacidad: " << llave.second->capacidad << std::endl;
        }
        if (!nodo->hoja) {
            for (auto& hijo : nodo->hijos) {
                imprimirNodo(hijo.second);
            }
        }
    }
};

#endif // ARBABOL_B_H