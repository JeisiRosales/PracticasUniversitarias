/*
EXAMEN:
Asignatura: Algoritmos y Estructuras de Datos III 
Docente: MSc. Karen López 
Institucion: Universidad de Oriente

Ejercicio 1. 
Se necesita desarrollar un programa en C++ que permita determinar si una palabra ingresada por 
el usuario es un palíndromo. Un palíndromo es una palabra que se lee igual de izquierda a derecha 
que de derecha a izquierda, es decir, una palabra que se mantiene igual al invertirla. 
Se pide diseñar el programa orientado a objetos en C++ que utilice una pila y una cola para 
verificar si la palabra ingresada por el usuario es un palíndromo. Implementa programación 
modular (con funciones) para realizar cada operación y organiza la lógica del programa de manera 
clara y eficiente. 

REALIZADO POR: JEISI ROSALES
*/
#include <iostream>
#include <string>
using namespace std;

//Lista
struct Nodo {
    char dato;
    Nodo* siguiente;
};

// Clase pila
class Pila {
    private:
        Nodo* tope;
    public:
        Pila() : tope(NULL) {}

        void push(char valor){
            Nodo* nuevo = new Nodo();
            nuevo->dato = valor;
            nuevo->siguiente = tope;
            tope = nuevo;
        }
        char pop(){
            if (tope == NULL)
            {
                cout << "ERROR: Pila vacía..." << endl;
            }
            char valor = tope->dato;
            Nodo* temp = tope;
            tope = tope->siguiente;
            delete temp;
            return valor;
        }
        bool vacia(){
            return tope == NULL;
        }
};

// Clase cola
class Cola {
private:
    Nodo* frente;
    Nodo* fin;
public:
    Cola() : frente(NULL), fin(NULL) {}
    void agregar_elem(char valor) {
        Nodo* nuevo = new Nodo();
        nuevo->dato = valor;
        nuevo->siguiente = NULL;
        if (frente == NULL) {
            frente = fin = nuevo;
        } else {
            fin->siguiente = nuevo;
            fin = nuevo;
        }
    }
    char remover_elem() {
        if (frente == NULL)
        {
            cout << "ERROR: Cola vacía..." << endl;
        }
        char valor = frente->dato;
        Nodo* temp = frente;
        frente = frente->siguiente;
        if (frente == NULL)
        {
            fin = NULL;
        }
        delete temp;
        return valor;
    }
    bool vacia(){
        return frente == NULL;
    }
};

//Funciones
bool esPalindromo(const string& palabra) {
    Pila pila;
    Cola cola;
    for (char c : palabra) {
        pila.push(c);
        cola.agregar_elem(c);
    }
    while (!pila.vacia() && !cola.vacia()) {
        if (pila.pop() != cola.remover_elem()) {
            return false;
        }
    }
    return true;
}

//Main
int main(){
    string palabra;
    int op;
    cout << "Este programa indica si una palabra es palindromo o no";
    do
    {
        cout << "\nMENU\n1. Ingresar palabra\n2. Salir\nEscoga una opcion: ";
        cin >> op;
        switch (op)
        {
        case 1:
            cout << "Ingrese una palabra: ";
            cin >> palabra;
            if (esPalindromo(palabra)) {
                cout << "La palabra \"" << palabra << "\" es un palíndromo." << endl;
            } else {
                cout << "La palabra \"" << palabra << "\" no es un palíndromo." << endl;
            }
            break;
        case 2: cout << "Adios..."; break;
        

        default: cout << "ERROR: Opcion invalida..."; break;
        }
    } while (op != 2);
}
