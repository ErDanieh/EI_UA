#ifndef tokenizador_h_
#define tokenizador_h_
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
//#include <windows.h>

using namespace std;

// Variables globales para la tokenizacion por automata de estados
//  Estatus de tokenizacion
const int TOKENIZARnormal = 0;
const int TOKENIZARreal = 1;
const int TOKENIZARacronimo = 2;
const int TOKENIZARguion = 3;

// Estatus de revision
// Estatus de URLs
const int TOK_URL_HTTP_FTTP = 4;
const int TOK_URL = 5;

// Estatus de revision numeros reales
const int TOK_Real = 6;
const int TOK_Real1 = 7;
const int TOK_Real2 = 8;
const int TOK_Real3 = 9;
const int TOK_Real4 = 10;
const int TOK_Real5 = 11;

// Estatus de revision de emails
const int TOK_Email = 12;
const int TOK_Email1 = 13;
const int TOK_Email2 = 14;
const int TOK_Email3 = 15;

// Estatus de revision de Acronimos
const int TOK_Acronimo = 16;
const int TOK_Acronimo1 = 17;
const int TOK_Acronimo2 = 18;
const int TOK_Acronimo3 = 19;
const int TOK_Acronimo4 = 20;
const int TOK_Acronimo5 = 21;
const int TOK_Acronimo6 = 22;

// Estatus de revision de guiones (palabras compuestas)
const int TOK_Guion = 23;
const int TOK_Guion1 = 24;
const int TOK_Guion2 = 25;
const int TOK_Guion3 = 26;
const int TOK_Guion4 = 27;
const int TOK_Guion5 = 28;

// Estatus de Tokenizacion palabra simple
const int TOK_Normal = 29;

// Delimitadores Frecuentes que aparecen en URLs
const string delimitadoresURL = "_:/.?&-=#@";
// Delimitadores Frecuentes que aparecen en numeros reales
const string delimitadoresReal = "%$��";
// Delimitadores Frecuentes que aparecen en emails
const string delimitadoresEmail = "-_.";

class Tokenizador
{

    /**
     * @brief Sobrecarga del operador salida
     * cout << “DELIMITADORES: “ << delimiters << “ TRATA CASOS ESPECIALES:
     “ << casosEspeciales << “ PASAR A MINUSCULAS Y SIN ACENTOS: “ << pasarAminuscSinAcentos;
        Aunque se modifique el almacenamiento de los delimitadores por temas
        de eficiencia, el campo delimiters se imprimirá con el string leído en
        el tokenizador (tras las modificaciones y eliminación de los caracteres
        repetidos correspondientes)
     * @return ostream&
     */
    friend ostream &operator<<(ostream &, const Tokenizador &);

private:
    /**
     * @brief  Delimitadores de términos. Aunque se
     * modifique la forma de almacenamiento interna para mejorar la eficiencia, este
     * campo debe permanecer para indicar el orden en que se introdujeron los
     * delimitadores
     */
    string delimiters;

    /**
     * @brief Si true detectará palabras compuestas y casos especiales. Sino,
     * trabajará al igual que el algoritmo propuesto en la sección “Versión del
     * tokenizador vista en clase”
     */
    bool casosEspeciales;

    /**
     * @brief  Si true pasará el token a minúsculas y quitará acentos, antes de realizar la tokenización
     */
    bool pasarAminuscSinAcentos;

    /**
     * @brief Elimina los delimitadores repetidos
     *
     * @param delimitadoresPalabra
     */
    void EliminarRepetidos(string &delimitadoresPalabra);

    /**
     * @brief Transforma una cadena de caracteres a minúsculas y sin acentos
     *
     * @param cadena Cadena para transformar
     */
    string getMinusSinAcentos(const string &palabra) const;

    string eliminaBarraN(const string &i);

    /**
     * @brief  Funcion que informa si el caracter a analizar es un
     * delimitador
     * @param c caracter a analizar
     * @return es un delimitador o no
     */
    bool EsDelimitador(const char c) const;

    bool delimitadorDeReales(const char c) const;

    // Analizadores de cadena
    void analizaURLHTTPFTP(char &c, int &estado, const string &frase, string::size_type &pos, string::size_type &npos, bool &salida) const;

    void analizaURLyMarcaTokeniza(string::size_type &npos, const string &frase, int &estado) const;

    void analizaReal(char &c, int &estado, const string &frase, string::size_type &pos, string::size_type &npos, bool &salida,
                     bool &delimitadorRealEspecial, bool &anadirCero) const;

    void analizaEmail(char &c, int &estado, const string &frase, string::size_type &pos, string::size_type &npos, bool &salida) const;

    void analizaAcronimo(char &c, int &estado, const string &frase, string::size_type &pos, string::size_type &npos, bool &salida,
                         int numPuntoIzquierda, int numPuntosDerecha) const;

public:
    static const string delimiters_Siempre;

    /**
     * @brief Construct a new Tokenizador object
     * Inicializa delimiters a delimitadoresPalabra filtrando que no se
     * introduzcan delimitadores repetidos (de izquierda a derecha, en cuyo
     * caso se eliminarían los que hayan sido repetidos por la derecha);
     * casosEspeciales a kcasosEspeciales; pasarAminuscSinAcentos a
     * minuscSinAcentos
     *
     * @param delimitadoresPalabra
     * @param kcasosEspeciales
     * @param minuscSinAcentos
     */
    Tokenizador(const string &delimitadoresPalabra, const bool &kcasosEspeciales, const bool &minuscSinAcentos);

    /**
     * @brief Construct a new Tokenizador object
     *
     */
    Tokenizador(const Tokenizador &);

