#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <limits>


// Estructura para un proceso
struct Process {
    int id;
    std::string name;
    int priority;
    std::string state;
};

// Nodo para lista enlazada de procesos registrados
struct ProcessNode {
    Process data;
    ProcessNode* next;
    ProcessNode(const Process& p) : data(p), next(nullptr) {}
};

// Gestor de Procesos: lista enlazada simple
class ProcessList {
private:
    ProcessNode* head;
public:
    ProcessList() : head(nullptr) {}

    // Insertar nuevo proceso al final
    void insertProcess(const Process& p) {
        ProcessNode* newNode = new ProcessNode(p);
        if (!head) {
            head = newNode;
        } else {
            ProcessNode* temp = head;
            while (temp->next) temp = temp->next;
            temp->next = newNode;
        }
    }

    // Eliminar proceso por ID
    bool removeProcess(int id) {
        if (!head) return false;
        if (head->data.id == id) {
            ProcessNode* toDelete = head;
            head = head->next;
            delete toDelete;
            return true;
        }
        ProcessNode* prev = head;
        ProcessNode* cur = head->next;
        while (cur) {
            if (cur->data.id == id) {
                prev->next = cur->next;
                delete cur;
                return true;
            }
            prev = cur;
            cur = cur->next;
        }
        return false;
    }

    // Buscar proceso por ID o nombre (busca ID primero)
    ProcessNode* searchById(int id) {
        ProcessNode* temp = head;
        while (temp) {
            if (temp->data.id == id) return temp;
            temp = temp->next;
        }
        return nullptr;
    }
    ProcessNode* searchByName(const std::string& name) {
        ProcessNode* temp = head;
        while (temp) {
            if (temp->data.name == name) return temp;
            temp = temp->next;
        }
        return nullptr;
    }

    // Modificar prioridad de un proceso por ID
    bool modifyPriority(int id, int newPriority) {
        ProcessNode* node = searchById(id);
        if (!node) return false;
        node->data.priority = newPriority;
        return true;
    }

    // Mostrar todos los procesos
    void displayAll() {
        if (!head) {
            std::cout << "No hay procesos registrados.\n";
            return;
        }
        ProcessNode* temp = head;
        std::cout << "ID\tNombre\tPrioridad\tEstado\n";
        while (temp) {
            std::cout << temp->data.id << "\t"
                      << temp->data.name << "\t"
                      << temp->data.priority << "\t\t"
                      << temp->data.state << "\n";
            temp = temp->next;
        }
    }

    // Persistencia: guardar a archivo
    void saveToFile(const std::string& filename) {
        std::ofstream file(filename);
        ProcessNode* temp = head;
        while (temp) {
            file << temp->data.id << ","
                 << temp->data.name << ","
                 << temp->data.priority << ","
                 << temp->data.state << "\n";
            temp = temp->next;
        }
        file.close();
    }

    // Cargar desde archivo (reinicia la lista)
    void loadFromFile(const std::string& filename) {
        // Liberar lista anterior
        while (head) {
            ProcessNode* toDelete = head;
            head = head->next;
            delete toDelete;
        }
        std::ifstream file(filename);
        if (!file.is_open()) return;
        std::string line;
        while (std::getline(file, line)) {
            std::stringstream ss(line);
            Process p;
            std::string token;
            std::getline(ss, token, ',');
            p.id = std::stoi(token);
            std::getline(ss, p.name, ',');
            std::getline(ss, token, ',');
            p.priority = std::stoi(token);
            std::getline(ss, p.state, ',');
            insertProcess(p);
        }
        file.close();
    }
};

// Nodo para cola de prioridad del planificador de CPU
struct QueueNode {
    Process data;
    QueueNode* next;
    QueueNode(const Process& p) : data(p), next(nullptr) {}
};

// Cola de prioridad: se inserta en orden descendente de prioridad (mayor prioridad primero)
class PriorityQueue {
private:
    QueueNode* front;
public:
    PriorityQueue() : front(nullptr) {}

    // Encolar según prioridad
    void enqueue(const Process& p) {
        QueueNode* newNode = new QueueNode(p);
        if (!front || p.priority > front->data.priority) {
            newNode->next = front;
            front = newNode;
        } else {
            QueueNode* temp = front;
            while (temp->next && temp->next->data.priority >= p.priority) {
                temp = temp->next;
            }
            newNode->next = temp->next;
            temp->next = newNode;
        }
    }

