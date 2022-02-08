#include "tokenizador.h"

void Tokenizador::EliminarRepetidos(string &delimitadoresPalabra)
{
    string delimitadores = "";
    for (int i = 0; i < delimitadoresPalabra.length(); i++)
    {
        if (delimitadores.find(delimitadoresPalabra[i]) == string::npos)
            delimitadores += delimitadoresPalabra[i];
    }
    delimitadoresPalabra = delimitadores;
}

Tokenizador::Tokenizador(string &delimitadoresPalabra, const bool &kcasosEspeciales, const bool &minuscSinAcentos)
{
    delimiters = "";
    casosEspeciales = kcasosEspeciales;
    pasarAminuscSinAcentos = minuscSinAcentos;
    EliminarRepetidos(delimitadoresPalabra);
}

Tokenizador::Tokenizador(const Tokenizador &token)
{
    this->delimiters = token.delimiters;
    this->casosEspeciales = token.casosEspeciales;
    this->pasarAminuscSinAcentos = token.pasarAminuscSinAcentos;
}

Tokenizador::Tokenizador()
{
    this->delimiters = ",;:.-/+*\\ '\"{}[]()<>¡!¿?&#=\t\n\r@";
    this->casosEspeciales = true;
    this->pasarAminuscSinAcentos = false;
}

Tokenizador::~Tokenizador()
{
    this->delimiters = "";
}

void Tokenizador::Tokenizar(const string &str, list<string> &tokens) const
{
    string::size_type lastPos = str.find_first_not_of(delimiters, 0);
    string::size_type pos = str.find_first_of(delimiters, lastPos);
    while (string::npos != pos || string::npos != lastPos)
    {
        tokens.push_back(str.substr(lastPos, pos - lastPos));
        lastPos = str.find_first_not_of(delimiters, pos);
        pos = str.find_first_of(delimiters, lastPos);
    }
}

bool Tokenizador::Tokenizar(const string &i, const string &f) const
{
    ifstream file;
    ofstream fileOut;
    string cadena;
    list<string> tokens;
    file.open(i.c_str());
    if (!file)
    {
        cerr << "ERROR: No existe el archivo: " << i << endl;
        return false;
    }
    else
    {
        while (!file.eof())
        {
            cadena = "";
            getline(file, cadena);
            if (cadena.length() != 0)
            {
                Tokenizar(cadena, tokens);
            }
        }
    }
    file.close();
    fileOut.open(f.c_str());
    list<string>::iterator itS;
    for (itS = tokens.begin(); itS != tokens.end(); itS++)
    {
        fileOut << (*itS) << endl;
    }
    fileOut.close();
    return true;
}

bool Tokenizador::Tokenizar(const string &i) const
{
    ifstream file;
    ofstream fileOut;
    string cadena;
    list<string> tokens;
    file.open(i.c_str());
    if (!file)
    {
        cerr << "ERROR: No existe el archivo: " << i << endl;
        return false;
    }
    else
    {
        while (!file.eof())
        {
            cadena = "";
            getline(file, cadena);
            if (cadena.length() != 0)
            {
                Tokenizar(cadena, tokens);
            }
        }
    }
    file.close();
    fileOut.open(i + ".tk");
    list<string>::iterator itS;
    for (itS = tokens.begin(); itS != tokens.end(); itS++)
    {
        fileOut << (*itS) << endl;
    }
    fileOut.close();
    return true;
}

bool Tokenizador::TokenizarListaFicheros(const string &i) const
{
    ifstream file;
    ofstream fileOut;
    string cadena;
    list<string> tokens;
    file.open(i.c_str());
    if (!file)
    {
        cerr << "ERROR: No existe el archivo: " << i << endl;
        return false;
    }
    else
    {
        while (!file.eof())
        {
            cadena = "";
            getline(file, cadena);
            cout << cadena << endl;
            if (cadena.length() != 0)
            {

                Tokenizar(cadena);
            }
        }
    }
    file.close();
    return true;
}

bool Tokenizador::TokenizarDirectorio(const string &i) const
{
    struct stat dir;
    // Compruebo la existencia del directorio
    int err = stat(i.c_str(), &dir);
    if (err == -1 || !S_ISDIR(dir.st_mode))
        return false;
    else
    {
        // Hago una lista en un fichero con find>fich
        string cmd = "find " + i + " -type f -follow |sort > .lista_fich";
        system(cmd.c_str());
        return TokenizarListaFicheros(".lista_fich");
    }
}



void Tokenizador::DelimitadoresPalabra(const string &nuevoDelimiters)
{
    EliminarRepetidos(this->delimiters = nuevoDelimiters);
}

void Tokenizador::AnyadirDelimitadoresPalabra(const string &nuevoDelimiters)
{
    EliminarRepetidos(this->delimiters += nuevoDelimiters);
}

string Tokenizador::DelimitadoresPalabra() const
{
    return this->delimiters;
}

void Tokenizador::CasosEspeciales(const bool &nuevoCasosEspeciales)
{
    this->casosEspeciales = nuevoCasosEspeciales;
}

bool Tokenizador::CasosEspeciales()
{
    return this->casosEspeciales;
}

void Tokenizador::PasarAminuscSinAcentos(const bool &nuevoPasarAminuscSinAcentos)
{
    this->pasarAminuscSinAcentos = nuevoPasarAminuscSinAcentos;
}

bool Tokenizador::PasarAminuscSinAcentos()
{
    return this->pasarAminuscSinAcentos;
}

ostream &operator<<(ostream &os, const Tokenizador &tokenizador)
{
    os << "DELIMITADORES: " << tokenizador.DelimitadoresPalabra() << " TRATA CASOS ESPECIALES: " << tokenizador.casosEspeciales
       << " PASAR A MINUSCULAS Y SIN ACENTOS: " << tokenizador.pasarAminuscSinAcentos;
    return os;
}