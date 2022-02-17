#include "tokenizador.h"

const string Tokenizador::delimiters_Siempre = " \n";

/**
 * TODO:Evitar salto linea en consola
 * TODO:Implementar el automata de estados
 * TODO:Dibujar diagrama de estados
 *
 **/

Tokenizador::Tokenizador()
{
    this->delimiters = ",;:.-/+*\\ '\"{}[]()<>¡!¿?&#=\t\n\r@";
    this->casosEspeciales = true;
    this->pasarAminuscSinAcentos = false;
}

Tokenizador::Tokenizador(const string &delimitadoresPalabra, const bool &kcasosEspeciales, const bool &minuscSinAcentos)
{
    this->casosEspeciales = kcasosEspeciales;
    this->pasarAminuscSinAcentos = minuscSinAcentos;
    if (this->casosEspeciales)
    {
        string insertar = delimiters_Siempre + delimitadoresPalabra;
        EliminarRepetidos(this->delimiters = insertar);
    }
    else
    {
        EliminarRepetidos(this->delimiters = delimitadoresPalabra);
    }
}

Tokenizador::Tokenizador(const Tokenizador &token)
{
    this->delimiters = token.delimiters;
    this->casosEspeciales = token.casosEspeciales;
    this->pasarAminuscSinAcentos = token.pasarAminuscSinAcentos;
}

Tokenizador::~Tokenizador()
{
    this->delimiters.clear();
}

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

string Tokenizador::getMinusSinAcentos(const string &palabra) const
{
    string palabraAux = "";
    for (int i = 0; i < palabra.size(); i++)
    {
        switch ((unsigned char)palabra[i])
        {
        case 225: // a minuscula con acento
        case 193: // A mayuscula con acento
            palabraAux += 'a';
            break;
        case 233: // e minuscula con acento
        case 201: // E mayuscula con acento
            palabraAux += 'e';
            break;
        case 237: // i minuscula con acento
        case 205: // I mayuscula con acento
            palabraAux += 'i';
            break;
        case 243: // o minuscula con acento
        case 211: // O mayuscula con acento
            palabraAux += 'o';
            break;
        case 250: // u minuscula con acento
        case 218: // U mayuscula con acento
            palabraAux += 'u';
            break;
        case 209: // transforma � mayuscula en minuscula
            palabraAux += '�';
            break;
        default: // El resto de letras si son mayusculas son transformadas a minusculas
            if (palabra[i] >= 'A' && palabra[i] <= 'Z')
                palabraAux += tolower(palabra[i]);
            else
                palabraAux += palabra[i];
            break;
        }
    }
    return palabraAux;
}

void Tokenizador::Tokenizar(const string &str, list<string> &tokens) const
{
    // Se limpia la lista de tokens
    // if (!tokens.empty())
    // tokens.clear();
#if 0
    if (casosEspeciales)
        UsandoCasosEspeciales(tokens, str);
#endif
    // else
    //{

    string token;
    string tok;
    string::size_type lastPos = str.find_first_not_of(delimiters, 0);
    string::size_type pos = str.find_first_of(delimiters, lastPos);
    while (string::npos != pos || string::npos != lastPos)
    {
        if (pasarAminuscSinAcentos)
        {
            token = getMinusSinAcentos(str.substr(lastPos, pos - lastPos));
        }
        else
            token = str.substr(lastPos, pos - lastPos);

        //cout << token << endl;
        tokens.push_back(token);
        lastPos = str.find_first_not_of(delimiters, pos);
        pos = str.find_first_of(delimiters, lastPos);
    }
    //}
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
        cerr << "ERROR: No existe el archivo: " << i << "\n";
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
        fileOut << (*itS) << "\n";
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
        cerr << "ERROR: No existe el archivo: " << i << "\n";
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
    bool resultado = true;
    string fichero;
    fstream f(i);

    getline(f, fichero);
    while (!f.eof())
    {
        if (!Tokenizar(fichero))
            resultado = false;

        getline(f, fichero);
    }
    return resultado;
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
    // Solo si los casos especiales están activados se añadirán los delimitadores que siempre se activan
    // el salto de linea y el espacio
    if (this->casosEspeciales)
    {
        string insertados = delimiters_Siempre + nuevoDelimiters;
        EliminarRepetidos(insertados);
        this->delimiters = insertados;
    }
    else
    {
        EliminarRepetidos(this->delimiters = nuevoDelimiters);
    }
}