    /**
     * @brief Construct a new Tokenizador object
     * Inicializa delimiters=",;:.-/+*\\ '\"{}[]()<>¡!¿?&#=\t\n\r@";
     * casosEspeciales a true; pasarAminuscSinAcentos a false
     */
    Tokenizador();

    /**
     * @brief Destroy the Tokenizador object
     *  Pone delimiters=""
     */
    ~Tokenizador();

    /**
     * @brief Sobrecarga operador asignación
     *
     * @return Tokenizador&
     */
    Tokenizador &operator=(const Tokenizador &);

    /**
     * @brief Tokeniza str devolviendo el resultado en tokens. La lista tokens se
     * vaciará antes de almacenar el resultado de la tokenización.
     * @param str string a tokenizar
     * @param tokens tokens generados
     */
    void Tokenizar(const string &str, list<string> &tokens) const;

    /**
     * @brief Tokeniza el fichero i guardando la salida en el fichero f (una
     * palabra en cada línea del fichero). Devolverá true si se realiza la
     * tokenización de forma correcta; false en caso contrario enviando a cerr
     * el mensaje correspondiente (p.ej. que no exista el archivo i)
     * @param i Fichero a tokenizar
     * @param f Fichero de salida
     * @return true Si se realiza la tokenización de forma correcta
     * @return false Si no se detecta el archivo i o falla tokenización
     */
    bool Tokenizar(const string &i, const string &f) const;

    /**
     * @brief Tokeniza el fichero i guardando la salida en un fichero de nombre i
     * añadiéndole extensión .tk (sin eliminar previamente la extensión de i
     * por ejemplo, del archivo pp.txt se generaría el resultado en pp.txt.tk),
     * y que contendrá una palabra en cada línea del fichero. Devolverá true si
     * se realiza la tokenización de forma correcta; false en caso contrario
     * @param i fichero a tokenizar
     * @return true
     * @return false
     */
    bool Tokenizar(const string &i) const;

    /**
     * @brief
     * Tokeniza el fichero i que contiene un nombre de fichero por línea guardando la
     *  salida en ficheros (uno por cada línea de i) cuyo nombre será el leído en i
     *  añadiéndole extensión .tk, y que contendrá una palabra en cada línea del
     * fichero leído en i. Devolverá true si se realiza la tokenización de forma
     *  correcta de todos los archivos que contiene i; devolverá false en caso
     * contrario enviando a cerr el mensaje correspondiente (p.ej. que no exista
     *  el archivo i, o que se trate de un directorio, enviando a “cerr” los
     *  archivos de i que no existan o que sean directorios; luego no se ha de
     *  interrumpir la ejecución si hay algún archivo en i que no exista)
     * @param i fichero con la lista de fichero a tokenizar
     * @return true tokenización correcta de todos los ficheros
     * @return false tokenización incorrecta
     */
    bool TokenizarListaFicheros(const string &i) const;

    /**
     * @brief Tokeniza todos los archivos que contenga el directorio i, incluyendo
     * los de los subdirectorios, guardando la salida en ficheros cuyo nombre
     * será el de entrada añadiéndole extensión .tk, y que contendrá una
     * palabra en cada línea del fichero. Devolverá true si se realiza la
     * tokenización de forma correcta de todos los archivos; devolverá false en
     * caso contrario enviando a cerr el mensaje correspondiente (p.ej. que no
     * exista el directorio i, o los ficheros que no se hayan podido tokenizar)
     * @param i Directorio a tokenizar
     * @return true tokenización correcta de todos los ficheros del directorio y subdirectorios
     * @return false tokenización incorrecta
     */
    bool TokenizarDirectorio(const string &i) const;

    /**
     * @brief  Cambia “delimiters” por “nuevoDelimiters” comprobando que no hayan
     *  delimitadores repetidos (de izquierda a derecha), en cuyo caso se
     * eliminarían los que hayan sido repetidos (los nuevos delimitadores que
     * se van analizando) tanto en “nuevoDelimiters” como los que ya estuviesen en “delimiters”
     * @param nuevoDelimiters nuevos delimitadores
     */
    void DelimitadoresPalabra(const string &nuevoDelimiters);

    /**
     * @brief Añade al final de “delimiters” los nuevos delimitadores que aparezcan
     * en “nuevoDelimiters” (no se almacenarán caracteres repetidos)
     * @param nuevoDelimiters Nuevo delimitador
     */
    void AnyadirDelimitadoresPalabra(const string &nuevoDelimiters);

    /**
     * @brief Devuelve “delimiters”
     * @return string
     */
    string DelimitadoresPalabra() const;

    /**
     * @brief Cambia la variable privada “casosEspeciales”
     *
     * @param nuevoCasosEspeciales
     */
    void CasosEspeciales(const bool &nuevoCasosEspeciales);

    /**
     * @brief Devuelve el contenido de la variable privada “casosEspeciales”
     *
     * @return true
     * @return false
     */
    bool CasosEspeciales();

    /**
     * @brief Cambia la variable privada “pasarAminuscSinAcentos”. Atención al
     * formato de codificación del corpus (comando “file” de Linux). Para la
     * corrección de la práctica se utilizará el formato actual (ISO-8859).
     * @param nuevoPasarAminuscSinAcentos
     */
    void PasarAminuscSinAcentos(const bool &nuevoPasarAminuscSinAcentos);

    /**
     * @brief  Devuelve el contenido de la variable privada “pasarAminuscSinAcentos”
     * @return true
     * @return false
     */
    bool PasarAminuscSinAcentos();

    /**
     * @brief La lista de tokens será creada a partir de los casos especiales que se defienen
     * la implementación es mediante un automata de estados finitos.
     * @param tokens lista de tokens
     * @param frase frase en la que buscar casos
     * */
    void UsandoCasosEspeciales(list<string> &tokens, const string &frase) const;
};

#endif