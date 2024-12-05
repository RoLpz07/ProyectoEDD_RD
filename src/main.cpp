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

    void insert(Person*& current, Person* newPerson) {
        if (current == nullptr) {
            current = newPerson;
            return;
        }
        if (current->id == newPerson->id_father) {
            if (current->left == nullptr) {
                current->left = newPerson;
            } else if (current->right == nullptr) {
                current->right = newPerson;
            } else {
                insert(current->left, newPerson);
            }
        } else {
            if (current->left != nullptr) {
                insert(current->left, newPerson);
            }
            if (current->right != nullptr) {
                insert(current->right, newPerson);
            }
        }
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

    void showAllMembers() {
        cout << "Lista de todos los familiares:" << endl;
        showAllMembers(root);
    }

    void showAllMembers(Person* current) {
        if (current == nullptr) return;
        cout << current->name << " " << current->last_name << " (ID: " << current->id << ")" << endl;
        showAllMembers(current->left);
        showAllMembers(current->right);
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

    void showDeceasedMembers() {
        cout << "Lista de familiares muertos:" << endl;
        showDeceasedMembers(root);
    }

    void showDeceasedMembers(Person* current) {
        if (current == nullptr) return;
        if (current->is_dead) {
            cout << current->name << " " << current->last_name << " (ID: " << current->id << ")" << endl;
        }
        showDeceasedMembers(current->left);
        showDeceasedMembers(current->right);
    }

    void showCurrentKingAndSuccessor() {
        Person* currentKing = findCurrentKing(root);
        if (currentKing == nullptr) {
            cout << "No hay rey actual." << endl;
            return;
        }
        cout << "Rey actual: " << currentKing->name << " " << currentKing->last_name << endl;
        Person* successor = findSuccessor(currentKing->left);
        if (!successor) successor = findSuccessor(currentKing->right);
        if (successor) {
            cout << "Sucesor: " << successor->name << " " << successor->last_name << endl;
        } else {
            cout << "No se encontró sucesor válido." << endl;
        }
    }

    void assassinateKing() {
        Person* currentKing = findCurrentKing(root);
        if (currentKing == nullptr) {
            cout << "No hay rey actual." << endl;
            return;
        }
        currentKing->is_dead = true;
        currentKing->is_king = false; // Asegurarse de que el rey actual ya no sea rey
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
            Person* successor = findSuccessor(currentKing->left);
            if (!successor) successor = findSuccessor(currentKing->right);
            if (successor) {
                successor->is_king = true;
                cout << "Nuevo rey asignado: " << successor->name << " " << successor->last_name << endl;
            } else {
                // Buscar en el árbol del hermano
                Person* uncle = findUncle(currentKing);
                if (uncle) {
                    successor = findSuccessor(uncle->left);
                    if (!successor) successor = findSuccessor(uncle->right);
                    if (successor) {
                        successor->is_king = true;
                        cout << "Nuevo rey asignado: " << successor->name << " " << successor->last_name << endl;
                    } else if (!uncle->is_dead) {
                        uncle->is_king = true;
                        cout << "Nuevo rey asignado: " << uncle->name << " " << uncle->last_name << endl;
                    } else {
                        // Buscar en el árbol del tío
                        Person* ancestor = findAncestorWithTwoChildren(currentKing);
                        if (ancestor) {
                            successor = findSuccessor(ancestor->left);
                            if (!successor) successor = findSuccessor(ancestor->right);
                            if (successor) {
                                successor->is_king = true;
                                cout << "Nuevo rey asignado: " << successor->name << " " << successor->last_name << endl;
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
        if (!current->is_dead) return current;
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
        cout << "1. Imprimir toda la lista de familiares\n";
        cout << "2. Imprimir solo los vivos\n";
        cout << "3. Imprimir solo los muertos\n";
        cout << "4. Imprimir rey actual y sucesor\n";
        cout << "5. Asesinar al rey y pasar corona\n";
        cout << "6. Salir\n";
        cout << "Elija una opción: ";
        cin >> choice;

        switch (choice) {
            case 1:
                tree.showAllMembers();
                break;
            case 2:
                tree.showLivingMembers();
                break;
            case 3:
                tree.showDeceasedMembers();
                break;
            case 4:
                tree.showCurrentKingAndSuccessor();
                break;
            case 5:
                tree.assassinateKing();
                break;
            case 6:
                cout << "Saliendo..." << endl;
                break;
            default:
                cout << "Opción no válida. Intente de nuevo." << endl;
        }
    } while (choice != 6);

    return 0;
}