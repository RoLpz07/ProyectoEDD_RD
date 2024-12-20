#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

struct Person {
    int id;
    string name;
    string last_name;
    char gender;
    int age;
    int id_father;
    bool is_dead;
    bool was_king;
    bool is_king;
    Person* left;
    Person* right;

    Person(int _id, const string& _name, const string& _last_name, char _gender, int _age,
           int _id_father, bool _is_dead, bool _was_king, bool _is_king)
        : id(_id), name(_name), last_name(_last_name), gender(_gender), age(_age),
          id_father(_id_father), is_dead(_is_dead), was_king(_was_king),
          is_king(_is_king), left(nullptr), right(nullptr) {}
};

class RoyalFamilyTree {
private:
    Person* root;

    bool insert(Person*& current, Person* newPerson) {
        if (current == nullptr) {
            current = newPerson;
            cout << "Inserted at root: " << newPerson->name << " " << newPerson->last_name << endl;
            return true;
        }

        if (current->id == newPerson->id_father) {
            if (current->left == nullptr) {
                current->left = newPerson;
                cout << "Inserted as left child of " << current->name << " " << current->last_name << ": " << newPerson->name << " " << newPerson->last_name << endl;
            } else {
                Person* child = current->left;
                while (child->right != nullptr) {
                    child = child->right;
                }
                child->right = newPerson;
                cout << "Inserted as right child of " << current->name << " " << current->last_name << ": " << newPerson->name << " " << newPerson->last_name << endl;
            }
            return true;
        }

        bool inserted = false;
        if (current->left != nullptr) {
            inserted = insert(current->left, newPerson);
        }
        if (!inserted && current->right != nullptr) {
            inserted = insert(current->right, newPerson);
        }

        return inserted;
    }

public:
    RoyalFamilyTree() : root(nullptr) {}

    ~RoyalFamilyTree() {
        deleteTree(root);
    }

    void loadFromCSV(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Error al abrir el archivo." << endl;
            return;
        }

        string line;
        getline(file, line); // Leer la cabecera

        int count = 0;
        while (getline(file, line)) {
            stringstream ss(line);
            string field;

            int id, age, id_father;
            char gender;
            bool is_dead, was_king, is_king;
            string name, last_name;

            getline(ss, field, ',');
            id = stoi(field);
            getline(ss, name, ',');
            getline(ss, last_name, ',');
            getline(ss, field, ',');
            gender = field[0];
            getline(ss, field, ',');
            age = stoi(field);
            getline(ss, field, ',');
            id_father = stoi(field);
            getline(ss, field, ',');
            is_dead = stoi(field);
            getline(ss, field, ',');
            was_king = stoi(field);
            getline(ss, field, ',');
            is_king = stoi(field);

            Person* newPerson = new Person(id, name, last_name, gender, age, id_father, is_dead, was_king, is_king);
            cout << "Insertando: " << newPerson->name << " " << newPerson->last_name << " (ID: " << newPerson->id << ", is_dead: " << newPerson->is_dead << ")" << endl;
            insert(root, newPerson);
            count++;
        }

