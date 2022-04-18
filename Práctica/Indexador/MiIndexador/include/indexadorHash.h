#ifndef _INDEXADORHASH_H_
#define _INDEXADORHASH_H_

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include "indexadorInformacion.h"
#include "stemmer.h"
#include "tokenizador.h"

using namespace std;

class IndexadorHash
{
    friend ostream &operator<<(ostream &s, const IndexadorHash &p);

public:
    IndexadorHash(const string &fichStopWords, const string &delimitadores,
                  const bool &detectComp, const bool &minuscSinAcentos, const string &dirIndice,
                  const int &tStemmer, const bool &almEnDisco, const bool &almPosTerm);

    IndexadorHash(const string &directorioIndexacion);

    /**
     * @brief Construtor copia
     *
     */
    IndexadorHash(const IndexadorHash &);

    /**
     * @brief Destroy the Indexador Hash object
     *
     */
    ~IndexadorHash();

    /**
     * @brief Operador igual
     */
    IndexadorHash &operator=(const IndexadorHash &);

    bool Indexar(const string &ficheroDocumentos);

    bool IndexarDirectorio(const string &dirAIndexar);

    bool GuardarIndexacion() const;

    bool RecuperarIndexacion(const string &directorioIndexacion);

    void ImprimirIndexacion() const;

    bool IndexarPregunta(const string &preg);

    bool DevuelvePregunta(string &preg) const;

    bool DevuelvePregunta(const string &word, InformacionTerminoPregunta &inf) const;

    bool DevuelvePregunta(InformacionPregunta &inf) const;

    void ImprimirIndexacionPregunta() const;

    void ImprimirPregunta();

    bool Devuelve(const string &word, InformacionTermino &inf) const;

    bool Devuelve(const string &word, const string &nomDoc, InfTermDoc &InfDoc) const;

    bool Existe(const string &word) const;

    bool Borra(const string &word);

    bool BorraDoc(const string &nomDoc);

    void VaciarIndiceDocs();

    void VaciarIndicePreg();

    bool Actualiza(const string &word, const InformacionTermino &inf);

    bool Inserta(const string &word, const InformacionTermino &inf);

    int NumPalIndexadas() const;

    string DevolverFichPalParada() const;

    void ListarPalParada() const;

    int NumPalParada() const;

    string DevolverDelimitadores() const;

    bool DevolverCasosEspeciales() const;

    bool DevolverPasarAminuscSinAcentos() const;

    bool DevolverAlmacenarPosTerm() const;

    string DevolverDirIndice() const;

    int DevolverTipoStemming() const;

    bool DevolverAlmEnDisco() const;

    void ListarInfColeccDocs() const;

    void ListarTerminos() const;

    bool ListarTerminos(const string &nomDoc) const;

    void ListarDocs() const;

    bool ListarDocs(const string &nomDoc) const;

private:
    /**
     * @brief Este constructor se pone en la parte privada porque no se permitirá
    crear un indexador sin inicializarlo convenientemente.
     */
    IndexadorHash();

    // Índice de términos indexados accesible por el término
    unordered_map<string, InformacionTermino> indice;

    // Índice de documentos indexados accesible por el nombre del documento
    unordered_map<string, InfDoc> indiceDocs;

    // Información recogida de la colección de documentos indexada
    InfColeccionDocs informacionColeccionDocs;

    // Pregunta indexada actualmente. Si no hay ninguna indexada, contendría el valor ""
    string pregunta;

    // Índice de términos indexados en una pregunta. Se almacenará en memoria principal
    unordered_map<string, InformacionTerminoPregunta> indicePregunta;

    // Información recogida de la pregunta indexada. Se almacenará en memoria principal
    InformacionPregunta infPregunta;

    unordered_set<string> stopWords;

    // Nombre del fichero que contiene las palabras de parada
    unordered_set<string> stopWords;

    Tokenizador tok;

    /** “directorioIndice” será el directorio del disco duro donde se almacenará el índice.
     * En caso que contenga la cadena vacía se creará enel directorio donde se ejecute el indexador**/
    string directorioIndice;

    int tipoStemmer;

    bool almacenarEnDisco;

    bool almacenarPosTerm;
};
#endif