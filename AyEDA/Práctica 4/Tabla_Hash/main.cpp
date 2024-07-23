#include "tabla_hash.hpp"
#include "matricula.h"
#include <iostream>
#include <curses.h>

using namespace std;

void create_banco (double factor_carga, int celdas, int bloques, vector<Matricula> &banco_pruebas)
{
    int size=round(factor_carga*celdas*bloques);
    banco_pruebas.resize(2*size);
}

int main(void)
{
    int celdas=0,bloques=0,dispersion_type=0,exploracion_type=0, numero_pruebas=0;
    double factor_carga;
    bool check=false;
    vector<Matricula> banco_pruebas;
    srand ((unsigned int)time(NULL));
    cout << "Introduza el numero de celdas: ";
    cin >> celdas;
    cout << endl << "Introduza el numero de bloques: ";
    cin >> bloques;
    do
    {
        if(check)
            cout << endl << "Introdujo una respuesta incorrecta" << endl;
        cout << endl << "Opciones de función de dispersión: " << endl;
        cout << endl << "[1] Modulo " << "[2] Suma " << "[3] Pseudo-aleatoria " << endl;
        cout << endl << "Introduzca la función de dispersión que desee: ";
        cin >> dispersion_type;
        check=true;
    }while(dispersion_type!=1 && dispersion_type!=2 && dispersion_type!=3);
    check=false;
    do
    {
        if(check)
            cout << endl << "Introdujo una respuesta incorrecta, las validas son [lineal], [cuadratica], [dispersion doble] o [re-dispersion]" << endl;
        cout << endl << "Opciones de función de exploración: " << endl;
        cout << endl << "[1] Lineal " << "[2] Cuadratica " << "[3] Dispersión Doble " << "[4] Re-Dispersión " << endl;
        cout << endl << "Introduzca la función de exploración que desee: ";
        cin >> exploracion_type;
        check=true;
    }while(exploracion_type!=1 && exploracion_type!=2 && exploracion_type!=3 && exploracion_type!=4);
    check=false;
    Tabla_Hash <Matricula> A(celdas,bloques,dispersion_type, exploracion_type);
    cout << endl << "Introduzca el factor de carga (Valor entre 0 y 1): ";
    cin >> factor_carga;
    cout << endl << "Introduzca el numero de pruebas: ";
    cin >> numero_pruebas;
    create_banco(factor_carga, celdas, bloques, banco_pruebas);
    for(int i=0; i<banco_pruebas.size()/2; i++)
        A.insertar(banco_pruebas[i]);
    int pos=-1, rand_pos=-1;
    for(int i=0; i<numero_pruebas; i++)
    {
        while(pos==rand_pos)
            rand_pos=rand()%banco_pruebas.size()/2;
        pos=rand_pos;
        A.buscar(banco_pruebas[pos]);
    }
    vector<int> comparaciones_busqueda=A.get_n_busquedas();
    A.clear_n_busquedas();
    pos=-1, rand_pos=-1;
    for(int i=0; i<numero_pruebas; i++)
    {
        while(pos==rand_pos)
            rand_pos=(banco_pruebas.size()/2)+rand()%(banco_pruebas.size()-(banco_pruebas.size()/2));
        pos=rand_pos;
        A.buscar(banco_pruebas[pos]);
    }
    vector<int> comparaciones_insercion=A.get_n_busquedas();
    A.clear_n_busquedas();
    int min_b=comparaciones_busqueda[0];
    int min_i=comparaciones_insercion[0];
    int max_b=comparaciones_busqueda[0];
    int max_i=comparaciones_insercion[0];
    int acumulador_b=comparaciones_busqueda[0], acumulador_i=comparaciones_insercion[0];
    for(int i=1; i<numero_pruebas; i++)
    {
        if(comparaciones_busqueda[i]<min_b)
            min_b=comparaciones_busqueda[i];
        else if(comparaciones_busqueda[i]>max_b)
            max_b=comparaciones_busqueda[i];
        acumulador_b+=comparaciones_busqueda[i];
        if(comparaciones_insercion[i]<min_i)
            min_i=comparaciones_insercion[i];
        else if(comparaciones_insercion[i]>max_i)
            max_i=comparaciones_insercion[i];
        acumulador_i+=comparaciones_insercion[i];
    }
    int media_b=acumulador_b/numero_pruebas;
    int media_i=acumulador_i/numero_pruebas;
    system("clear");
    cout << "Celdas\t" << "Bloques\t" << "Exploracion\t" << "Carga\t" << "Pruebas" << endl;
    cout << celdas << "\t" << bloques << "\t";
    switch (exploracion_type)
    {
        case 1:
            cout << "Lineal";
            break;
        case 2:
            cout << "Cuadrática";
            break;
        case 3:
            cout << "Dispersión doble";
            break;
        case 4:
            cout << "Re-dispersión";
            break;
        default:
            break;
    }
    cout << "\t\t" << factor_carga << "\t" << numero_pruebas << endl << endl;
    cout << "\t\tNumero de Comparaciones" << endl;
    cout << "\t\tMínimo" << "\tMedio" << "\tMáximo" << endl;
    cout << "Búsquedas\t" << min_b << "\t" << media_b << "\t" << max_b << endl;
    cout << "Inserción\t" << min_i << "\t" << media_i << "\t" << max_i << endl;
    return(9);
}
