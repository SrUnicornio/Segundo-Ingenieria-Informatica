#ifndef MATRICULA_H
#define MATRICULA_H
#include <stdlib.h>
#include <vector>
#include <time.h>
#include <iostream>
#include <string>
#include <math.h>

using namespace std;

class Matricula
{
    private:
        vector<char> matricula_text;
        int comparaciones;
    public:
        Matricula(void);
        Matricula(int introduced);
        unsigned random_number(int inf, int sup);
        operator unsigned long (void);
        bool get_ocupado(void);
        vector<char> get_vector(void);
        int get_comparaciones (void);
        void clear_comparaciones (void);
        Matricula operator=(Matricula introduced);
        bool operator==(Matricula introduced);
};

#endif // MATRICULA_H
