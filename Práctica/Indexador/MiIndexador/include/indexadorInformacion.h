#ifndef _INDEXADORINFORMACION_H_
#define _INDEXADORINFORMACION_H_

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

class InformacionTermino
{
    friend ostream &operator<<(ostream &s, const InformacionTermino &p)
    {
        s << "Frecuencia total : " << p.ftc << "\tfd : " << p.l_docs.size();
        // A continuación se mostrarían todos los elementos de l_docs
        for (auto &doc : p.l_docs)
            s << "\tId.Doc : " << doc.first << "\t" << doc.second;
        return s;
    }

public:
    InformacionTermino(const InformacionTermino &);
    InformacionTermino();  // Inicializa ftc = 0
    ~InformacionTermino(); // Pone ftc = 0 y vacía l_docs
    InformacionTermino &operator=(const InformacionTermino &);
    // Añadir cuantos métodos se consideren necesarios para manejar la parte privada de la clase
private:
    // Frecuencia total del término en la colección
    int ftc;
    /** Tabla Hash que se accederá por el id del documento, devolviendo un
     *  objeto de la clase InfTermDoc que contiene toda la información de
     *  aparición del término en el documento
     * */
    unordered_map<int, InfTermDoc> l_docs;
};
/////////////////////////////////////////////////////////////////////////////////////////

#endif