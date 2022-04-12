#include <iostream>
#include <random>
#include <chrono>
using namespace std;

//para un circulo de radio 1. Coordenadas entre -1 y 1
//de cualquier coordenada sacamos el valor absoluto.

static std::default_random_engine e;
static std::uniform_real_distribution<> dis(-1, 1);
float get_random()
{
    
    e.seed(std::chrono::system_clock::now().time_since_epoch().count());
    
    return dis(e);
}
int tries;
int success;
int n;
double x;
double y;
double r;
int main(){
    cout << "Monte carlo pi. Introduce el numero de pruebas:";
    cin >> n;

    for(int i = 0; i<n;i++){
        x = get_random();
        y = get_random();
        r = (double)y*(double)y+(double)x*(double)x;
        if((double)r*(double)r<1){success++;};
        tries++;
    };

    cout << "Tries: " << tries << " Success: " << success << "\n";
    double pi = (double)4*(double)success/(double)tries;
    cout << "Pi = " << pi <<"\n";

    return 0;
}
