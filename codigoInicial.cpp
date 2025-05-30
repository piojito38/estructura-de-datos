// Eduardo Sebastián Rojas Rivas
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

// ====== Menú principal ======
int main() {
    int opcion;
    PilaMemoria gestorMemoria;

    do {
        cout << "\n=== SISTEMA DE GESTIÓN DE PROCESOS ===\n";
        cout << "1. Insertar proceso a lista\n";
        cout << "2. Encolar proceso a CPU\n";
        cout << "3. Desencolar proceso del CPU\n";
        cout << "4. Mostrar cola del CPU\n";
        cout << "5. Salir\n";
        cout << "6. Mostrar memoria asignada\n";
        cout << "7. Liberar último bloque de memoria\n";
        cout << "Seleccione una opción: ";
        cin >> opcion;

        if (opcion == 1) {
            int id, prioridad, tamaño;
            string nombre;
            cout << "ID del proceso: ";
            cin >> id;
            cout << "Nombre del proceso: ";
            cin >> nombre;
            cout << "Prioridad: ";
            cin >> prioridad;
            cout << "Tamaño de memoria a asignar (MB): ";
            cin >> tamaño;
            insertarProceso(id, nombre, prioridad);
            gestorMemoria.asignarMemoria(id, tamaño);
            cout << "Proceso insertado y memoria asignada.\n";
        }

        else if (opcion == 2) {
            int id;
            cout << "ID del proceso a encolar: ";
            cin >> id;
            Proceso* p = buscarProceso(id);
            if (p != nullptr) {
                Nodo nuevo = {p->id, p->nombre, p->prioridad};
                encolarCPU(nuevo);
            } else {
                cout << "Proceso no encontrado en la lista.\n";
            }
        }

        else if (opcion == 3) {
            desencolarCPU();
        }

        else if (opcion == 4) {
            showCola();
        }

        else if (opcion == 5) {
            cout << "Saliendo...\n";
        }

        else if (opcion == 6) {
            gestorMemoria.mostrarMemoria();
        }

        else if (opcion == 7) {
            gestorMemoria.liberarMemoria();
        }

        else {
            cout << "Opción no válida.\n";
        }

    } while (opcion != 5);

    return 0;
}
