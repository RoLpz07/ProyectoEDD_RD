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
            }
        } else {
            insert(current->left, newPerson);
            insert(current->right, newPerson);
        }
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
    
    void showLineOfSuccession(Person* current) {
        if (current == nullptr) return;
        if (!current->is_dead) {
            cout << current->name << " " << current->last_name << " (ID: " << current->id << ")" << endl;
        }
        showLineOfSuccession(current->left);
        showLineOfSuccession(current->right);
    }

    void deleteTree(Person* current) {
        if (current == nullptr) return;
        deleteTree(current->left);
        deleteTree(current->right);
        delete current;
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
        getline(file, line);

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
            insert(root, newPerson);
        }

        file.close();
    }

    void showSuccessionLine() {
        cout << "Línea de sucesión actual:" << endl;
        showLineOfSuccession(root);
    }

    void assignNewKing() {
        Person* currentKing = findCurrentKing(root);
        if (currentKing == nullptr) {
            cout << "No hay rey actual." << endl;
            return;
        }

        if (currentKing->is_dead) {
            Person* successor = findSuccessor(currentKing->left);
            if (!successor) successor = findSuccessor(currentKing->right);
            if (successor) {
                successor->is_king = true;
                cout << "Nuevo rey asignado: " << successor->name << " " << successor->last_name << endl;
            } else {
                cout << "No se encontró sucesor válido." << endl;
            }
        } else {
            cout << "El rey actual está vivo: " << currentKing->name << " " << currentKing->last_name << endl;
        }
    }
};
