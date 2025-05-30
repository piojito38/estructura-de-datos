//Eduardo Sebastián Rojas Rivas
#include <iostream>
using namespace std;

struct NodoCola {
    Proceso* proceso;  // Apunta a un proceso existente
    NodoCola* siguiente;
};

NodoCola* frente = nullptr;
NodoCola* fin = nullptr;

// Encolar proceso existente
void encolarProceso(Proceso* p) {
    NodoCola* nuevo = new NodoCola{p, nullptr};
    if (fin == nullptr) {
        frente = fin = nuevo;
    } else {
        fin->siguiente = nuevo;
        fin = nuevo;
    }
}

// Desencolar proceso
Proceso* desencolarProceso() {
    if (frente == nullptr) return nullptr;

    NodoCola* temp = frente;
    Proceso* p = temp->proceso;
    frente = frente->siguiente;

    if (frente == nullptr) fin = nullptr; // Si se vació la cola
    delete temp;
    return p;
}

// Mostrar procesos en la cola
void mostrarCola() {
    NodoCola* actual = frente;
    cout << "Procesos en cola:\n";
    while (actual != nullptr) {
        cout << "ID: " << actual->proceso->id 
             << ", Nombre: " << actual->proceso->nombre 
             << ", Prioridad: " << actual->proceso->prioridad << "\n";
        actual = actual->siguiente;
    }
}
