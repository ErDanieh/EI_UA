#include "indexadorInformacion.h"

// Constructor de la clase Fecha
Fecha::Fecha()
{
    this->dia = 0;
    this->mes = 0;
    this->anyo = 0;
    this->hora = 0;
    this->min = 0;
    this->seg = 0;
}

// Constructor parametrizado
Fecha::Fecha(struct tm *clock)
{
    this->dia = clock->tm_mday;
    this->mes = clock->tm_mon;
    this->anyo = clock->tm_year;
    this->hora = clock->tm_hour;
    this->min = clock->tm_min;
    this->seg = clock->tm_sec;
}

Fecha::Fecha(const Fecha &f)
{
    this->dia = f.dia;
    this->mes = f.mes;
    this->anyo = f.anyo;
    this->hora = f.hora;
    this->min = f.min;
    this->seg = f.seg;
}

// Operador menor que para comparar dos fechas
bool Fecha::operator<(const Fecha &f) const
{
    if (this->anyo < f.anyo)
        return true;
    else if (this->anyo > f.anyo)
        return false;
    else if (this->mes < f.mes)
        return true;
    else if (this->mes > f.mes)
        return false;
    else if (this->dia < f.dia)
        return true;
    else if (this->dia > f.dia)
        return false;
    else if (this->hora < f.hora)
        return true;
    else if (this->hora > f.hora)
        return false;
    else if (this->min < f.min)
        return true;
    else if (this->min > f.min)
        return false;
    else if (this->seg < f.seg)
        return true;
    else
        return false;
}

// Operador mayor que para comparar dos fechas
bool Fecha::operator>(const Fecha &f) const
{
    return !(*this < f);
}

// Operador igual para asignar los mismos valores de una fecha a otra
Fecha &Fecha::operator=(const Fecha &f)
{
    if (this != &f)
    {
        this->dia = f.dia;
        this->mes = f.mes;
        this->anyo = f.anyo;
        this->hora = f.hora;
        this->min = f.min;
        this->seg = f.seg;
    }
    return (*this);
}

////////////////////////////////////////////////////////////////////////////

// Constructor de informacionPregunta
InformacionPregunta::InformacionPregunta()
{
    this->numTotalPal = 0;
    this->numTotalPalSinParada = 0;
    this->numTotalPalDiferentes = 0;
}

// Constructor copia
InformacionPregunta::InformacionPregunta(const InformacionPregunta &p)
{
    this->numTotalPal = p.numTotalPal;
    this->numTotalPalSinParada = p.numTotalPalSinParada;
    this->numTotalPalDiferentes = p.numTotalPalDiferentes;
}

// Destructor de informacionPregunta
InformacionPregunta::~InformacionPregunta()
{
    this->numTotalPal = 0;
    this->numTotalPalDiferentes = 0;
    this->numTotalPalSinParada = 0;
}

// Operador igual para asignar los mismos valores de una informacionPregunta a otra
InformacionPregunta &InformacionPregunta::operator=(const InformacionPregunta &p)
{
    if (this != &p)
    {
        this->numTotalPal = p.numTotalPal;
        this->numTotalPalSinParada = p.numTotalPalSinParada;
        this->numTotalPalDiferentes = p.numTotalPalDiferentes;
    }
    return (*this);
}

// Operador salida
ostream &operator<<(ostream &os, const InformacionPregunta &p)
{
    os << "numTotalPal: " << p.numTotalPal << "\tnumTotalPalSinParada: " << p.numTotalPalSinParada << "\tnumTotalPalSinParada: " << p.numTotalPalDiferentes;
    return os;
}
////////////////////////////////////////////////////////////////////////////

// Constructor InformacionTerminoPregunta
InformacionTerminoPregunta::InformacionTerminoPregunta()
{
    this->ft = 0;
}

InformacionTerminoPregunta::InformacionTerminoPregunta(const InformacionTerminoPregunta &t)
{
    this->ft = t.ft;
    this->posTerm = t.posTerm;
}

InformacionTerminoPregunta::~InformacionTerminoPregunta()
{
    this->ft = 0;
    this->posTerm.clear();
}

InformacionTerminoPregunta &InformacionTerminoPregunta::operator=(const InformacionTerminoPregunta &t)
{
    if (this != &t)
    {
        this->ft = t.ft;
        this->posTerm = t.posTerm;
    }
    return (*this);
}

ostream &operator<<(ostream &os, const InformacionTerminoPregunta &t)
{
    os << "ft: " << t.ft;
    for (auto it = t.posTerm.begin(); it != t.posTerm.end(); ++it)
        os << "\t" << *it;
    return os;
}

////////////////////////////////////////////////////////////////////////////
// Constructor InfColeccionDocs
InfColeccionDocs::InfColeccionDocs()
{
    this->numDocs = 0;
    this->numTotalPal = 0;
    this->numTotalPalSinParada = 0;
    this->numTotalPalDiferentes = 0;
    this->tamBytes = 0;
}

InfColeccionDocs::InfColeccionDocs(const InfColeccionDocs &c)
{
    this->numDocs = c.numDocs;
    this->numTotalPal = c.numTotalPal;
    this->numTotalPalSinParada = c.numTotalPalSinParada;
    this->numTotalPalDiferentes = c.numTotalPalDiferentes;
    this->tamBytes = c.tamBytes;
}

InfColeccionDocs::~InfColeccionDocs()
{
    this->numDocs = 0;
    this->numTotalPal = 0;
    this->numTotalPalSinParada = 0;
    this->numTotalPalDiferentes = 0;
    this->tamBytes = 0;
}

InfColeccionDocs &InfColeccionDocs::operator=(const InfColeccionDocs &c)
{
    if (this != &c)
    {
        this->numDocs = c.numDocs;
        this->numTotalPal = c.numTotalPal;
        this->numTotalPalSinParada = c.numTotalPalSinParada;
        this->numTotalPalDiferentes = c.numTotalPalDiferentes;
        this->tamBytes = c.tamBytes;
    }
    return (*this);
}

ostream &operator<<(ostream &os, const InfColeccionDocs &c)
{
    os << "numDocs: " << c.numDocs << "\tnumTotalPal: " << c.numTotalPal
       << "\tnumTotalSinParada: " << c.numTotalPalSinParada << "\tnumTotalPalDiferentes: " << c.numTotalPalDiferentes << "\ttamBytes: " << c.tamBytes;
    return os;
}

////////////////////////////////////////////////////////////////////////////
// Constructor InfDoc
InfDoc::InfDoc()
{
    this->idDoc = 0;
    this->numPal = 0;
    this->numPalSinParada = 0;
    this->numPalDiferentes = 0;
    this->tamBytes = 0;
    this->fechaModificacion = NULL;

}

InfDoc::InfDoc(const InfDoc &d)
{
    this->idDoc = d.idDoc;
    this->numPal = d.numPal;
    this->numPalSinParada = d.numPalSinParada;
    this->numPalDiferentes = d.numPalDiferentes;
    this->tamBytes = d.tamBytes;
    this->fechaModificacion = d.fechaModificacion;
}