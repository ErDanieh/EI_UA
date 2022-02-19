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
    this->delimiters = ",;:.-/+*\\ '\"{}[]()<>Â¡!Â¿?&#=\t\n\r@";
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
        case 209: // transforma Ñ mayuscula en minuscula
            palabraAux += 'ñ';
            break;
        default: // El resto de letras si son mayusculas son transformadas a minusculas
            if (palabra[i] >= 'A' && palabra[i] <= 'Z')
                palabraAux += tolower(palabra[i]);
            else
                // cout<<sizeof(palabra[i])<<endl;
                palabraAux += palabra[i];
            break;
        }
    }
    return palabraAux;
}

void Tokenizador::Tokenizar(const string &str, list<string> &tokens) const
{
    if (!tokens.empty())
        tokens.clear();

    if (casosEspeciales)
    {
        // cout << str << endl;
        UsandoCasosEspeciales(tokens, str);
    }
    else
    {

        string token;
        string::size_type lastPos = str.find_first_not_of(delimiters, 0);
        string::size_type pos = str.find_first_of(delimiters, lastPos);
        while (string::npos != pos || string::npos != lastPos)
        {
            if (pasarAminuscSinAcentos)
                token = getMinusSinAcentos(str.substr(lastPos, pos - lastPos));
            else
                token = str.substr(lastPos, pos - lastPos);

            tokens.push_back(token);
            lastPos = str.find_first_not_of(delimiters, pos);
            pos = str.find_first_of(delimiters, lastPos);
        }
    }
}

bool Tokenizador::Tokenizar(const string &i, const string &f) const
{
    ifstream file;
    ofstream fileOut;
    string cadena;
    list<string> tokens;
    file.open(i.c_str());
    fileOut.open(f.c_str());
    list<string>::iterator itS;

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
                for (itS = tokens.begin(); itS != tokens.end(); itS++)
                {
                    fileOut << (*itS) << "\n";
                }
            }
        }
    }
    file.close();
    fileOut.close();

    if (!tokens.empty())
        tokens.clear();

    return true;
}