    // Desencolar (ejecutar) el de mayor prioridad
    bool dequeue(Process& p) {
        if (!front) return false;
        QueueNode* toDelete = front;
        p = front->data;
        front = front->next;
        delete toDelete;
        return true;
    }

    // Ver cola actual
    void displayQueue() {
        if (!front) {
            std::cout << "La cola de prioridad está vacía.\n";
            return;
        }
        std::cout << "ID\tNombre\tPrioridad\tEstado\n";
        QueueNode* temp = front;
        while (temp) {
            std::cout << temp->data.id << "\t"
                      << temp->data.name << "\t"
                      << temp->data.priority << "\t\t"
                      << temp->data.state << "\n";
            temp = temp->next;
        }
    }

    // Persistencia: guardar a archivo
    void saveToFile(const std::string& filename) {
        std::ofstream file(filename);
        QueueNode* temp = front;
        while (temp) {
            file << temp->data.id << ","
                 << temp->data.name << ","
                 << temp->data.priority << ","
                 << temp->data.state << "\n";
            temp = temp->next;
        }
        file.close();
    }

    // Cargar desde archivo (reinicia la cola)
    void loadFromFile(const std::string& filename) {
        // Liberar cola anterior
        while (front) {
            QueueNode* toDelete = front;
            front = front->next;
            delete toDelete;
        }
        std::ifstream file(filename);
        if (!file.is_open()) return;
        std::string line;
        while (std::getline(file, line)) {
            std::stringstream ss(line);
            Process p;
            std::string token;
            std::getline(ss, token, ',');
            p.id = std::stoi(token);
            std::getline(ss, p.name, ',');
            std::getline(ss, token, ',');
            p.priority = std::stoi(token);
            std::getline(ss, p.state, ',');
            enqueue(p);
        }
        file.close();
    }
};

// Estructura para bloque de memoria
struct MemoryBlock {
    int blockId;
    int processId;
    int size;
};

// Nodo para pila de memoria
struct StackNode {
    MemoryBlock data;
    StackNode* next;
    StackNode(const MemoryBlock& mb) : data(mb), next(nullptr) {}
};

// Gestor de Memoria: pila simple LIFO
class MemoryStack {
private:
    StackNode* topNode;
    int nextBlockId;
public:
    MemoryStack() : topNode(nullptr), nextBlockId(1) {}

    // Push: asignar bloque al proceso
    void push(int procId, int size) {
        MemoryBlock mb;
        mb.blockId = nextBlockId++;
        mb.processId = procId;
        mb.size = size;
        StackNode* newNode = new StackNode(mb);
        newNode->next = topNode;
        topNode = newNode;
        std::cout << "Bloque " << mb.blockId << " asignado al proceso " << procId << " (tamaño " << size << ").\n";
    }

    // Pop: liberar bloque superior
    bool pop() {
        if (!topNode) return false;
        StackNode* toDelete = topNode;
        std::cout << "Bloque " << toDelete->data.blockId << " (proceso " << toDelete->data.processId
                  << ", tamaño " << toDelete->data.size << ") liberado.\n";
        topNode = topNode->next;
        delete toDelete;
        return true;
    }

    // Ver estado actual de la pila
    void displayStack() {
        if (!topNode) {
            std::cout << "La pila de memoria está vacía.\n";
            return;
        }
        std::cout << "BlockID\tProcID\tTamaño\n";
        StackNode* temp = topNode;
        while (temp) {
            std::cout << temp->data.blockId << "\t"
                      << temp->data.processId << "\t"
                      << temp->data.size << "\n";
            temp = temp->next;
        }
    }

    // Persistencia: guardar a archivo
    void saveToFile(const std::string& filename) {
        std::ofstream file(filename);
        // Para guardar en orden de llegada, recolectamos en un arreglo temporal y luego invertimos
        StackNode* temp = topNode;
        // Contar nodos
        int count = 0;
        while (temp) {
            count++;
            temp = temp->next;
        }
        if (count == 0) {
            file.close();
            return;
        }
        MemoryBlock* arr = new MemoryBlock[count];
        temp = topNode;
        for (int i = 0; i < count; i++) {
            arr[i] = temp->data;
            temp = temp->next;
        }
        // Guardar invirtiendo para mantener orden de asignación
        for (int i = count - 1; i >= 0; i--) {
            file << arr[i].blockId << ","
                 << arr[i].processId << ","
                 << arr[i].size << "\n";
        }
        delete[] arr;
        file << nextBlockId << "\n"; // Guardamos el siguiente blockId para no reiniciar
        file.close();
    }

