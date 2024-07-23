#ifndef TABLA_HASH_H
#define TABLA_HASH_H
#include "matricula.h"
#include <vector>
#include <iostream>
#include <cmath>
#include <time.h>
#include "Celda.hpp"
#include "f_d.hpp"
#include "f_e.hpp"

using namespace std;

template<class T>
class Tabla_Hash
{
    private:
        vector< Celda<T> > tablero;
        int numero_pruebas,n,m;
        f_d* dispersion;
        f_e* exploracion;
        vector<int> n_busquedas;
    
    public:
        Tabla_Hash(int n_i, int m_i, int d_t, int e_t);
        bool buscar(T introduced);
        bool insertar(T introduced);
        vector<int> get_n_busquedas(void);
        void clear_n_busquedas(void);
};

template<class T>
Tabla_Hash<T>::Tabla_Hash(int n_i, int m_i, int d_t, int e_t)
{
    tablero.resize(n_i);
    for(int i=0; i<n_i; i++)
        tablero[i].resize_celda(m_i);
    n=n_i;
    m=m_i;
    switch (d_t)
    {
        case 1:
            dispersion=new modulo;
            break;
        case 2:
            dispersion=new suma;
            break;
        case 3:
            dispersion=new pseudo_aleatoria;
            break;
        default:
            break;
    }
    switch (e_t)
    {
        case 1:
            exploracion=new lineal;
            break;
        case 2:
            exploracion=new cuadratica;
            break;
        case 3:
            exploracion=new dispersion_doble(d_t);
            break;
        case 4:
            exploracion=new re_dispersion(d_t);
            break;
        default:
            break;
    }
    dispersion->set_n(n);
    exploracion->set_n(n);
}

template<class T>
bool Tabla_Hash<T>::insertar(T introduced)
{
    int n_intentos=1, pos=0;
    pos=dispersion->position(introduced);
    while((tablero[pos].insertar(introduced))==false)
    {
        pos=exploracion->position(introduced, n_intentos);
        n_intentos++;
    }
    return(true);
}

template<class T>
bool Tabla_Hash<T>::buscar(T introduced)
{
    int n_intentos=1, pos=0, n_comparaciones=0;
    pos=dispersion->position(introduced);
    if((tablero[pos].buscar(introduced))==false)
    {
        while((tablero[pos].buscar(introduced))==false && (tablero[pos].empty())==false)
        {
            for(int i=0; i<tablero[pos].get_vector().size();i++)
            {
                n_comparaciones+=tablero[pos].get_vector()[i].get_comparaciones();
                tablero[pos].get_vector()[i].clear_comparaciones();
            }
            pos=exploracion->position(introduced, n_intentos);
            n_intentos++;
        }
    }
    for(int i=0; i<tablero[pos].get_vector().size();i++)
    {
        n_comparaciones+=tablero[pos].get_vector()[i].get_comparaciones();
        tablero[pos].get_vector()[i].clear_comparaciones();
    }
    n_busquedas.push_back(n_comparaciones);
    return(true);
}

template<class T>
vector<int> Tabla_Hash<T>::get_n_busquedas()
{
    return(n_busquedas);
}

template<class T>
void Tabla_Hash<T>::clear_n_busquedas(void)
{
    n_busquedas.clear();
}

#endif // TABLA_HASH_H
