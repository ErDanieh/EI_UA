#include "tokenizador.h"

const string Tokenizador::delimiters_Siempre = " \n";

/**
 * TODO: Volver a poner el TonekizarFichero el mensaje de error que falta
 * TODO:
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
        case 225: //á 
        case 193: //Á
            palabraAux += 'a';
            break;
        case 233: //é
        case 201: //É
            palabraAux += 'e';
            break;
        case 237: //í
        case 205: //Í
            palabraAux += 'i';
            break;
        case 243: //ó
        case 211: //Ó
            palabraAux += 'o';
            break;
        case 250: //ú
        case 218: //Ú
            palabraAux += 'u';
            break;
        case 209: // transforma Ñ mayuscula en minuscula
            palabraAux += 'ñ';
            break;
        default: //El resto de letras si son mayusculas son transformadas a minusculas
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
    //Se limpia la lista de tokens
    if(!tokens.empty()) tokens.clear();

    /**
     * if(casosEspeciales)
     *  tokenizarConCasosEspeciales(str,token);
     * else{
     * **/
    string token;
    string::size_type lastPos = str.find_first_not_of(delimiters, 0);
    string::size_type pos = str.find_first_of(delimiters, lastPos);
    while (string::npos != pos || string::npos != lastPos)
    {
        if(pasarAminuscSinAcentos)
            token = getMinusSinAcentos(str.substr(lastPos, pos - lastPos));
        else
            token = str.substr(lastPos, pos - lastPos);
        
        tokens.push_back(token);
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