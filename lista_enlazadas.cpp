using namespace std;

// ESTRUCTURA 1: Lista Enlazada – Gestor de Procesos

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

void eliminarProcesoPorID(int id) {
    Proceso* actual = inicio;
    Proceso* anterior = nullptr;
    while (actual != nullptr && actual->id != id) {
        anterior = actual;
        actual = actual->siguiente;
    }

    if (actual == nullptr) return;
    if (anterior == nullptr) {
        inicio = actual->siguiente;
    } else {
        anterior->siguiente = actual->siguiente;
    }
    delete actual;
}

Proceso* buscarProceso(int id) {
    Proceso* actual = inicio;
    while (actual != nullptr) {
        if (actual->id == id) return actual;
        actual = actual->siguiente;
    }
    return nullptr;
}

void modificarPrioridad(int id, int nuevaPrioridad) {
    Proceso* p = buscarProceso(id);
    if (p != nullptr) p->prioridad = nuevaPrioridad;
}
