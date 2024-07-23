#ifndef f_e_hpp
#define f_e_hpp

#include <stdio.h>
#include <math.h>
#include "f_d.hpp"
#include <vector>

using namespace std;

class f_e
{
    protected:
        int pos;
        int n;
    public:
        void set_n(int n_i);
        virtual int position (Matricula introduced, int n_intentos);
};

class lineal: public f_e
{
    public:
        int position (Matricula introduced, int n_intentos);
};

class cuadratica: public f_e
{
    public:
        int position (Matricula introduced, int n_intentos);
};

class dispersion_doble: public f_e
{
    private:
        int disp_selec;
    public:
        dispersion_doble(int introduced);
        int position (Matricula introduced, int n_intentos);
};

class re_dispersion: public f_e
{
    private:
        int disp_selec;
    public:
        re_dispersion(int introduced);
        int position (Matricula introduced, int n_intentos);
};

void f_e::set_n(int n_i)
{
    n=n_i-1;
}

int f_e::position(Matricula introduced, int n_intentos)
{
    return(pos);
}

int lineal::position(Matricula introduced, int n_intentos)
{
    long suma=0;
    suma=long(introduced)+n_intentos;
    return(suma%n);
}

int cuadratica::position(Matricula introduced, int n_intentos)
{
    long suma=0;
    suma=long(introduced)+pow(n_intentos,2);
    return(suma%n);
}

dispersion_doble::dispersion_doble(int introduced)
{
    disp_selec=introduced;
}

int dispersion_doble::position(Matricula introduced, int n_intentos)
{
    f_d* dispersion_1;
    f_d* dispersion_2;
    switch (disp_selec)
    {
        case 1:
            dispersion_1=new modulo;
            dispersion_2=new suma;
            break;
        case 2:
            dispersion_1=new suma;
            dispersion_2=new pseudo_aleatoria;
            break;
        case 3:
            dispersion_1=new pseudo_aleatoria;
            dispersion_2=new modulo;
            break;
        default:
            break;
    }
    int producto=0;
    producto=(n_intentos*(dispersion_2->position(introduced)));
    int suma=dispersion_1->position(introduced)+producto;
    return (suma%n);
}

re_dispersion::re_dispersion(int introduced)
{
    disp_selec=introduced;
}

int re_dispersion::position(Matricula introduced, int n_intentos)
{
    f_d* dispersion_1;
    f_d* dispersion_2;
    switch (disp_selec)
    {
        case 1:
            dispersion_1=new modulo;
            break;
        case 2:
            dispersion_1=new suma;
            break;
        case 3:
            dispersion_1=new pseudo_aleatoria;
            break;
        default:
            break;
    }
    switch (n_intentos)
    {
        case 1:
            dispersion_2=new modulo;
            break;
        case 2:
            dispersion_2=new suma;
            break;
        case 3:
            dispersion_2=new pseudo_aleatoria;
            break;
        default:
            dispersion_2=new pseudo_aleatoria;
            break;
    }
    int producto=0;
    producto=n_intentos*((dispersion_1->position(introduced))+(dispersion_2->position(introduced)));
    int suma=dispersion_1->position(introduced)+producto;
    return (suma%n);
}

#endif /* f_e_hpp */
