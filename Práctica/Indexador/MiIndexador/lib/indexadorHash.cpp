#include "indexadorHash.h"
#include "Tokenizador.h"

ostream &operator<<(ostream &s, const IndexadorHash &p)
{
    s << "Fichero con el listado de palabras de parada : " << p.ficheroStopWords << "\n";
    s << "Tokenizador : " << p.tok << "\n";
    s << "Directorio donde se almacenara el indice generado : " << p.directorioIndice << "\n";
    s << "Stemmer utilizado : " << p.tipoStemmer << "\n";
    s << "Informacion de la coleccion indexada : " << p.informacionColeccionDocs << "\n";
    s << "Se almacenara parte del indice en disco duro : " << p.almacenarEnDisco << "\n";
    s << "Se almacenaran las posiciones de los terminos : " << p.almacenarPosTerm;
    return s;
}

// Constructor de la clase IndexadorHash que inicializa los atributos de la clase
IndexadorHash::IndexadorHash(const string &fichStopWords, const string &delimitadores,
                             const bool &detectComp, const bool &minuscSinAcentos, const string &dirIndice,
                             const int &tStemmer, const bool &almEnDisco, const bool &almPosTerm)
{
    // Asignamos el fichero que contiene las stopWords
    this->ficheroStopWords = fichStopWords;
    // Intentamos abrir el ficheor
    ifstream fichero(fichStopWords, ifstream::in);
    if (fichero.good())
    {
        // Si se ha podido abrir, leemos las stopWords
        string palabra;
        fichero >> palabra;
        while (!fichero.eof())
        {
            this->stopWords.insert(palabra);
            fichero >> palabra;
        }
        fichero.close();
    }

    // Inicializamos los valores que va a tener nuestro tokenizador
    this->tok.DelimitadoresPalabra(delimitadores);
    this->tok.PasarAminuscSinAcentos(minuscSinAcentos);
    this->tok.CasosEspeciales(detectComp);

    // Asignamos el valor del stemmer
    this->tipoStemmer = tStemmer; // 0 sin stemmer, 1 Porter espanol, 2 Porter ingles

    this->almacenarEnDisco = almEnDisco;
    this->almacenarPosTerm = almPosTerm;

    // Si se nos pasa el directorio vacio le asignamos el directorio en el que nos encontramos
    if (dirIndice != "")
    {
        this->directorioIndice = dirIndice;
    }
    else
    {
        this->directorioIndice = get_current_dir_name();
    }
}

IndexadorHash::IndexadorHash(const string &directorioIndexacion) {}

// Constrcutor de copia
IndexadorHash::IndexadorHash(const IndexadorHash &ind)
{
    this->ficheroStopWords = ind.ficheroStopWords;
    this->stopWords = ind.stopWords;
    this->tok = ind.tok;
    this->tipoStemmer = ind.tipoStemmer;
    this->directorioIndice = ind.directorioIndice;
    this->informacionColeccionDocs = ind.informacionColeccionDocs;
    this->almacenarEnDisco = ind.almacenarEnDisco;
    this->almacenarPosTerm = ind.almacenarPosTerm;
    this->indice = ind.indice;
    this->indiceDocs = ind.indiceDocs;

    this->indicePregunta = ind.indicePregunta;
    this->infPregunta = ind.infPregunta;
    this->pregunta = ind.pregunta;
}

// Destructor de indexadorHash
IndexadorHash::~IndexadorHash()
{
    // Primero tenemos que vaciar todos los indices
    indice.clear();
    indiceDocs.clear();
    indicePregunta.clear();

    this->pregunta.clear();
    this->ficheroStopWords.clear();
    this->directorioIndice.clear();
    this->tipoStemmer = 0;
    // Ponemos el almacenarEnDisco a false y el almacenarPosTerm a false
    this->almacenarEnDisco = false;
    this->almacenarPosTerm = false;
}

