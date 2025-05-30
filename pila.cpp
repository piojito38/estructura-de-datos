#include <iostream>
using namespace std;

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

    // Asignar memoria (push)
    void asignarMemoria(int id, int tam) {
        BloqueMemoria* nuevo = new BloqueMemoria;
        nuevo->idProceso = id;
        nuevo->tamaño = tam;
        nuevo->siguiente = cima;
        cima = nuevo;
        cout << "Memoria asignada al proceso " << id << " (" << tam << " MB)\n";
    }

    // Liberar memoria (pop)
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

    // Ver estado actual de la memoria
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

int main() {
    PilaMemoria gestor;
    int opcion, id, tam;

    do {
        cout << "\n--- GESTOR DE MEMORIA ---\n";
        cout << "1. Asignar memoria a proceso\n";
        cout << "2. Liberar memoria\n";
        cout << "3. Ver estado de la memoria\n";
        cout << "0. Salir\n";
        cout << "Seleccione una opción: ";
        cin >> opcion;

        switch (opcion) {
            case 1:
                cout << "ID del proceso: ";
                cin >> id;
                cout << "Tamaño en MB: ";
                cin >> tam;
                gestor.asignarMemoria(id, tam);
                break;
            case 2:
                gestor.liberarMemoria();
                break;
            case 3:
                gestor.mostrarMemoria();
                break;
            case 0:
                cout << "Saliendo del programa.\n";
                break;
            default:
                cout << "Opción inválida.\n";
        }
    } while (opcion != 0);

    return 0;
}
