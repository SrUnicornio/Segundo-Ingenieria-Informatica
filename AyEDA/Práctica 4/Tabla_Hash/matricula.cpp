#include "matricula.h"

Matricula::Matricula(void)
{
    comparaciones=0;
    matricula_text.clear();
    matricula_text.resize(7);
    vector<unsigned> numbers;
    vector<char> letters;
    letters.resize(3);
    numbers.resize(4);
    for(int i=0; i<4; i++)
    {
        numbers[i]=random_number(48,57);
        if(i!=3)
            letters[i]=random_number(65,90);
    }
    for(int i=0; i<7; i++)
    {
        if(i<4)
            matricula_text[i]=numbers[i];
        else
            matricula_text[i]=letters[i-4];
    }
}

Matricula::Matricula(int introduced)
{
    comparaciones=0;
    matricula_text.clear();
    matricula_text.push_back('&');
}

unsigned Matricula::random_number(int inf, int sup)
{
    unsigned random_number;
    random_number= inf + rand() % (sup-inf);
    return(random_number);
}

bool Matricula::get_ocupado(void)
{
    bool check=false;
    if(matricula_text[0]!='&')
        check=true;
    return (check);
}

int Matricula::get_comparaciones(void)
{
    return (comparaciones);
}

void Matricula::clear_comparaciones(void)
{
    comparaciones=0;
}

Matricula::operator unsigned long()
{
    unsigned long ascii=0;
    for(int i=0; i<7;i++)
        ascii+=(int(matricula_text[i])*pow(128,i));
    return(ascii);
}

Matricula Matricula::operator=(Matricula introduced)
{
    matricula_text=introduced.get_vector();
    return (*this);
}

bool Matricula::operator==(Matricula introduced)
{
    comparaciones=1;
    if(matricula_text[0]=='&')
        return (false);
    else
        for(int i=0; i<matricula_text.size(); i++)
            if(matricula_text[i]!=introduced.get_vector()[i])
                return (false);
    return (true);
}

vector<char> Matricula::get_vector(void)
{
    return(matricula_text);
}