    // Cargar desde archivo (reinicia la pila)
    void loadFromFile(const std::string& filename) {
        // Liberar pila anterior
        while (topNode) {
            StackNode* toDelete = topNode;
            topNode = topNode->next;
            delete toDelete;
        }
        std::ifstream file(filename);
        if (!file.is_open()) return;
        std::string line;
        // Cargamos los bloques en un arreglo temporal primero
        MemoryBlock arr[1000]; // asumimos no más de 1000 bloques simultáneos
        int count = 0;
        while (std::getline(file, line) && !line.empty()) {
            std::stringstream ss(line);
            MemoryBlock mb;
            std::string token;
            std::getline(ss, token, ',');
            mb.blockId = std::stoi(token);
            std::getline(ss, token, ',');
            mb.processId = std::stoi(token);
            std::getline(ss, token, ',');
            mb.size = std::stoi(token);
            arr[count++] = mb;
        }
        // La última línea es nextBlockId
        if (std::getline(file, line)) {
            nextBlockId = std::stoi(line);
        }
        file.close();
        // Reconstruir pila invirtiendo orden para que el último leído quede en top
        for (int i = 0; i < count; i++) {
            StackNode* newNode = new StackNode(arr[i]);
            newNode->next = topNode;
            topNode = newNode;
        }
    }
};