void Tokenizador::AnyadirDelimitadoresPalabra(const string &nuevoDelimiters)
{
    if (!this->casosEspeciales)
    {
        EliminarRepetidos(this->delimiters += nuevoDelimiters);
    }
    else
    {
        string insertados = delimiters_Siempre + nuevoDelimiters;
        EliminarRepetidos(insertados);
        this->delimiters += insertados;
    }
}

string Tokenizador::DelimitadoresPalabra() const
{
    return this->delimiters;
}

void Tokenizador::CasosEspeciales(const bool &nuevoCasosEspeciales)
{
    if (!nuevoCasosEspeciales)

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

bool Tokenizador::EsDelimitador(const char caracter) const
{
    if (caracter == '\0')
        return true;
    else if (caracter == ' ')
        return true;
    else if (this->delimiters.find(caracter) != string::npos) // Mira en los delimitadores marcados
        return true;
    else
        return false;
}

void Tokenizador::analizaURLHTTPFTP(char &c, int &estado, const string &frase, string::size_type &pos, string::size_type &npos, bool &salida) const
{
    if ((pos + 5 < frase.length() && frase[pos] == 'h' && frase[pos + 1] == 't' && frase[pos + 2] == 't' && frase[pos + 3] == 'p' &&
         (frase[pos + 4] == ':' || (pos + 6 < frase.length() && frase[pos + 4] == 's' && frase[pos + 5] == ':'))) ||
        (pos + 4 < frase.length() && frase[pos] == 'f' && frase[pos + 1] == 't' && frase[pos + 2] == 'p' && frase[pos + 3] == ':'))
    {
        estado = TOK_URL;
        npos = frase.find_first_of(':', pos);
    }
    else if (c == '\0')
        salida = true;
    else if (c != '.' && c != ',' && EsDelimitador(c))
        ++pos;
    else
        estado = TOK_Real;
}

void Tokenizador::analizaURLyMarcaTokeniza(string::size_type &npos, const string &frase, int &estado) const
{
    while (true)
    {
        npos = frase.find_first_of(this->delimiters, npos + 1);
        if (npos > frase.length() || delimitadoresURL.find(frase[npos]) == string::npos || frase[npos] == '\0')
            break;
    }
    estado = TOKENIZARnormal;
}

void Tokenizador::UsandoCasosEspeciales(list<string> &tokens, const string &frase) const
{
    int casoEstamos = TOK_URL_HTTP_FTTP;
    string delimitadores = delimiters + " ";
    char caracter;
    string::size_type pos = 0;
    string::size_type npos = 0;
    string token;
    bool salir = false;
    bool anadirCero = false;
    bool delimitadorRealEspecial = false;
    int nLeftPointAcronim = 0;
    int nRightPointAcronim = 0;
    int nRightGuionGuion = 0;

    if (frase.length() == 0)
    {
        while (!salir)
        {
            // Asignamos el barra 0 cuando nos pasamos de la longitud de la cadena para salirnos
            if (npos >= frase.length())
                caracter = '\0';
            else
                caracter = frase[npos]; // Asignamos el caracter que vamos a analizar

            // Automata de analisis de estados de la cadena
            switch (casoEstamos)
            {
            case TOK_URL_HTTP_FTTP:
                analizaURLHTTPFTP(caracter, casoEstamos, frase, pos, npos, salir);
                break;
            case TOK_URL:
                analizaURLyMarcaTokeniza(npos, frase, casoEstamos);
                break;
            }
        }
    }
}
