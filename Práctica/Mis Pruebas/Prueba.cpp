#include <iostream>
#include <string>
#include <list>
#include <ostream>
#include <fstream>
#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <cstring>
#include <stdio.h>
#include <filesystem>
#include <sys/stat.h>

/**TODO
 * Probar las estructuras de datos para almacenar las diferentes flags activadas
 * por los tokens de es decir la URL, el correo, etc.
 * */

using namespace std;

typedef struct TokenF
{
    string token;
    vector<bool> flags [5];
}

#define casosEspeciales true
#define pasarAminuscSinAcentos false

void
Tokenizar(const string &str, list<string> &tokens)
{
    unsigned t0, t1;
    t0 = clock();
    string delimiters = " ,;:.-/+*\\ '\"{}[]()<>¡!¿?&#=\t\n\r@";
    string::size_type lastPos = str.find_first_not_of(delimiters, 0);
    string::size_type pos = str.find_first_of(delimiters, lastPos);
    while (string::npos != pos || string::npos != lastPos)
    {
        tokens.push_back(str.substr(lastPos, pos - lastPos));
        lastPos = str.find_first_not_of(delimiters, pos);
        pos = str.find_first_of(delimiters, lastPos);
    }
    t1 = clock();
    long time = (long(t1 - t0) / CLOCKS_PER_SEC);
    cout << "Execution Time: " << time << endl;
}

void TokenizarEspacios(const string &str, list<string> &tokens)
{
    unsigned t0, t1;
    t0 = clock();
    string delimiters = " ";
    string::size_type lastPos = str.find_first_not_of(delimiters, 0);
    string::size_type pos = str.find_first_of(delimiters, lastPos);
    while (string::npos != pos || string::npos != lastPos)
    {
        tokens.push_back(str.substr(lastPos, pos - lastPos));
        lastPos = str.find_first_not_of(delimiters, pos);
        pos = str.find_first_of(delimiters, lastPos);
    }
    t1 = clock();
    long time = (long(t1 - t0) / CLOCKS_PER_SEC);
    cout << "Execution Time: " << time << endl;
}

void Tokenizar2(const string &str, list<string> &tokens)
{
    unsigned t0, t1;
    t0 = clock();
    string delimiters = ",;:.-/+*\\ '\"{}[]()<>¡!¿?&#=\t\n\r@";
    // Recorrer la cadena str y separar los tokens
    string token;
    for (string::const_iterator it = str.begin(); it != str.end(); it++)
    {
        if (delimiters.find(*it) != string::npos)
        {
            // Si el caracter es un delimitador, agregar el token a la lista
            if (!token.empty())
            {
                tokens.push_back(token);
                token.clear();
            }
        }
        else
        {
            // Si el caracter no es un delimitador, agregarlo al token
            token += *it;
        }
    }
    t1 = clock();
    double time = (double(t1 - t0) / CLOCKS_PER_SEC);
    cout << "Execution Time: " << time << endl;
}

// url detector from string
bool UrlDetector(const string &str)
{
    string delimiters = " ,;:.-/+*\\ '\"{}[]()<>¡!¿?&#=\t\n\r@";
    string::size_type lastPos = str.find_first_not_of(delimiters, 0);
    string::size_type pos = str.find_first_of(delimiters, lastPos);
    while (string::npos != pos || string::npos != lastPos)
    {
        if (str.substr(lastPos, pos - lastPos).find("http") != string::npos ||
            str.substr(lastPos, pos - lastPos).find("ftp") != string::npos ||
            str.substr(lastPos, pos - lastPos).find("https") != string::npos ||
            str.substr(lastPos, pos - lastPos).find("Https") != string::npos ||
            str.substr(lastPos, pos - lastPos).find("Http") != string::npos ||
            str.substr(lastPos, pos - lastPos).find("Ftp") != string::npos)
        {
            cout << "he encontrado una url:" << str << endl;
            return true;
        }
        lastPos = str.find_first_not_of(delimiters, pos);
        pos = str.find_first_of(delimiters, lastPos);
    }
    return false;
}

bool Ficherizador(const string &i, const string &f)
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
                cout << cadena << endl;
                TokenizarEspacios(cadena, tokens);
                while (!tokens.empty())
                {
                    string tk = tokens.front();
                    UrlDetector(tk);
                    Tokenizar(tk, tokens);
                    tokens.pop_front();
                }
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

int main()
{
    // list<string> tokens;
    // Tokenizar("hola, mundo este es mi correo electrónico podeis agregarme quienes queraís danielasensi444@outlook.com", tokens);
    /**
    while (!tokens.empty())
    {
        string tk = tokens.front();
        cout << tk << endl;
        tokens.pop_front();
    }
    **/

    Ficherizador("loreIpsum3l.txt", "loreTok.txt");
}