// Funciones auxiliares para menú e I/O
void pause() {
    std::cout << "Presione ENTER para continuar...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

int main() {
    ProcessList procList;
    PriorityQueue cpuQueue;
    MemoryStack memStack;

    // Cargar datos persistentes (si existen)
    procList.loadFromFile("processes.txt");
    cpuQueue.loadFromFile("queue.txt");
    memStack.loadFromFile("stack.txt");

    int mainChoice;
    do {
        clearScreen();
        std::cout << "==== SISTEMA DE GESTIÓN DE PROCESOS ====\n";
        std::cout << "1. Gestor de Procesos\n";
        std::cout << "2. Planificador de CPU (Cola de Prioridad)\n";
        std::cout << "3. Gestor de Memoria (Pila)\n";
        std::cout << "4. Salir y guardar\n";
        std::cout << "Seleccione una opción: ";
        std::cin >> mainChoice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (mainChoice == 1) {
            int opt;
            do {
                clearScreen();
                std::cout << "-- Gestor de Procesos --\n";
                std::cout << "1. Registrar nuevo proceso\n";
                std::cout << "2. Eliminar proceso\n";
                std::cout << "3. Buscar proceso\n";
                std::cout << "4. Modificar prioridad\n";
                std::cout << "5. Listar todos los procesos\n";
                std::cout << "6. Volver al menú principal\n";
                std::cout << "Seleccione: ";
                std::cin >> opt;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                if (opt == 1) {
                    Process p;
                    std::cout << "Ingrese ID: ";
                    std::cin >> p.id;
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Ingrese nombre: ";
                    std::getline(std::cin, p.name);
                    std::cout << "Ingrese prioridad (número entero, mayor = más prioridad): ";
                    std::cin >> p.priority;
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Ingrese estado: ";
                    std::getline(std::cin, p.state);
                    procList.insertProcess(p);
                    std::cout << "Proceso registrado con éxito.\n";
                    pause();
                }
                else if (opt == 2) {
                    int id;
                    std::cout << "Ingrese ID del proceso a eliminar: ";
                    std::cin >> id;
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    if (procList.removeProcess(id))
                        std::cout << "Proceso eliminado.\n";
                    else
                        std::cout << "Proceso no encontrado.\n";
                    pause();
                }
                else if (opt == 3) {
                    int subOpt;
                    std::cout << "Buscar por: 1) ID  2) Nombre: ";
                    std::cin >> subOpt;
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    if (subOpt == 1) {
                        int id;
                        std::cout << "Ingrese ID: ";
                        std::cin >> id;
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        ProcessNode* node = procList.searchById(id);
                        if (node) {
                            std::cout << "ID: " << node->data.id << "\n";
                            std::cout << "Nombre: " << node->data.name << "\n";
                            std::cout << "Prioridad: " << node->data.priority << "\n";
                            std::cout << "Estado: " << node->data.state << "\n";
                        } else {
                            std::cout << "Proceso no encontrado.\n";
                        }
                    } else if (subOpt == 2) {
                        std::string name;
                        std::cout << "Ingrese nombre: ";
                        std::getline(std::cin, name);
                        ProcessNode* node = procList.searchByName(name);
                        if (node) {
                            std::cout << "ID: " << node->data.id << "\n";
                            std::cout << "Nombre: " << node->data.name << "\n";
                            std::cout << "Prioridad: " << node->data.priority << "\n";
                            std::cout << "Estado: " << node->data.state << "\n";
                        } else {
                            std::cout << "Proceso no encontrado.\n";
                        }
                    } else {
                        std::cout << "Opción inválida.\n";
                    }
                    pause();
                }
                else if (opt == 4) {
                    int id, newPrio;
                    std::cout << "Ingrese ID del proceso: ";
                    std::cin >> id;
                    std::cout << "Ingrese nueva prioridad: ";
                    std::cin >> newPrio;
                    if (procList.modifyPriority(id, newPrio))
                        std::cout << "Prioridad actualizada.\n";
                    else
                        std::cout << "Proceso no encontrado.\n";
                    pause();
                }
                else if (opt == 5) {
                    procList.displayAll();
                    pause();
                }
            } while (opt != 6);
        }
        else if (mainChoice == 2) {
            int opt;
            do {
                clearScreen();
                std::cout << "-- Planificador de CPU (Cola de Prioridad) --\n";
                std::cout << "1. Encolar proceso (según prioridad)\n";
                std::cout << "2. Desencolar (Ejecutar)\n";
                std::cout << "3. Ver cola actual\n";
                std::cout << "4. Volver al menú principal\n";
                std::cout << "Seleccione: ";
                std::cin >> opt;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                if (opt == 1) {
                    ProcessNode* node;
                    int id;
                    std::cout << "Ingrese ID del proceso a encolar: ";
                    std::cin >> id;
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    node = procList.searchById(id);
                    if (node) {
                        cpuQueue.enqueue(node->data);
                        std::cout << "Proceso encolado.\n";
                    } else {
                        std::cout << "Proceso no encontrado en el gestor de procesos.\n";
                    }
                    pause();
                }
                else if (opt == 2) {
                    Process executed;
                    if (cpuQueue.dequeue(executed)) {
                        std::cout << "Ejecutando proceso ID " << executed.id 
                                  << " (" << executed.name << ").\n";
                    } else {
                        std::cout << "La cola está vacía.\n";
                    }
                    pause();
                }
                else if (opt == 3) {
                    cpuQueue.displayQueue();
                    pause();
                }
            } while (opt != 4);
        }
        else if (mainChoice == 3) {
            int opt;
            do {
                clearScreen();
                std::cout << "-- Gestor de Memoria (Pila) --\n";
                std::cout << "1. Asignar bloque (Push)\n";
                std::cout << "2. Liberar bloque (Pop)\n";
                std::cout << "3. Ver pila de bloques\n";
                std::cout << "4. Volver al menú principal\n";
                std::cout << "Seleccione: ";
                std::cin >> opt;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                if (opt == 1) {
                    int pid, size;
                    std::cout << "Ingrese ID de proceso que recibe memoria: ";
                    std::cin >> pid;
                    std::cout << "Ingrese tamaño del bloque: ";
                    std::cin >> size;
                    memStack.push(pid, size);
                    pause();
                }
                else if (opt == 2) {
                    if (!memStack.pop()) {
                        std::cout << "La pila está vacía.\n";
                    }
                    pause();
                }
                else if (opt == 3) {
                    memStack.displayStack();
                    pause();
                }
            } while (opt != 4);
        }
        else if (mainChoice == 4) {
            // Guardar todo antes de salir
            procList.saveToFile("processes.txt");
            cpuQueue.saveToFile("queue.txt");
            memStack.saveToFile("stack.txt");
            std::cout << "Datos guardados. Saliendo...\n";
        }
    } while (mainChoice != 4);

    return 0;
}



