#include <iostream>
#include "tokenizador.h"
#include <list>

using namespace std;

int main()
{
    //SetConsoleOutputCP(1252);
    //SetConsoleCP(1252);
    list<string> tokens;
    Tokenizador tok;
    //tok.Tokenizar("prueba del metodo", tokens);
    /**while (!tokens.empty())
    {
        string tk = tokens.front();
        cout << tk << endl;
        tokens.pop_front();
    }
    **/
    //tok.Tokenizar("pruebatexto.txt", "salida.txt");
    //tok.AnyadirDelimitadoresPalabra("KKKX;;;;;;;");
    //cout << tok.DelimitadoresPalabra() << endl;
    //cout << tok << endl;
    //tok.Tokenizar("Hola.txt");
    //tok.TokenizarListaFicheros("pruebaTok.txt");
    tok.TokenizarDirectorio("TextosPruebasTokenizar");
    return 0;
}