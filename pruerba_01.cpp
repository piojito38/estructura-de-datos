#include <iostream>
using namespace std;

#define MAX 5

// ====== Estructura para lista enlazada ======
struct Proceso {
    int id;
    string nombre;
    int prioridad;
    Proceso* siguiente;
};

Proceso* inicio = nullptr;

void insertarProceso(int id, string nombre, int prioridad) {
    Proceso* nuevo = new Proceso{id, nombre, prioridad, nullptr};
    if (inicio == nullptr) {
        inicio = nuevo;
    } else {
        Proceso* actual = inicio;
        while (actual->siguiente != nullptr) {
            actual = actual->siguiente;
        }
        actual->siguiente = nuevo;
    }
}

// Buscar proceso por ID
Proceso* buscarProceso(int id) {
    Proceso* actual = inicio;
    while (actual != nullptr) {
        if (actual->id == id) return actual;
        actual = actual->siguiente;
    }
    return nullptr;
}

// ====== Estructura de la cola ======
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

// ====== Pila para gestión de memoria ======
struct BloqueMemoria {
    int idProceso;
    int tamaño;
    BloqueMemoria* siguiente;
};

class PilaMemoria {
private:
    BloqueMemoria* cima;

public:
    PilaMemoria() {
        cima = nullptr;
    }

    void asignarMemoria(int id, int tam) {
        BloqueMemoria* nuevo = new BloqueMemoria;
        nuevo->idProceso = id;
        nuevo->tamaño = tam;
        nuevo->siguiente = cima;
        cima = nuevo;
        cout << "Memoria asignada al proceso " << id << " (" << tam << " MB)\n";
    }

    void liberarMemoria() {
        if (cima == nullptr) {
            cout << "No hay memoria para liberar.\n";
            return;
        }
        BloqueMemoria* temp = cima;
        cout << "Liberando memoria del proceso " << cima->idProceso << " (" << cima->tamaño << " MB)\n";
        cima = cima->siguiente;
        delete temp;
    }

    void mostrarMemoria() {
        if (cima == nullptr) {
            cout << "Memoria vacía.\n";
            return;
        }
        cout << "Bloques de memoria actuales:\n";
        BloqueMemoria* actual = cima;
        while (actual != nullptr) {
            cout << "Proceso ID: " << actual->idProceso << ", Tamaño: " << actual->tamaño << " MB\n";
            actual = actual->siguiente;
        }
    }

    ~PilaMemoria() {
        while (cima != nullptr) {
            liberarMemoria();
        }
    }
};
