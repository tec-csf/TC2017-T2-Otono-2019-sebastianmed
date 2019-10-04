/*
 
 PROBLEMA:
 Supongamos que tenemos n hombres y n mujeres y dos matrices M y H que contienen las preferencias de los unos por los otros. Más concretamente, la fila M[i,·] es una ordenación (de mayor a menor) de las mujeres según las preferencias del i-ésimo hombre y, análogamente, la fila H[i,·] es una ordenación (de mayor a menor) de los hombres según las preferencias de la i-ésima mujer.
 
 Diseñe un algoritmo que encuentre, si es que existe, un emparejamiento de hombres y mujeres tal que todas las parejas formadas sean estables. Una pareja (h,m) es estable si no se da ninguna de estas dos circunstancias:
 
 > Existe una mujer m’ (que forma la pareja (h’,m’)) tal que el hombre h la prefiere sobre la mujer m y además la mujer m’ también prefiere a h sobre h’.
 > Existe un hombre h” (que forma la pareja (h”,m”)) tal que la mujer m lo prefiere sobre el hombre h y además el hombre h” también prefiere a m sobre la mujer m”.
  
 Algoritmo backtracking
 
 Complejidad: O(n^2) por el recorrido de la matriz (se necesita un bucle dentro de otro)
 
 Referencia: GeeksForGeeks

 */



#include <iostream>
#include <vector>
using namespace std;

bool mejorHombre(vector<vector<int>> preferencia, int hombre, int mujer, int hombre1, int n);
void asignarParejas(vector<vector<int>> preferencia, int columnas);
bool mejorHombre(vector<vector<int>> preferencia, int hombre, int mujer, int otroHombre, int columnas)
{
    for (int i = 0; i < columnas; i++)
    {
        if (preferencia[mujer][i] == otroHombre)
        {
            return true;
        }
        if (preferencia[mujer][i] == hombre)
        {
            return false;
        }
        
    }
    
    return false;
}

void asignarParejas(vector<vector<int>> preferencia, int columnas)
{
    vector<int> parejaMujeres;
    vector<bool> asignadosH;
    
    for (int i = 0; i < columnas; i++)
    {
        parejaMujeres.push_back(-1);
        asignadosH.push_back(false);
    }
    
    int solteros = columnas;
    
    while (solteros > 0)
    {
        int hombre=0;
        
        for (int i = 0; i < columnas; i++)
        {
            if (asignadosH[i] == false)
            {
                hombre = i;
                i = columnas;
            }
        }
        
        for (int i = 0; (i < columnas) && (asignadosH[hombre] == false); i++)
        {
            int mujer = preferencia[hombre][i];
            if (parejaMujeres[mujer - columnas] == -1)
            {
                parejaMujeres[mujer - columnas] = hombre;
                asignadosH[hombre] = true;
                solteros--;
            }
            
            else
            {
                int otroHombre = parejaMujeres[mujer - columnas];
                
                if (mejorHombre(preferencia, hombre, mujer, otroHombre, columnas) == false)
                {
                    parejaMujeres[mujer - columnas] = hombre;
                    asignadosH[hombre] = true;
                    asignadosH[otroHombre] = false;
                }
            }
        }
    }
    
    for (int i = 0; i < columnas; i++)
    {
        cout << "Mujer " << i + columnas << " asignada al hombre " << parejaMujeres[i] << endl;
    }
}


int main()
{
    int columnas = 4;
    vector<vector<int>> preferencia(2 * columnas, vector<int>(columnas));
    
    preferencia =
    {
        //hombres
        { 7, 4, 5, 6 }, //hombre 0
        { 5, 6, 7, 4 }, //hombre 1
        { 4, 5, 6, 7 }, //hombre 2
        { 6, 7, 4, 5 }, //hombre 3
        
        //mujeres
        { 1, 2, 3, 0 }, //mujer 4
        { 2, 3, 0, 1 }, //mujer 5
        { 0, 1, 2, 3 }, //mujer 6
        { 3, 0, 1, 2 }, //mujer 7
        
    };
    
    asignarParejas(preferencia, columnas);
}
