#ifndef _INDEXADORINFORMACION_H_
#define _INDEXADORINFORMACION_H_

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <list>

using namespace std;
///////////////////////////////////////////////////////////////////////////////
class InfTermDoc
{
    friend ostream &operator<<(ostream &s, const InfTermDoc &p);

public:
    InfTermDoc(const InfTermDoc &);
    InfTermDoc();  // Inicializa ft = 0
    ~InfTermDoc(); // Pone ft = 0
    InfTermDoc &operator=(const InfTermDoc &);

private:
    // Frecuencia del tÃ©rmino en el documento
    int ft;

    /** Solo se almacenarÃ¡ esta informaciÃ³n si el campo privado del indexador
    almacenarPosTerm == true
    Lista de nÃºmeros de palabra en los que aparece el tÃ©rmino en el
    documento. Los nÃºmeros de palabra comenzarÃ¡n desde cero (la primera
    palabra del documento). Se numerarÃ¡n las palabras de parada. EstarÃ¡
    ordenada de menor a mayor posiciÃ³n.
    **/
    list<int> posTerm;
};
/////////////////////////////////////////////////////////////////////////////////////////
class InformacionTermino
{
    friend ostream &operator<<(ostream &s, const InformacionTermino &p);

public:
    InformacionTermino(const InformacionTermino &);
    InformacionTermino();  // Inicializa ftc = 0
    ~InformacionTermino(); // Pone ftc = 0 y vacÃ­a l_docs
    InformacionTermino &operator=(const InformacionTermino &);
    // AÃ±adir cuantos mÃ©todos se consideren necesarios para manejar la parte privada de la clase
private:
    // Frecuencia total del tÃ©rmino en la colecciÃ³n
    int ftc;
    /** Tabla Hash que se accederÃ¡ por el id del documento, devolviendo un
     *  objeto de la clase InfTermDoc que contiene toda la informaciÃ³n de
     *  apariciÃ³n del tÃ©rmino en el documento
     * */
    unordered_map<int, InfTermDoc> l_docs;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * Esta es mi implementacion de las fechas para los archivos,
 * utilizaremos la estructura del clock para obtener los datos
 * de la fecha y hora
 */
class Fecha
{
    int dia;
    int mes;
    int anyo;
    int hora;
    int min;
    int seg;

public:
    Fecha();
    Fecha(struct tm *clock);
    Fecha(const Fecha &);
    bool operator<(const Fecha &f) const;
    bool operator>(const Fecha &f) const;
    Fecha& operator=(const Fecha &f);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class InfDoc
{
    friend ostream &operator<<(ostream &s, const InfDoc &p);

public:
    InfDoc(const InfDoc &);
    InfDoc();
    ~InfDoc();
    InfDoc &operator=(const InfDoc &);
    // AÃ±adir cuantos mÃ©todos se consideren necesarios para manejar la parte privada de la clase
private:
    /** Identificador del documento. El primer documento indexado en la
     colecciÃ³n serÃ¡ el identificador 1  */
    int idDoc;

    // NÂº total de palabras del documento
    int numPal;

    // NÂº total de palabras sin stop-words del documento
    int numPalSinParada;

    /** NÂº total de palabras diferentes que no sean stop-words (sin acumular
    la frecuencia de cada una de ellas) */
    int numPalDiferentes;

    // TamaÃ±o en bytes del documento
    int tamBytes;

    /** Atributo correspondiente a la fecha y hora de modificaciÃ³n del
    documento*/
    Fecha fechaModificacion;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class InfColeccionDocs
{
    friend ostream &operator<<(ostream &s, const InfColeccionDocs &p);

public:
    InfColeccionDocs(const InfColeccionDocs &);
    InfColeccionDocs();
    ~InfColeccionDocs();
    InfColeccionDocs &operator=(const InfColeccionDocs &);

private:
    // NÂº total de documentos en la colecciÃ³n
    int numDocs;
    // NÂº total de palabras en la colecciÃ³n
    int numTotalPal;
    // NÂº total de palabras sin stop-words en la colecciÃ³n
    int numTotalPalSinParada;
    // NÂº total de palabras diferentes en la colecciÃ³n que no sean stop-words (sin acumular la frecuencia de cada una de ella
    int numTotalPalDiferentes;
    // TamaÃ±o total en bytes de la colecciÃ³n
    int tamBytes;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class InformacionTerminoPregunta
{
    friend ostream &operator<<(ostream &s, const InformacionTerminoPregunta &p);

public:
    InformacionTerminoPregunta(const InformacionTerminoPregunta &);
    InformacionTerminoPregunta();
    ~InformacionTerminoPregunta();
    InformacionTerminoPregunta &operator=(const InformacionTerminoPregunta &);

private:
    // Frecuencia total del término en la pregunta
    int ft;
    // Lista de números de palabra en los que aparece el término en la pregunta
    list<int> posTerm;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class InformacionPregunta
{
    friend ostream &operator<<(ostream &s, const InformacionPregunta &p);

public:
    InformacionPregunta(const InformacionPregunta &);
    InformacionPregunta();
    ~InformacionPregunta();
    InformacionPregunta &operator=(const InformacionPregunta &);

private:
    // NÂº total de palabras en la pregunta
    int numTotalPal;
    // NÂº total de palabras sin stop-words en la pregunta
    int numTotalPalSinParada;
    // NÂº total de palabras diferentes en la pregunta que no sean stop-words (sin acumular la frecuencia de cada una de ellas)
    int numTotalPalDiferentes;
};

#endif