        cout << "Total members inserted: " << count << endl;
        file.close();
    }

    void showLivingMembers() {
        cout << "Lista de familiares vivos:" << endl;
        showLivingMembers(root);
    }

    void showLivingMembers(Person* current) {
        if (current == nullptr) return;
        if (!current->is_dead) {
            cout << current->name << " " << current->last_name << " (ID: " << current->id << ")" << endl;
        }
        showLivingMembers(current->left);
        showLivingMembers(current->right);
    }

    void showCurrentKingAndSuccessor() {
        Person* currentKing = findCurrentKing(root);
        if (currentKing == nullptr) {
            cout << "No hay rey actualmente." << endl;
            return;
        }
        cout << "Rey actual: " << currentKing->name << " " << currentKing->last_name << endl;
        Person* successor = findSuccessor(currentKing->left);
        if (!successor) successor = findSuccessor(currentKing->right);
        if (successor) {
            cout << "Sucesor: " << successor->name << " " << successor->last_name << endl;
        } else {
            cout << "No se encontró sucesor válido en la rama actual." << endl;
        }
    }

    void assassinateKing() {
        Person* currentKing = findCurrentKing(root);
        if (currentKing == nullptr) {
            cout << "No hay rey actual." << endl;
            return;
        }
        currentKing->is_dead = true;
        cout << "El rey " << currentKing->name << " " << currentKing->last_name << " ha sido asesinado." << endl;
        assignNewKing();
    }

    void assignNewKing() {
        Person* currentKing = findCurrentKing(root);
        if (currentKing == nullptr) {
            cout << "No hay rey actual." << endl;
            return;
        }

        if (currentKing->is_dead || currentKing->age > 70) {
            currentKing->is_king = false;
            Person* successor = findSuccessor(currentKing->left);
            if (!successor) successor = findSuccessor(currentKing->right);
            if (successor) {
                successor->is_king = true;
                cout << "Nuevo rey asignado: " << successor->name << " " << successor->last_name << endl;

                Person* nextSuccessor = findSuccessor(successor->left);
                if (!nextSuccessor) nextSuccessor = findSuccessor(successor->right);
                if (nextSuccessor) {
                    cout << "Siguiente sucesor: " << nextSuccessor->name << " " << nextSuccessor->last_name << endl;
                } else {
                    cout << "No se encontró siguiente sucesor válido." << endl;
                }
            } else {
                Person* uncle = findUncle(currentKing);
                if (uncle) {
                    successor = findSuccessor(uncle->left);
                    if (!successor) successor = findSuccessor(uncle->right);
                    if (successor) {
                        successor->is_king = true;
                        cout << "Nuevo rey asignado: " << successor->name << " " << successor->last_name << endl;

                        Person* nextSuccessor = findSuccessor(successor->left);
                        if (!nextSuccessor) nextSuccessor = findSuccessor(successor->right);
                        if (nextSuccessor) {
                            cout << "Siguiente sucesor: " << nextSuccessor->name << " " << nextSuccessor->last_name << endl;
                        } else {
                            cout << "No se encontró siguiente sucesor válido." << endl;
                        }
                    } else if (!uncle->is_dead) {
                        uncle->is_king = true;
                        cout << "Nuevo rey asignado: " << uncle->name << " " << uncle->last_name << endl;

                        Person* nextSuccessor = findSuccessor(uncle->left);
                        if (!nextSuccessor) nextSuccessor = findSuccessor(uncle->right);
                        if (nextSuccessor) {
                            cout << "Siguiente sucesor: " << nextSuccessor->name << " " << nextSuccessor->last_name << endl;
                        } else {
                            cout << "No se encontró siguiente sucesor válido." << endl;
                        }
                    } else {
                        Person* ancestor = findAncestorWithTwoChildren(currentKing);
                        if (ancestor) {
                            successor = findSuccessor(ancestor->left);
                            if (!successor) successor = findSuccessor(ancestor->right);
                            if (successor) {
                                successor->is_king = true;
                                cout << "Nuevo rey asignado: " << successor->name << " " << successor->last_name << endl;

                                Person* nextSuccessor = findSuccessor(successor->left);
                                if (!nextSuccessor) nextSuccessor = findSuccessor(successor->right);
                                if (nextSuccessor) {
                                    cout << "Siguiente sucesor: " << nextSuccessor->name << " " << nextSuccessor->last_name << endl;
                                } else {
                                    cout << "No se encontró siguiente sucesor válido." << endl;
                                }
                            } else {
                                cout << "No se encontró sucesor válido." << endl;
                            }
                        } else {
                            cout << "No se encontró sucesor válido." << endl;
                        }
                    }
                } else {
                    cout << "No se encontró sucesor válido." << endl;
                }
            }
        } else {
            cout << "El rey actual está vivo: " << currentKing->name << " " << currentKing->last_name << endl;
        }
    }

    void printAllMembers() {
        cout << "Lista de todos los miembros de la familia real:" << endl;
        printAllMembers(root);
    }

    void printAllMembers(Person* current) {
        if (current == nullptr) return;
        cout << "ID: " << current->id << ", Nombre: " << current->name << " " << current->last_name
             << ", Género: " << current->gender << ", Edad: " << current->age
             << ", ID Padre: " << current->id_father << ", Muerto: " << current->is_dead
             << ", Fue Rey: " << current->was_king << ", Es Rey: " << current->is_king << endl;
        printAllMembers(current->left);
        printAllMembers(current->right);
    }

    void updatePerson(int id, const string& name, const string& last_name, char gender, int age, bool is_dead, bool was_king, bool is_king) {
        Person* person = findPersonById(root, id);
        if (person == nullptr) {
            cout << "Persona con ID " << id << " no encontrada." << endl;
            return;
        }

        person->name = name;
        person->last_name = last_name;
        person->gender = gender;
        person->age = age;
        person->is_dead = is_dead;
        person->was_king = was_king;
        person->is_king = is_king;

        cout << "Datos actualizados para la persona con ID " << id << ":" << endl;
        cout << "Nombre: " << person->name << " " << person->last_name << endl;
        cout << "Género: " << person->gender << endl;
        cout << "Edad: " << person->age << endl;
        cout << "Muerto: " << person->is_dead << endl;
        cout << "Fue Rey: " << person->was_king << endl;
        cout << "Es Rey: " << person->is_king << endl;
    }

