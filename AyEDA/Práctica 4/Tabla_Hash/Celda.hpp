#ifndef Celda_h
#define Celda_h
#include <stdio.h>
#include <vector>
#include "matricula.h"

using namespace std;

template<class T>
class Celda
{
    private:
        vector<T> Matriculas;
        int nBloques;
    public:
        //Celda(int introduced);
        void resize_celda(int introduced);
        vector<T> get_vector(void);
        bool buscar(T introduced);
        bool insertar(T introduced);
        bool empty(void);
};

/*template<class T>
Celda<T>::Celda(int introduced)
{
    Matriculas.resize(introduced);
    nBloques=introduced;
    for(int i=0; i<introduced; i++)
        Matriculas[i]=T(0);
}*/

template<class T>
void Celda<T>::resize_celda(int introduced)
{
    Matriculas.resize(introduced);
    nBloques=introduced;
    for(int i=0; i<introduced; i++)
        Matriculas[i]=T(0);
}

template<class T>
bool Celda<T>::insertar(T introduced)
{
    if((Matriculas[Matriculas.size()-1].get_ocupado())==false)
        for(int i=0; i<Matriculas.size(); i++)
            if((Matriculas[i].get_ocupado())==false)
            {
                Matriculas[i]=introduced;
                return(true);
            }
    return(false);
}

template<class T>
vector<T> Celda<T>::get_vector(void)
{
    return (Matriculas);
}

template<class T>
bool Celda<T>::buscar(T introduced)
{
    for(int i=0; i<Matriculas.size(); i++)
        if(Matriculas[i]==introduced)
            return(true);
    return (false);
}

template<class T>
bool Celda<T>::empty(void)
{
    for(int i=0; i<Matriculas.size(); i++)
        if((Matriculas[i].get_ocupado())==false)
            return(true);
    return (false);
}

#endif //Celda_h
