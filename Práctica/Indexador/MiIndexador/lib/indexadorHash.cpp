#include "indexadorHash.h"
#include "tokenizador.h"

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
    else
    {
        cerr << "No se ha encontrado el fichero con StopWords"
             << "\n";
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

IndexadorHash::IndexadorHash(const string &directorioIndexacion)
{
    RecuperarIndexacion(directorioIndexacion);
}

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
    // Abrimos el fichero ficheroDocumentos
    ifstream nombresDocumentos(ficheroDocumentos, ifstream::in);
    // Informacion del documento que necesito para indexar
    string documentoAnalizo, lineaAnalizo;
    bool procederIndexacion = false;
    struct stat infoDocumento;
    int idDocumentoAuxiliar;
    list<string> tokensLineaAnalizo;
    int posTermino;

    // Si se ha podido abrir bien el fichero y podido leer
    if (nombresDocumentos.good())
    {
        ifstream documentoAnalizoFich;
        documentoAnalizo = "";
        // Cojo el nonbre de un documento
        while (getline(nombresDocumentos, documentoAnalizo))
        {
            // cout << "Analizando documento : " << documentoAnalizo << endl;
            auto itIndiceDocumentos = indiceDocs.find(documentoAnalizo);
            // Reinicio los valores que necesito
            idDocumentoAuxiliar = 0;
            procederIndexacion = false;
            posTermino = 0;

            // Saco la informacion del documento
            stat(documentoAnalizo.c_str(), &infoDocumento);
            // Obtengo la fecha de modificacion del documento y la parseo a mi propia estructura
            Fecha fechaModificacionDoc(gmtime(&(infoDocumento.st_mtime)));

            // Comprobamos que el fichero no esta en el indice
            if (itIndiceDocumentos == indiceDocs.end())
            {
                // cout << "El documento no esta en el indice" << endl;
                //  Como no hemos encontrado el fichero entonces lo indexamos
                procederIndexacion = true;
            }
            else
            {
                cerr << "El documento " << documentoAnalizo << " ya esta en el indice"
                     << "\n";
                // Lo ha encontrado entonces deberemos comprobar si la fecha de modificacion es igual o menor que la que tenemos
                if (itIndiceDocumentos->second.getFechaModificacion() < fechaModificacionDoc)
                {
                    cout << "El documento " << documentoAnalizo << " se va a reindexar"
                         << "\n";
                    // Nos guardamos el id del documento que tenemos en el indice para poder asignarlo nuevamente
                    idDocumentoAuxiliar = itIndiceDocumentos->second.getIdDoc();
                    procederIndexacion = true;
                }
                // Borramos el documento del indice para poder volver a indexarlo
                if (!BorraDoc(documentoAnalizo))
                {
                    // cout << "No se ha podido borrar el documento del indice" << endl;
                    cerr << "No se ha podido borrar el documento " << documentoAnalizo << "\n";
                    return false;
                }
            }

            // Procedemos a la indexacion si lo necesitamos
            if (procederIndexacion)
            {
                // cout << documentoAnalizo << endl;
                //  Abrimos el documento que estamos analizando para leerlo
                documentoAnalizoFich.open(documentoAnalizo.c_str(), ifstream::in);

                // Si no se ha podido abrir el documento
                if (!documentoAnalizoFich.good())
                {
                    // Sacamos el error por pantalla si no se ha podido abrir el documento
                    this->~IndexadorHash();
                    cerr << "No se ha podido abrir el documento " << documentoAnalizo << "\n";
                    return false;
                }
                else // Si si que se puede abrir
                {
                    // cout << "he abierto el documento" << endl;

                    InfDoc informacioDocumentoAnalizo;
                    // cout << "Voy a insertar la informacion" << endl;

                    // Empezamos a meterle la informacion
                    if (idDocumentoAuxiliar != 0)
                    {
                        informacioDocumentoAnalizo.setIdDoc(idDocumentoAuxiliar);
                    }
                    else
                    {
                        informacionColeccionDocs.setNumDocs(informacionColeccionDocs.getNumDocs() + 1);
                        informacioDocumentoAnalizo.setIdDoc(informacionColeccionDocs.getNumDocs());
                    }
                    informacioDocumentoAnalizo.setTamBytes(infoDocumento.st_size);
                    informacioDocumentoAnalizo.setFechaModificacion(fechaModificacionDoc);

                    // cout << "he asignado la informacion" << endl;

                    // Leemos la linea del documento
                    while (getline(documentoAnalizoFich, lineaAnalizo))
                    {
                        // cout << lineaAnalizo << endl;
                        //  Sacamos los tokens de la linea
                        tok.Tokenizar(lineaAnalizo, tokensLineaAnalizo);
                        // Asignamos la cantidad de palabras que tiene el documento
                        informacioDocumentoAnalizo.setNumPal(informacioDocumentoAnalizo.getNumPal() + tokensLineaAnalizo.size());

                        // Para cada uno de los token que tenemos le pasamos el stemmer y sacamos su informacion
                        for (auto itTokens = tokensLineaAnalizo.begin(); itTokens != tokensLineaAnalizo.end(); ++itTokens)
                        {
                            // Le pasamos el stemmer al token
                            stemmerIndexador.stemmer((*itTokens), tipoStemmer);
                            // Buscamos en nuestras stopWords para ver si tenemos que quitarla o no
                            if (stopWords.find((*itTokens)) == stopWords.end())
                            {
                                // Incrementamos el numero de palabras sin parada
                                informacioDocumentoAnalizo.setNumPalSinParada(informacioDocumentoAnalizo.getNumPalSinParada() + 1);

                                // Comprobamos si el termino ya esta en el indice
                                auto PalabrasIndice = indice.find((*itTokens));

                                // Si el termino ya existe lo que vamos a hacer es cargar su informacion y modificarla
                                if (PalabrasIndice != indice.end())
                                {
                                    // Cargamos la informacion del termino
                                    auto informacionTerminoCargado = PalabrasIndice->second.getL_docs().find(informacioDocumentoAnalizo.getIdDoc());

                                    // El termino existe ya en el documento por lo tanto actualizamos frencuencia y posicion
                                    if (informacionTerminoCargado != PalabrasIndice->second.getL_docs().end())
                                    {
                                        informacionTerminoCargado->second.setFt(informacionTerminoCargado->second.getFt() + 1);
                                        informacionTerminoCargado->second.setPosTerm(posTermino);
                                    }
                                    else // El termino no ha aparecido el documento hasta ahora por lo tanto lo anadimos
                                    {
                                        informacioDocumentoAnalizo.setNumPalDiferentes(informacioDocumentoAnalizo.getNumPalDiferentes() + 1);
                                        InfTermDoc informacionTerminoDocumento;
                                        informacionTerminoDocumento.setFt(1);
                                        informacionTerminoDocumento.setPosTerm(posTermino);
                                        PalabrasIndice->second.getL_docs().insert(pair<int, InfTermDoc>(informacioDocumentoAnalizo.getIdDoc(), informacionTerminoDocumento));
                                    }
                                    PalabrasIndice->second.setFtc(PalabrasIndice->second.getFtc() + 1);
                                }
                                else // Si el termino no existe lo que vamos a hacer es insertarlo
                                {
                                    // Incrementamos las palabras diferentes que tenemos en el documento
                                    informacioDocumentoAnalizo.setNumPalDiferentes(informacioDocumentoAnalizo.getNumPalDiferentes() + 1);
                                    // Incrementamos nuestra coleccion de palabras diferentes
                                    informacionColeccionDocs.setNumTotalPalDiferentes(informacionColeccionDocs.getNumTotalPalDiferentes() + 1);

                                    // Esta la informacion que tenemos del termino en general
                                    InformacionTermino informacionTodosTerminos;
                                    // Esta es la informacion del termino en el documento
                                    InfTermDoc informacionTerminoEnDocumento;

                                    // Le asignamos la informacion inicial al termino
                                    informacionTerminoEnDocumento.setFt(1);
                                    informacionTerminoEnDocumento.setPosTerm(posTermino);
                                    // Inicializamos la informacion global del termino
                                    informacionTodosTerminos.setFtc(1);
                                    informacionTodosTerminos.insertarDoc(informacioDocumentoAnalizo.getIdDoc(), informacionTerminoEnDocumento);
                                    // Insertamos el termino en el indice
                                    indice.insert(pair<string, InformacionTermino>((*itTokens), informacionTodosTerminos));
                                    // cout << informacionTodosTerminos << endl;
                                    // cout << informacionTerminoEnDocumento << endl;
                                }
                            }
                            ++posTermino;
                        }

                        lineaAnalizo.clear();
                    }
                    // cout << "he analizado bien" << endl;
                    //  Ahora que tenemos todos los nuevos terminos y los documentos debemos actualizar nuestra coleccion
                    informacionColeccionDocs.setNumTotalPal(informacionColeccionDocs.getNumTotalPal() + informacioDocumentoAnalizo.getNumPal());
                    informacionColeccionDocs.setNumTotalPalSinParada(informacionColeccionDocs.getNumTotalPalSinParada() + informacioDocumentoAnalizo.getNumPalSinParada());
                    informacionColeccionDocs.setNumTotalPalDiferentes(informacionColeccionDocs.getNumTotalPalDiferentes() + informacioDocumentoAnalizo.getNumPalDiferentes());
                    // cout << "he seteado el total de palabras" << endl;

                    // Insertamos el documento en la coleccion
                    // cout << documentoAnalizo << endl;
                    // cout << informacioDocumentoAnalizo << endl;
                    this->indiceDocs.insert({documentoAnalizo, informacioDocumentoAnalizo});
                    // cout << "todo de locos " << endl;
                    documentoAnalizoFich.close();
                }
            }
        }

        // Cerramos el documento con los nombres
        nombresDocumentos.close();
    }
    else
    {
        cerr << "ERROR: no se ha podido abrir el archivo que contiene los nombres de los documentos: " << ficheroDocumentos << "\n";
    }

    // Si todo ha salido segun lo previsto :)
    return true;
}

