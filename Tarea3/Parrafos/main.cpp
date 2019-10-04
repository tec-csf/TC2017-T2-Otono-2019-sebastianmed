/*
 
 PROBLEMA:
 Dada una secuencia de palabras p1, p2, ..., pn de longitudes l1, l2, ..., ln se desea agruparlas en líneas de longitud L. Las palabras están separadas por espacios cuya amplitud ideal (en milímetros) es b, pero los espacios pueden reducirse o ampliarse si es necesario (aunque sin solapamiento de palabras), de tal forma que una línea pi, pi+1, ..., pj tenga exactamente longitud L. Sin embargo, existe una penalización por reducción o ampliación en el número total de espacios que aparecen o desaparecen. El costo de fijar la línea pi, pi+1, ..., pj es (j – i)|b’ – b|, siendo b’ el ancho real de los espacios, es decir (L – li – li+1 – ... – lj)/(j – i). No obstante, si j = n (la última palabra) el costo será cero a menos que b’ < b (ya que no es necesario ampliar la última línea).

 Es un algoritmo ávido. 

 Complejidad: O(n) debido a que el bucle for solamente va a pasar hasta n-1
 
 Referencia: Capítulo 4. Algoritmos Ávidos. http://www.lcc.uma.es/~av/Libro/CAP4.pdf
 
 */

#include <iostream>
#include <vector>
#include <math.h>
using namespace std;

float reducirCosto(float red, float sizeI, int j, int i);
float ampliarCosto(float amp, float sizeI, int j, int i);

int main(int argc, const char * argv[]) {
    
    vector<string> text = {"Hola", "mi", "nombre", "es", "Sebastian"};
    vector<int> longWord;
    vector<string> line;
    
    float longLine = 27;
    float word = 0;
    float longWordAmpl = 0;
    
    float sizeI = 1;
    float sobrante;
    
    int i = 0;
    int j;
    
    float red;
    float amp;
    
    bool foo = true;
    
    for (int i = 0; i < text.size(); i++)
    {
        longWord.push_back( int(text[i].length()) );
    }
    
    for (j = 0; j < text.size(); j++)
    {
        if (foo)
        {
            i = j;
            foo = false;
            line.clear();
            word = 0;
        }
        
        word += longWord[j];
        longWordAmpl = word + ((j - i) * sizeI);
        
        if (longWordAmpl > longLine)
        {
            sobrante = longWordAmpl - longLine;
            red = sizeI - (sobrante / (j - i));
            amp = sizeI + (longLine - ( (word - longWord[j]) + (sizeI * (j - i - 1) ) ) ) / (j - i - 1);
            
            if ( ampliarCosto(amp,sizeI,j,i) > reducirCosto(red, sizeI,j,i) && red > 0 )
            {
                line.push_back(text[j]);
                
                for (int i = 0; i < line.size(); i++)
                {
                    cout << line[i] << " ";
                }
                
                cout << " " <<  " ---> reducción de : " << red << " mm." << endl;
                foo = true;
            }
            else {
                for (int i = 0; i < line.size(); i++)
                {
                    cout << line[i] << " ";
                }
                
                cout << " " << " ---> amplitud de : " << amp << " mm." << endl;
                j--;
                foo = true;
            }
        }
        line.push_back(text[j]);
    }
    
    for (int i = 0; i < line.size(); i++)
    {
        std::cout << line[i] << " ";
    }
    
    cout << endl;
    
}



float reducirCosto(float red, float amplitudI, int j, int i)
{
    return ( fabs(red - amplitudI) * (j - i) );
}

float ampliarCosto(float amp, float amplitudI, int j, int i)
{
    return ( fabs(amp - amplitudI) * (j - i - 1) );
}
