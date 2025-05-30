//Eduardo Sebastián Rojas Rivas
#include <iostream>
using namespace std;

#define MAX 5

struct Nodo {
    int id;
    string proceso;
    int priority;
};

Nodo cola[MAX];
int frente = -1;
int final = -1;

bool estaVacia() {
    return frente == -1;
}

bool estaLlena() {
    return final == MAX - 1;
}

void encolarCPU(Nodo proceso) {
    if (estaLlena()) {
        cout << "La cola del CPU está llena\n";
    } else {
        if (frente == -1) frente = 0;
        final++;
        cola[final] = proceso;
        cout << "Proceso: " << proceso.proceso << " encolado.\n\n";
    }
}

void desencolarCPU() {
    if (estaVacia()) {
        cout << "La cola del CPU está vacía\n";
    } else {
        cout << "Proceso: " << cola[frente].proceso << " desencolado.\n";
        frente++;
        if (frente > final) frente = final = -1;
    }
}

void showCola() {
    if (estaVacia()) {
        cout << "La cola del CPU está vacía.\n";
    } else {
        cout << "Procesos en la cola del CPU:\n";
        for (int i = frente; i <= final; i++) {
            cout << "Proceso: " << cola[i].proceso << "\n";
            cout << "Prioridad: " << cola[i].priority << "\n\n";
        }
    }
}
