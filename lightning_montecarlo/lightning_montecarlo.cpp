//Sala cuadrada de 100x100. Objetos en la sala.
//dos array de 100x100 definen la sala, uno para la iluminación y otro para las colisiones
//Un rayo de luz se lanza en una dirección aleatoria, cuando choca con algo, se refleja de forma aleatoria. Cuando choca 5
//veces, termina
//Por donde va pasando, va añadiendo un valor fijo a la iluminación. Luego igual hay que dibujarla en logaritmo, who knows.
#include <iostream>
#include <random>
#include <chrono>
#include <memory.h>
#include <math.h>
#include <windows.h>
#include <fstream>

#define HEIGHT 50
#define WIDTH 100
#define N 1000
using namespace std;

//Escala de grises de blanco a negro
char greyscale[11] = " .-:=+*#%@";
char anothergreyscale[] = "  .\'`^\",:;Il!i><~+_-?][}{1)(|\\/tfjrxnuvczXYUJCLQ0OZmwqpdbkhao*#MW&8%B@$";
#define ANOTHERLENGHT 68
int mapgreyscale(int iluminacion, int maxvalue){

    int factor = maxvalue/100;
    if (iluminacion<factor*10){return 0;}else
    if (iluminacion<factor*20){return 1;}else
    if (iluminacion<factor*30){return 2;}else
    if (iluminacion<factor*40){return 3;}else
    if (iluminacion<factor*50){return 4;}else
    if (iluminacion<factor*60){return 5;}else
    if (iluminacion<factor*70){return 6;}else
    if (iluminacion<factor*80){return 7;}else
    if (iluminacion<factor*90){return 8;}else
    if (iluminacion>=factor*90){return 9;};

}

void configconsole(){
    CONSOLE_FONT_INFOEX info = {0};
    info.cbSize       = sizeof(info);
    info.dwFontSize.Y = 12;
    info.dwFontSize.X = 12;
    info.FontWeight   = FW_NORMAL;
    wcscpy(info.FaceName, L"Lucida Console");
    SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), NULL, &info);
}

int mapanothergreyscale(int iluminacion, int maxvalue){

    double factor = (double)maxvalue/(double)(ANOTHERLENGHT*100000);
    for (int i=0;i<ANOTHERLENGHT;i++){
        if (iluminacion<factor*10*i*1){return i;}
    }

}

class Sala{
    private:
        int iluminacion[HEIGHT][WIDTH] = {0};
        //columna, fila
        bool colision[HEIGHT][WIDTH] = {false};
    public:
        int maxvalue = 0;
        int e;
        Sala(){
            //Init borders
            for(int i = 0; i<WIDTH;i++){colision[0][i] = true;}
            for(int i = 0; i<HEIGHT;i++){colision[i][0] = true;}
            for(int i = 0; i<WIDTH;i++){colision[HEIGHT-1][i] = true;}
            for(int i = 0; i<HEIGHT;i++){colision[i][WIDTH-1] = true;}
        }
        //Draw=true to draw colision, false to erase it from a pixel
        void set_colision(int x, int y, bool draw = true){
            if (draw){colision[y][x]=true;}else{colision[y][x]=false;}};
        //Get if a pixel is occupied
        bool get_colision(int x, int y){
            return colision[y][x];
        };
        //Add to ilumination
        void add_iluminacion(int x, int y, int add = 100){
            iluminacion[y][x]+=add;
        }
        //hay que hacer el codigo para mapear la iluminación a un color, y mejor si no es en consola, ya que estamos.
        //Si se mapea a 255 seguro que se puede hacer que lo guarde en un archivo.
        void draw(){
            maxvalue = 0;
            for (int i = 0; i < HEIGHT;i++){
                for (int j = 0; j<WIDTH;j++){
                if (maxvalue < iluminacion[i][j]) {maxvalue = iluminacion[i][j];}

            }
            
            };
            cout << maxvalue;
            for (int i = 0; i < HEIGHT;i++){
                for (int j = 0; j<WIDTH;j++){
                    cout << anothergreyscale[mapanothergreyscale(iluminacion[i][j],maxvalue)];

            }
            cout << "\n";
            }
        }

};


float get_random()
{
    static std::default_random_engine e;
    static std::uniform_real_distribution<> dis(-1, 1);
    e.seed(std::chrono::system_clock::now().time_since_epoch().count());
    return dis(e);
}

class Rayo{
    private:
        class Sala *sala;
        float A;
        float B;
        float pte;
        bool check = false;
        bool angle = false;
        int x;
        int bouncex;
        float relx;
        int y;
        int lastmove;
        int bouncey;
        float rely;
        int steps = 1;
        bool flag = true;
        bool flag2 = true;
        bool up(){
            if (sala->get_colision(x,y+1)) {return false;}
            lastmove=1;
            y=y+1;
            return true;
        };
        bool down(){
            if (sala->get_colision(x,y-1)) {return false;}
            lastmove=2;
            y=y-1;
            return true;
        }
        bool right(){
            if (sala->get_colision(x+1,y)) {return false;}
            lastmove=3;
            x=x+1;
            return true;
        }
        bool left(){
            if (sala->get_colision(x-1,y)) {return false;}
            lastmove=4;
            x=x-1;
            return true;
        }
        bool checkup(){
            if (sala->get_colision(x,y+1)) {return false;}
            return true;
        };
        bool checkdown(){
            if (sala->get_colision(x,y-1)) {return false;}
            
            return true;
        }
        bool checkright(){
            if (sala->get_colision(x+1,y)) {return false;}
            
            return true;
        }
        bool checkleft(){
            if (sala->get_colision(x-1,y)) {return false;}
        
            return true;
        }

