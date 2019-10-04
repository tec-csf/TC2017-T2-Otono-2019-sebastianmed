/*
 
 PROBLEMA:
 Un camionero conduce desde DF a Acapulco siguiendo una ruta dada y llevando un camión que le permite, con el tanque de gasolina lleno, recorrer n kilómetros sin parar. El camionero dispone de un mapa de carreteras que le indica las distancias entre las gasolineras que hay en su ruta. Como va con prisa, el camionero desea detenerse a abastecer gasolina el menor número de veces posible.

 Es un algoritmo ávido.
 
 Complejidad: O(n) debido a que el problema solo tiene un bucle para contar el número de estaciones de gas
*/

#include <iostream>
using namespace std;

void calculoGas(int kmRecorridos, int n, int *gas, int estaciones);

int main(){
    
    int n = 2;
    const int length = 8;
    int gasolinera[length] = {1, 2, 3, 5, 7, 11, 15, 19};
    calculoGas(0, n, gasolinera, 8);
    
    return 0;
}

void calculoGas(int kmRecorridos, int n, int *gas, int estaciones){
    int i = 0;
    int kmRecorridost = 0;
    while(estaciones > i){
        if(n-kmRecorridos == (gas[i+1] - gas[i])|| n-1 == (gas[i+1] - gas[i])){
            cout << "Parar en gasolina " << gas[i] << endl;
            kmRecorridos = 0;
            
        }else{
            cout << "No se tiene que poner gasolina" << endl;
            
        }
        i++;
        kmRecorridos++;
        kmRecorridost++;
        
    }
    
}
