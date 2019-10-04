/*
 
 PROBLEMA:
 Dada una secuencia X={x1 x2 ... xm}, se dice que Z={z1 z2 ... zk} es una subsecuencia de X (siendo k ≤ m) si existe una secuencia creciente {i1 i2 ... ik} de índices de X tales que para todo j = 1, 2, ..., k tenemos xij = zj.
 
 Dadas dos secuencias X e Y, se dice que Z es una subsecuencia común de X e Y si es subsecuencia de X y subsecuencia de Y.
 
 Algoritmo ávido.
 
 Complejidad: O(n+m) debido a que los bucles dependen exclusivamente de la longitud de los arreglos (n y m)
 
 Referencia: GeeksforGeeks y visualización en http://lcs-demo.sourceforge.net/
 
 */

#include <iostream>
#define CONSTANT 100

using namespace std;

int tmp[CONSTANT][CONSTANT];
int aux[CONSTANT][CONSTANT];

void LCS(char * X, char * Y, int sizeX, int sizeY);
void imprimir(char * x, int i, int j);


int main() {
    
    const int sizeX = 9;
    const int sizeY = 6;
    char xchar[sizeX] = {'S', 'e', 'b', 'a', 's', 't', 'i', 'a', 'n'};
    char ychar[sizeY] = {'M', 'e', 'd', 'i', 'n', 'a'};

    
    LCS(xchar, ychar, sizeX, sizeY);
    cout << "Resultado: ";
    imprimir(xchar, sizeX-1, sizeY-1);
    cout << endl;
    
    return 0;
}

void LCS(char * X, char * Y, int sizeX, int sizeY)
{
    int m,n;
    m = sizeX;
    n = sizeY;
    
    for(int i = 1; i <= m; i++)
        tmp[i][0] = 0;
    
    for(int j = 0; j <= n; j++)
        tmp[0][j] = 0;
    
    for(int i = 1; i <= m; i++)
    {
        for(int j = 1; j <= n; j++)
        {
            if (X[i] == Y[j])
            {
                tmp[i][j] = tmp[i-1][j-1] + 1;
                aux[i][j] = 1;
            } else {
                if (tmp[i-1][j] >= tmp[i][j-1])
                {
                    tmp[i][j] = tmp[i-1][j];
                    aux[i][j] = 2;
                } else {
                    tmp[i][j] = tmp[i][j-1];
                    aux[i][j] = 3;
                }
            }
        }
    }
}

void imprimir(char * x, int i, int j)
{
    if (i == 0 || j == 0)
        return;
    
    if (aux[i][j] == 1)
    {
        imprimir(x, i - 1, j - 1);
        cout << x[i];
    } else {
        if (aux[i][j] == 2){
            imprimir(x, i - 1, j);
        }
        else {
            imprimir(x, i, j -1);
        }
    }
}
