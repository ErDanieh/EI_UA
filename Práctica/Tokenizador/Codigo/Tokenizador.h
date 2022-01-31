#ifndef Tokenizador_h_
#define Tokenizador_h_
#include <ostream>
#include <vector>
#include <list>
using namespace std;

class Tokenizador
{

    /*** cout << “DELIMITADORES: “ << delimiters << “ TRATA CASOS ESPECIALES: 
    “ << casosEspeciales << “ PASAR A MINUSCULAS Y SIN ACENTOS: “ << pasarAminuscSinAcentos; 
    Aunque se modifique el almacenamiento de los delimitadores por temas 
    de eficiencia, el campo delimiters se imprimirá con el string leído en 
    el tokenizador (tras las modificaciones y eliminación de los caracteres 
    repetidos correspondientes) 
    **/
    friend ostream &operator<<(ostream &, const Tokenizador &);

private:
    string delimiters;

    bool casosEspeciales;

    bool pasarAminuscSinAcentos;

public:
    Tokenizador(const string &delimitadoresPalabra, const bool &kcasosEspeciales, const bool &minuscSinAcentos);

    Tokenizador(const Tokenizador &);

    Tokenizador();

    ~Tokenizador();

    Tokenizador &operator=(const Tokenizador &);

    void Tokenizar(const string &str, list<string> &tokens) const;

    bool Tokenizar(const string &i, const string &f) const;

    bool Tokenizar(const string &i) const;

    bool TokenizarListaFicheros(const string &i) const;

    bool TokenizarDirectorio(const string &i) const;

    void DelimitadoresPalabra(const string &nuevoDelimiters);

    void AnyadirDelimitadoresPalabra(const string &nuevoDelimiters);

    string DelimitadoresPalabra() const;

    void CasosEspeciales(const bool &nuevoCasosEspeciales);

    bool CasosEspeciales();

    void PasarAminuscSinAcentos(const bool &nuevoPasarAminuscSinAcentos);

    bool PasarAminuscSinAcentos();
};

#endif