bool Tokenizador::Tokenizar(const string &i) const
{
    return Tokenizar(i, i + ".tk");
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
    // Solo si los casos especiales estÃ¡n activados se aÃ±adirÃ¡n los delimitadores que siempre se activan
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

bool Tokenizador::delimitadorDeReales(const char c) const
{

    // Miramos si esta en los caracteres definidos en los delimitadores de los reales
    if (delimitadoresReal.find(c) != string::npos)
        return true;
    else if ((unsigned char)c == 164) // Miramos si el caracter es el simbolo del ? en iso-8859-1
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
    bool salbucle = true;
    while (salbucle)
    {
        npos = frase.find_first_of(this->delimiters, npos + 1);
        if (npos > frase.length() || delimitadoresURL.find(frase[npos]) == string::npos || frase[npos] == '\0')
            salbucle = false;
    }
    estado = TOKENIZARnormal;
}

void Tokenizador::analizaReal(char &c, int &estado, const string &frase, string::size_type &pos, string::size_type &npos,
                              bool &salida, bool &delimitadorRealEspecial, bool &anadirCero) const
{
    switch (estado)
    {
    case TOK_Real:
        npos = pos;
        c = frase[npos];
        // Miramos si al número real hay que añadirle un 0 porque no lo tiene en el texto
        if ((c == '.' || c == ',') && (pos == 0 || (frase[pos - 1] != '.' && frase[pos - 1] != ',')))
            estado = TOK_Real1;
        else if (c >= '0' && c <= '9')
            estado = TOK_Real2;
        else
            estado = TOK_Email;
        break;

    // Al ser un número decimal asi -> .9 debemos añadirle un 0 en la tokenización ->0.9
    case TOK_Real1:
        // Comprabamos si debemos anadir un 0 al número tokenizado
        if (c >= '0' && c <= '9')
        {
            anadirCero = true;
            estado = TOK_Real2;
        }
        else
            estado = TOK_Email;
        break;

    // Analizamos las comas y puntos ya que el número es decimal
    case TOK_Real2:
        if (c == '.' || c == ',')
            estado = TOK_Real3;
        else if (EsDelimitador(c))
            estado = TOKENIZARreal;
        else if (delimitadorDeReales(c))
        {
            delimitadorRealEspecial = true;
            estado = TOK_Real5;
        }
        break;

    case TOK_Real3:
        if (c >= '0' && c <= '9')
            estado = TOK_Real2; // Volvemos a analizar los números
        else if (EsDelimitador(c))
        {
            estado = TOKENIZARreal;
            --npos;
        }
        else
            estado = TOK_Email;
        break;
    case TOK_Real5:
        if (EsDelimitador(c))
        {
            estado = TOKENIZARreal;
            --npos;
        }
        else
            estado = TOK_Email;
    }
}

void Tokenizador::analizaEmail(char &c, int &estado, const string &frase, string::size_type &pos, string::size_type &npos, bool &salida) const
{
    switch (estado)
    {

    case TOK_Email:
        npos = pos;
        c = frase[npos];
        if (EsDelimitador(c))
            estado = TOK_Acronimo;
        else
            estado = TOK_Email1;
        break;

    case TOK_Email1:
        if (c == '@') // Buscamos el arrboba para mandar todo el trozo a tokenizar
            estado = TOK_Email2;
        else if (delimitadoresEmail.find(c) != string::npos)
        {
        }
        else if (EsDelimitador(c))
            estado = TOK_Acronimo;
        break;

    case TOK_Email2:
        if (c == '@')
            estado = TOK_Acronimo;
        else if (delimitadoresEmail.find(c) != string::npos)
        {
            estado = TOK_Email3;
        }
        else if (EsDelimitador(c))
            estado = TOKENIZARnormal;
        break;

    case TOK_Email3:
        if (c == '@')
            estado = TOK_Acronimo;
        else if (delimitadoresEmail.find(c) != string::npos)
        {
            estado = TOKENIZARnormal;
        }
        else if (EsDelimitador(c))
            estado = TOKENIZARnormal;
        break;
    }
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

    cout << frase << endl;

    if (frase.length() != 0)
    {
        while (!salir)
        {
            cout << caracter << endl;
            cout << casoEstamos << endl;
            // Asignamos el barra 0 cuando nos pasamos de la longitud de la cadena para salirnos
            if (npos >= frase.length())
                caracter = '\0';
            else
                caracter = frase[npos]; // Asignamos el caracter que vamos a analizar

            // Automata de analisis de estados de la cadena
            switch (casoEstamos)
            {
            // Analisis y marcaje de URLs
            case TOK_URL_HTTP_FTTP:
            {
                analizaURLHTTPFTP(caracter, casoEstamos, frase, pos, npos, salir);
                break;
            }
            case TOK_URL:
            {
                analizaURLyMarcaTokeniza(npos, frase, casoEstamos);
                break;
            }

            // Analisis de números reales
            case TOK_Real:
            case TOK_Real1:
            case TOK_Real2:
            case TOK_Real3:
            case TOK_Real4:
            case TOK_Real5:
                analizaReal(caracter, casoEstamos, frase, pos, npos, salir, delimitadorRealEspecial, anadirCero);
                break;

            // Analisis de emails
            case TOK_Email:
            case TOK_Email1:
            case TOK_Email2:
            case TOK_Email3:
                analizaEmail(caracter, casoEstamos, frase, pos, npos, salir);
                break;

            // Analisis de acronimos
            
            }

            // Casos de tokenizacion y adicion de informacion a tokens
            switch (casoEstamos)
            {
            case TOKENIZARnormal:
                token = frase.substr(pos, npos - pos);
                break;
            case TOKENIZARreal:
                if (!anadirCero)
                    token = frase.substr(pos, npos - pos);
                else
                    token = "0" + frase.substr(pos, npos - pos);

                if (delimitadorRealEspecial)
                    --npos;
                break;
            case TOKENIZARacronimo:
                token = frase.substr(pos + nLeftPointAcronim, (npos - nRightPointAcronim) - (pos + nLeftPointAcronim));
                break;
            case TOKENIZARguion:
                token = frase.substr(pos, (npos - nRightGuionGuion) - pos);
                break;
            }

            // Metemos el token en la lista, pasanado a minusculas si es necesario
            if (casoEstamos >= 0 && casoEstamos <= 3)
            {
                // Antes de meterlo lo pasamos a minusculas
                if (this->pasarAminuscSinAcentos)
                    tokens.push_back(getMinusSinAcentos(token));
                else
                    tokens.push_back(token);

                // Reseteamos los valores
                pos = npos + 1;
                anadirCero = false;
                delimitadorRealEspecial = false;
                nLeftPointAcronim = 0;
                nRightPointAcronim = 0;
                nRightGuionGuion = 0;
                casoEstamos = TOK_URL_HTTP_FTTP;

                if (npos >= frase.length() || frase[npos] == '\0')
                    salir = true;
            }

            if (npos != string::npos)
                ++npos; // Pasamos a la siguiente letra del string
        }
    }
}