IndexadorHash &IndexadorHash::operator=(const IndexadorHash &ind)
{
    if (this != &ind)
    {
        this->ficheroStopWords = ind.ficheroStopWords;
        this->stopWords = ind.stopWords;
        this->tok = ind.tok;
        this->tipoStemmer = ind.tipoStemmer;
        this->directorioIndice = ind.directorioIndice;
        this->informacionColeccionDocs = ind.informacionColeccionDocs;
        this->almacenarEnDisco = ind.almacenarEnDisco;
        this->almacenarPosTerm = ind.almacenarPosTerm;
        this->indice = ind.indice;
        this->indiceDocs = ind.indiceDocs;

        this->indicePregunta = ind.indicePregunta;
        this->infPregunta = ind.infPregunta;
        this->pregunta = ind.pregunta;
    }
    return *this;
}

bool IndexadorHash::Indexar(const string &ficheroDocumentos)
{
    
}

bool IndexadorHash::IndexarDirectorio(const string &dirAIndexar) {}

bool IndexadorHash::GuardarIndexacion() const {}

bool IndexadorHash::RecuperarIndexacion(const string &directorioIndexacion) {}

void IndexadorHash::ImprimirIndexacion() const {}

bool IndexadorHash::IndexarPregunta(const string &preg) {}

bool IndexadorHash::DevuelvePregunta(string &preg) const
{
    if (!this->pregunta.empty())
    {
        preg = this->pregunta;
        return true;
    }
    else
        return false;
}

bool IndexadorHash::DevuelvePregunta(const string &word, InformacionTerminoPregunta &inf) const
{
    // Buscamos el termino en el indice de pregunta mediante un iterador
    auto it = indicePregunta.find(word);

    if (it != indicePregunta.end())
    {
        // Si lo encontramos se devuelve la informacion dentro del inf
        inf = it->second;
        return true;
    }
    else
        return false;
}

bool IndexadorHash::DevuelvePregunta(InformacionPregunta &inf) const
{
    if (!this->pregunta.empty())
    {
        inf = this->infPregunta;
        return true;
    }
    else
        return false;
}

void IndexadorHash::ImprimirIndexacionPregunta() const {}

void IndexadorHash::ImprimirPregunta()
{
    cout << "Pregunta indexada : " << this->pregunta << "\n";
    cout << "Informacion de la pregunta : " << this->infPregunta << "\n";
}

bool IndexadorHash::Devuelve(const string &word, InformacionTermino &inf) const {}

bool IndexadorHash::Devuelve(const string &word, const string &nomDoc, InfTermDoc &InfDoc) const {}

bool IndexadorHash::Existe(const string &word) const
{
    return (indice.find(word) != indice.end());
}

bool IndexadorHash::Borra(const string &word)
{
    return (indice.erase(word) != 0);
}

bool IndexadorHash::BorraDoc(const string &nomDoc) {}

void IndexadorHash::VaciarIndiceDocs() {}

void IndexadorHash::VaciarIndicePreg() {}

bool IndexadorHash::Actualiza(const string &word, const InformacionTermino &inf) {}

bool IndexadorHash::Inserta(const string &word, const InformacionTermino &inf) {}

int IndexadorHash::NumPalIndexadas() const {}

string IndexadorHash::DevolverFichPalParada() const {}

void IndexadorHash::ListarPalParada() const {}

int IndexadorHash::NumPalParada() const
{
    return this->stopWords.size();
}

string IndexadorHash::DevolverDelimitadores() const
{
    return this->tok.DelimitadoresPalabra();
}

bool IndexadorHash::DevolverCasosEspeciales()
{
    return tok.CasosEspeciales();
}

bool IndexadorHash::DevolverPasarAminuscSinAcentos()
{
    return tok.PasarAminuscSinAcentos();
}

bool IndexadorHash::DevolverAlmacenarPosTerm() const
{
    return this->almacenarPosTerm;
}

string IndexadorHash::DevolverDirIndice() const
{
    return this->directorioIndice;
}

int IndexadorHash::DevolverTipoStemming() const
{
    return this->tipoStemmer;
}

bool IndexadorHash::DevolverAlmEnDisco() const
{
    return this->almacenarEnDisco;
}

void IndexadorHash::ListarInfColeccDocs() const
{
    cout << informacionColeccionDocs << "\n";
}

void IndexadorHash::ListarTerminos() const {}

bool IndexadorHash::ListarTerminos(const string &nomDoc) const {}

void IndexadorHash::ListarDocs() const {}

bool IndexadorHash::ListarDocs(const string &nomDoc) const {}