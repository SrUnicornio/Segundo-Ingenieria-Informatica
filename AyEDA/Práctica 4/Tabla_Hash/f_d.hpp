#ifndef f_d_hpp
#define f_d_hpp

#include <stdio.h>
#include "matricula.h"

using namespace std;

class f_d
{
    protected:
        int pos;
        int n_celdas;
    public:
        void set_n(int n_i);
        virtual int position (Matricula introduced)=0;
        virtual int get_pos (void);
};

class modulo: public f_d
{
    private:
    public:
        int position (Matricula introduced);
};

class suma: public f_d
{
    private:
    public:
        int position (Matricula introduced);
};

class pseudo_aleatoria: public f_d
{
    private:
    public:
        int position (Matricula introduced);
};

void f_d::set_n(int n_i)
{
    n_celdas=n_i-1;
}

int f_d::get_pos(void)
{
    return (pos);
}

int modulo::position(Matricula introduced)
{
    int primo=n_celdas,contador=0, pos_aux=0;
    do
    {
        contador=0;
        primo--;
        for(int i=1; i<primo+1;i++)
        {
            if(primo%i==0)
                contador++;
            
        }
    }while(contador!=2);
    pos_aux=long(introduced) % primo;
    pos=pos_aux;
    return(pos_aux);
}

int suma::position(Matricula introduced)
{
    long d = 0, x = long(introduced), y = 0;
    while (x > 0)
    {
        y = x % 10;
        d = d + y;
        x = x / 10;
    }
    return (d % n_celdas);
}

int pseudo_aleatoria::position(Matricula introduced)
{
    srand(long(introduced));
    return (rand() % n_celdas);
}

#endif /* f_d_hpp */
