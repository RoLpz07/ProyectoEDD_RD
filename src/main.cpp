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
    