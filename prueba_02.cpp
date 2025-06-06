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