    public:
    Rayo(class Sala &sala_){
        //aquí son dos objetos distintos, pero ha de ser una referencia?
        sala = &sala_;
    }
        void set_XY(int X, int Y){
            x = X;
            bouncex = X;
            y = Y;
            bouncey = Y;
        }
        void set_random_vector(int bounce = 0){
            check = true;
            angle = false;
            while(check){
            A = get_random();
            B = get_random();
            //el numero depende de donde estaba el ultimo angulo?
            //no angle, inicial, up down right left
            if (bounce == 0){angle=true;}else
            if (bounce == 1 && A<0){angle=true;}else
            if (bounce == 2 && A>0){angle=true;}else
            if (bounce == 3 && B<0){angle=true;}else
            if (bounce == 4 && B>0){angle=true;};


            //no se si esto de abajo está bien
            if ((float)A*(float)A+(float)B*(float)B<1 && angle==true){check=false;};
            }
            pte = (float)A/(float)B;
            }
        void draw(int rebotes){
            lastmove = 0;
            steps = 1;
            for (int i = 0;i<rebotes;i++){
            flag = true;
            flag2 = true;
            while(flag2){
            set_random_vector(lastmove);
            flag2 = !checkmove();
            }
            while (flag){
            flag = move();
            sala->add_iluminacion(x,y,1000000/pow(steps,2));
            steps++;
            }
            bouncex = x;
            bouncey = y;
            }


        }
        
        bool checkmove(){
            relx = x - bouncex;
            if (relx==0 & B<0){if (A>0){relx= -0.00001;}}
            if (relx==0 & B<0){if (A<0){relx= -0.00001;}}
            if (relx==0 & B>0){if (A<0){relx= 0.00001;}}
            if (relx==0 & B>0){if (A>0){relx= 0.00001;}}
            rely = y - bouncey;
            
            if (B>0){
                if (A>0){
                    if ((float)rely/(float)relx>pte){if(!checkright()){return false;};}
                    if ((float)rely/(float)relx<pte){if(!checkup()){return false;};}
                }
                if (A<0){
                    if ((float)rely/(float)relx>pte){if(!checkdown()){return false;};}
                    if ((float)rely/(float)relx<pte){if(!checkright()){return false;};}
                }
                if (A==0){
                    if(!checkright()){return false;};
                }
            }
            if (B<0){
                if (A>0){
                    if ((float)rely/(float)relx>pte){if(!checkup()){return false;};}
                    if ((float)rely/(float)relx<pte){if(!checkleft()){return false;};}
                }
                if (A<0){
                    if ((float)rely/(float)relx>pte){if(!checkleft()){return false;};}
                    if ((float)rely/(float)relx<pte){if(!checkdown()){return false;};}
                }
                if (A==0){
                    if(!checkleft()){return false;};
                }
            }
            if (B==0) {
                if (A>0){if(!checkup()){return false;};}
                if (A<0){if(!checkdown()){return false;};}
            }
        
        return true;
        }


        //mueve, si lo ha conseguido, da true, si habia colisión, da false
        bool move(){
            relx = x - bouncex;
            if (relx==0 & B<0){if (A>0){relx= -0.00001;}}
            if (relx==0 & B<0){if (A<0){relx= -0.00001;}}
            if (relx==0 & B>0){if (A<0){relx= 0.00001;}}
            if (relx==0 & B>0){if (A>0){relx= 0.00001;}}
            rely = y - bouncey;
            
            if (B>0){
                if (A>0){
                    if ((float)rely/(float)relx>pte){if(!right()){return false;};}
                    if ((float)rely/(float)relx<pte){if(!up()){return false;};}
                }
                if (A<0){
                    if ((float)rely/(float)relx>pte){if(!down()){return false;};}
                    if ((float)rely/(float)relx<pte){if(!right()){return false;};}
                }
                if (A==0){
                    if(!right()){return false;};
                }
            }
            if (B<0){
                if (A>0){
                    if ((float)rely/(float)relx>pte){if(!up()){return false;};}
                    if ((float)rely/(float)relx<pte){if(!left()){return false;};}
                }
                if (A<0){
                    if ((float)rely/(float)relx>pte){if(!left()){return false;};}
                    if ((float)rely/(float)relx<pte){if(!down()){return false;};}
                }
                if (A==0){
                    if(!left()){return false;};
                }
            }
            if (B==0) {
                if (A>0){if(!up()){return false;};}
                if (A<0){if(!down()){return false;};}
            }
        return true;
        }
};
class Cuadrado{
    private:
    int x1;
    int y1;
    int x2;
    int y2;
    class Sala *sala;
    public:
    void build(){
        for (int x = x1;x<=x2;x++){
            for (int y = y1;y<=y2;y++){
                sala->set_colision(x,y);
            }
        }
        
    }
    Cuadrado(class Sala &sala_,int x1_,int y1_, int x2_, int y2_){
        x1 = x1_;
        y1 = y1_;
        x2 = x2_;
        y2 = y2_;
        sala = &sala_;
        build();

    }


};
#define RAYOS 1000
#define REBOTES 10
int main() {
    configconsole();
    //Hay que hacer el algoritmo del rayo, que vaya interaccionando con la sala.
    //Se puede hacer cada rayo por separado usando multithreading y luego juntarlos todos con una suma.
    class Sala sala;
    class Cuadrado cuadrado(sala,40,33,50,37);
    class Cuadrado cuadrado2(sala,60,15,61,35);
    class Rayo rayo(sala);

    for (int i = 0;i<RAYOS;i++){
    rayo.set_XY(1,1);
    rayo.draw(REBOTES);}

    sala.draw();

    return 0;
}