private:
    void deleteTree(Person* current) {
        if (current == nullptr) return;
        deleteTree(current->left);
        deleteTree(current->right);
        delete current;
    }

    Person* findCurrentKing(Person* current) {
        if (current == nullptr) return nullptr;
        if (current->is_king) return current;
        Person* king = findCurrentKing(current->left);
        if (king == nullptr) king = findCurrentKing(current->right);
        return king;
    }

    Person* findSuccessor(Person* current) {
        if (current == nullptr) return nullptr;
        if (!current->is_dead && current->gender == 'H') return current;
        Person* successor = findSuccessor(current->left);
        if (successor == nullptr) successor = findSuccessor(current->right);
        return successor;
    }

    Person* findUncle(Person* current) {
        if (current == nullptr || current->id_father == 0) return nullptr;
        Person* father = findPersonById(root, current->id_father);
        if (father == nullptr || father->id_father == 0) return nullptr;
        Person* grandfather = findPersonById(root, father->id_father);
        if (grandfather == nullptr) return nullptr;
        if (grandfather->left && grandfather->left != father) return grandfather->left;
        if (grandfather->right && grandfather->right != father) return grandfather->right;
        return nullptr;
    }

    Person* findAncestorWithTwoChildren(Person* current) {
        if (current == nullptr || current->id_father == 0) return nullptr;
        Person* ancestor = findPersonById(root, current->id_father);
        while (ancestor != nullptr) {
            if (ancestor->left && ancestor->right) return ancestor;
            ancestor = findPersonById(root, ancestor->id_father);
        }
        return nullptr;
    }

    Person* findPersonById(Person* current, int id) {
        if (current == nullptr) return nullptr;
        if (current->id == id) return current;
        Person* found = findPersonById(current->left, id);
        if (found == nullptr) found = findPersonById(current->right, id);
        return found;
    }
};

int main() {
    RoyalFamilyTree tree;
    tree.loadFromCSV("royal_family.csv");

    int choice;
    do {
        cout << "\nMenú:\n";
        cout << "1. Imprimir linea de sucesion actual \n";
        cout << "2. Imprimir rey actual y sucesor\n";
        cout << "3. Asesinar al rey y pasar corona\n";
        cout << "4. Imprimir todos los miembros\n";
        cout << "5. Actualizar datos de una persona\n";
        cout << "6. Salir\n";
        cout << "Elija una opción: ";
        cin >> choice;

        switch (choice) {
            case 1:
                tree.showLivingMembers();
                break;
            case 2:
                tree.showCurrentKingAndSuccessor();
                break;
            case 3:
                tree.assassinateKing();
                break;
            case 4:
                tree.printAllMembers();
                break;
            case 5: {
                int id;
                string name, last_name;
                char gender;
                int age;
                bool is_dead, was_king, is_king;

                cout << "Ingrese el ID de la persona a actualizar: ";
                cin >> id;
                cout << "Ingrese el nuevo nombre: ";
                cin >> name;
                cout << "Ingrese el nuevo apellido: ";
                cin >> last_name;
                cout << "Ingrese el nuevo género (H/M): ";
                cin >> gender;
                cout << "Ingrese la nueva edad: ";
                cin >> age;
                cout << "¿Está muerto? (1 para sí, 0 para no): ";
                cin >> is_dead;
                cout << "¿Fue rey? (1 para sí, 0 para no): ";
                cin >> was_king;
                cout << "¿Es rey? (1 para sí, 0 para no): ";
                cin >> is_king;

                tree.updatePerson(id, name, last_name, gender, age, is_dead, was_king, is_king);
                break;
            }
            case 6:
                cout << "Saliendo..." << endl;
                break;
            default:
                cout << "Opción no válida. Intente de nuevo." << endl;
        }
    } while (choice != 6);

    return 0;
}