bool IndexadorHash::IndexarDirectorio(const string &dirAIndexar)
{
    struct stat dir;
    int err = stat(dirAIndexar.c_str(), &dir);

    if (err == -1 || !S_ISDIR(dir.st_mode))
    {
        return false;
    }
    else
    {
        // Le decimos al directorio que no coja los archivos con extension .tk por si acaso
        string cmd = "find " + dirAIndexar + " -follow -type f -not -name \"*.tk\" | sort > lista_fich_indexar";
        system(cmd.c_str());
        return Indexar("lista_fich_indexar");
    }
}

bool IndexadorHash::GuardarIndexacion() const
{
    // Obtenemos la informacion del directorio
    struct stat infoDirectorio;
    stat(this->directorioIndice.c_str(), &infoDirectorio);

    // En el caso de que no exista el directorio tendremos que crearlo
    if (!S_ISDIR(infoDirectorio.st_mode))
    {
        system(string("mkdir " + this->directorioIndice).c_str());
    }

    ofstream ficheroCreado(this->directorioIndice + "/" + "indice", ofstream::out);

    if (!ficheroCreado.good())
    {
        cerr << "ERROR: no se ha podido crear el archivo de indice: " << this->directorioIndice << "/"
             << "indice" << endl;
        return false;
    }
    else
    {
        ficheroCreado << this->almacenarPosTerm << "\n";
        ficheroCreado << this->almacenarEnDisco << "\n";
        ficheroCreado << this->ficheroStopWords << "\n";
        // Escribimos todas las stopWords en el fichero
        for (auto it = this->stopWords.begin(); it != this->stopWords.end(); ++it)
        {
            ficheroCreado << *it << " ";
        }
        ficheroCreado << "\n";

        // Escribimos toda la informacion de la pregunta
        ficheroCreado << this->infPregunta.getNumTotalPal() << "\n";
        ficheroCreado << this->infPregunta.getNumTotalPalDiferentes() << "\n";
        ficheroCreado << this->infPregunta.getNumTotalPalSinParada() << "\n";
        ficheroCreado << this->pregunta << "\n";
        ficheroCreado << indicePregunta.size() << "\n";
        // Escribimos la informacion de todos los terminos de la pregunta
        for (auto it = indicePregunta.begin(); it != indicePregunta.end(); ++it)
        {
            ficheroCreado << it->first << "\n";
            ficheroCreado << it->second.getFt() << "\n";
            for(auto itTerm = it->second.getPosTerm().begin(); itTerm != it->second.getPosTerm().end(); ++itTerm)
            {
                ficheroCreado << (*itTerm) << " ";
            }
            //El intro nos indica que pasamos al siguiente termino
            ficheroCreado << "\n";
        }
        //ME HE QUEDADO LINEA 327
    }
}

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