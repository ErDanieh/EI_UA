#include <iostream>
#include <string>
#include <list>
#include "tokenizador.h"
#include "chrono"

using namespace std;

///////// Comprobaci�n de que vac�e la lista resultado

void imprimirListaSTL(const list<string> &cadena)
{
        list<string>::const_iterator itCadena;
        for (itCadena = cadena.begin(); itCadena != cadena.end(); itCadena++)
        {
                cout << (*itCadena) << ", ";
        }
        cout << endl;
}

int main(void)
{
        Tokenizador a("",true,true);
        list<string> tokens;

        auto start = chrono::high_resolution_clock::now();
        a.TokenizarListaFicheros("listaFicheros.txt");
        auto end = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
        cout << "Tiempo de ejecución: " << duration.count() << " milisegundos" << endl;
}
