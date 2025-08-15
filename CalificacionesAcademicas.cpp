/*
EXAMEN:
Asignatura: Algoritmos y Estructuras de Datos III 
Docente: MSc. Karen López 
Institucion: Universidad de Oriente

Ejercicio 2. 
Desarrolle un programa que permita el ingreso de n calificaciones en el rango de 0 a 10, los cuales 
deben ser guardados en una pila de números reales positivos. Con las operaciones básicas de pilas 
y colas escribir un programa en C++ para poner todos los elementos mayores al promedio de la 
pila en la cola. Muestre los elementos el promedio y elementos de la cola. Indique cuantos 
números estaban en la cola. 

REALIZADO POR: JEISI ROSALES
*/

#include <iostream>
#include <string>
using namespace std;

//Pila
const int MAX_PILA = 5;
int pila_calificaciones[MAX_PILA];
int tope = -1;
double promedio = 0;

//Cola
const int MAX_COLA = 5;
int cola_calificaciones[MAX_COLA];
int frente = -1;
int fin  = -1;

//Funciones
bool pila_vacia(){
    return tope == -1;
}

bool pila_llena(){
    return tope == MAX_PILA - 1;
}

bool cola_vacia(){
    return frente == -1 && fin == -1;
}

bool cola_llena(){
    return fin == MAX_COLA - 1;
}

void calcular_promedio(){
    int suma = 0;
    int num_calif = 0;
    for (int i = 0; i <= tope; i++) { 
        suma += pila_calificaciones[i];
        num_calif++;
    }
    if (num_calif > 0) {
        promedio = static_cast<double>(suma) / num_calif; 
    } else {
        promedio = 0; 
    }
}

void mover_calif_cola(){
    calcular_promedio();
    for (int i = 0; i <= tope; i++)
    {
        if (pila_calificaciones[i] > promedio)
        {
            if (!cola_llena()) 
            {
                if (cola_vacia()) 
                {
                    frente = 0;
                }
                fin++;
                cola_calificaciones[fin] = pila_calificaciones[i];
            } 
        }
    }
}

void push_calif(){
    if (!pila_llena())
    {
        int calif = 0;
        cout << "\nIngrese la calificacion: ";
        cin >> calif;
        if (calif >= 0 && calif <= 10)
        {
            pila_calificaciones[++tope] = calif;
        }
        else
        {
            cout << "ERROR: Calificaciones en el rango de 0 a 10..." << endl;
        }
    }
    else
    {
        cout << "ERROR: La pila esta llena..." << endl;
    }
}

void mostrar_cola(){
    if (!cola_vacia()) {
        cout << "Cola de calificaciones mayor al promedio: (Promedio = " << promedio << ")" << endl;
        for (int i = frente; i <= fin; i++) {
            cout << cola_calificaciones[i] << endl;
        }
    } else {
        cout << "ERROR: La cola está vacía..." << endl;
    }
}

int main(){
    int op;
    cout << "Este programa permite el ingreso de maximo 5 calificaciones" << endl;
    do
    {
        cout << "\nMENU\n1. Ingresar calificacion\n2. Mostrar promedio de calificaciones\n3. Mostrar cola de calificaciones\n4. Salir\nEscoga una opcion: ";
        cin >> op;
        switch (op)
        {
            case 1: push_calif(); break;
            case 2: 
                calcular_promedio(); 
                cout << "\nEl promedio es de: " << promedio << endl;
                break;
            case 3: 
                mover_calif_cola();
                mostrar_cola();
                break;
            case 4: cout << "Adios..." << endl; break;

            default: cout << "ERROR: Opcion invalida..." << endl; break;
        }
    } while (op != 4);
}