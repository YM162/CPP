#define ANCHO 60
#define ALTO 60
#define INICIOX 0
#define INICIOY 0
#define FINX 59
#define FINY 59

using namespace std;
#include <math.h>
#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <windows.h>

class spot{
    public:
    
    int x;
    int y;
    double g;
    double h;
    double f;
    spot* parent = NULL;
};

spot SPOTS[ANCHO][ALTO];
vector <spot*> ABIERTOS;
vector <spot*> CERRADOS;
vector <spot*> BLOQUEADOS;


void configconsole(){
    CONSOLE_FONT_INFOEX info = {0};
    info.cbSize       = sizeof(info);
    info.dwFontSize.Y = 12;
    info.dwFontSize.X = 12;
    info.FontWeight   = FW_NORMAL;
    wcscpy(info.FaceName, L"Lucida Console");
    SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), NULL, &info);
}



void initbloqueados(){
    
    char x;
    ifstream inFile;
    
    inFile.open("map.txt");
    if (!inFile) {
        cout << "Unable to open file";
        exit(1); // terminate with error
    }
    int i = 0;
    int j = 0;
    while (inFile >> x) {
        if (x == 'X') {BLOQUEADOS.push_back(&SPOTS[j][i]);}
        j++;
        if (j==ANCHO){
            j = 0;
            i++;
        }
    }

    inFile.close();

    
}
double h_calc(int x,int y,int xend,int yend){
        int xdif = xend-x;
        int ydif = yend-y;
        double distancia = sqrt(xdif*xdif+ydif*ydif);
        return distancia;
        }






spot* getbestspot(){
    spot* best = ABIERTOS[0];
    for(auto i : ABIERTOS){
        if (i->f<best->f){
            best = i;
            }
    };
    return best;
}

vector <spot*> getparents(spot* children){
    vector <spot*> parents;
    if (children->parent == NULL){
        //si no tiene parent
        parents.push_back(children);
        return parents;
    }else{
        //si si tiene parent
        parents = getparents(children->parent);
        parents.push_back(children);
        return parents;
    }


    return parents;
}


int main(){

    //añadimos por donde no queremos que vaya
    initbloqueados();
    configconsole();
    for (auto i:BLOQUEADOS){
        CERRADOS.push_back(i);
    }
    for(int y_=0;y_<ALTO;y_++){
        for(int x_=0;x_<ANCHO;x_++){
            SPOTS[x_][y_].x=x_;
            SPOTS[x_][y_].y=y_;
        }
    }
    ABIERTOS.push_back(&SPOTS[INICIOX][INICIOY]);
    SPOTS[INICIOX][INICIOY].g=0;
    SPOTS[INICIOX][INICIOY].h=h_calc(INICIOX,INICIOY,FINX,FINY);
    SPOTS[INICIOX][INICIOY].f=h_calc(INICIOX,INICIOY,FINX,FINY);
    //bucle principal
    spot* current;
    while(ABIERTOS.size()!=0){
        current = getbestspot();
        
        if (current->x==FINX && current->y==FINY){break;}

        //cout << "X: " << current->x << " Y: "<< current->y;
        //hay que borrar el current de abiertos y añadirlo a closed.
        ABIERTOS.erase(remove(ABIERTOS.begin(),ABIERTOS.end(),current),ABIERTOS.end());
        CERRADOS.push_back(current);
        //evaluar vecinos del current e ir añadiendoles a abiertos
        double newg = current->g +1;
        //para x+1,y
        //hay que ver si está en walkable
        if (current->x!=ANCHO-1){
        double newh = h_calc(current->x +1, current->y ,FINX,FINY);
        double newf = newg + newh;
        if (find(ABIERTOS.begin(),ABIERTOS.end(),&SPOTS[current->x+1][current->y])!=ABIERTOS.end()) {
        //si está ya en abiertos
        if (SPOTS[current->x+1][current->y].f > newf){
            //si el f nuevo es mejor que el f anterior.
            SPOTS[current->x+1][current->y].f=newf;
            SPOTS[current->x+1][current->y].g=newg;
            SPOTS[current->x+1][current->y].h=newh;
            SPOTS[current->x+1][current->y].parent=current;

        }
        //si es peor no hacemos nada
        }else{
        //si no está en abiertos, lo añadimos y updateamos los valores
        if (find(CERRADOS.begin(),CERRADOS.end(),&SPOTS[current->x+1][current->y])==CERRADOS.end()) {
        //esto solo occurre si no está en cerrados
        ABIERTOS.push_back(&SPOTS[current->x+1][current->y]);
        SPOTS[current->x+1][current->y].f=newf;
        SPOTS[current->x+1][current->y].g=newg;
        SPOTS[current->x+1][current->y].h=newh;
        SPOTS[current->x+1][current->y].parent=current;
        }
        }
        }

        //para x-1,y
        if (current->x!=0){
        double newh = h_calc(current->x -1, current->y ,FINX,FINY);
        double newf = newg + newh;
        if (find(ABIERTOS.begin(),ABIERTOS.end(),&SPOTS[current->x-1][current->y])!=ABIERTOS.end()) {
        //si está ya en abiertos
        if (SPOTS[current->x-1][current->y].f > newf){
            //si el f nuevo es mejor que el f anterior.
            SPOTS[current->x-1][current->y].f=newf;
            SPOTS[current->x-1][current->y].g=newg;
            SPOTS[current->x-1][current->y].h=newh;
            SPOTS[current->x-1][current->y].parent=current;
        }
        //si es peor no hacemos nada
        }else{
        //si no está en abiertos, lo añadimos y updateamos los valores
        if (find(CERRADOS.begin(),CERRADOS.end(),&SPOTS[current->x-1][current->y])==CERRADOS.end()) {
        //esto solo occurre si no está en cerrados
        
        ABIERTOS.push_back(&SPOTS[current->x-1][current->y]);
        SPOTS[current->x-1][current->y].f=newf;
        SPOTS[current->x-1][current->y].g=newg;
        SPOTS[current->x-1][current->y].h=newh;
        SPOTS[current->x-1][current->y].parent=current;
        }
        }
        }

        //para x,y+1
        if (current->x!=ALTO-1){
        double newh = h_calc(current->x , current->y+1 ,FINX,FINY);
        double newf = newg + newh;
        if (find(ABIERTOS.begin(),ABIERTOS.end(),&SPOTS[current->x][current->y+1])!=ABIERTOS.end()) {
        //si está ya en abiertos
        if (SPOTS[current->x][current->y+1].f > newf){
            //si el f nuevo es mejor que el f anterior.
            SPOTS[current->x][current->y+1].f=newf;
            SPOTS[current->x][current->y+1].g=newg;
            SPOTS[current->x][current->y+1].h=newh;
            SPOTS[current->x][current->y+1].parent=current;

        }
        //si es peor no hacemos nada
        }else{
        //si no está en abiertos, lo añadimos y updateamos los valores
        if (find(CERRADOS.begin(),CERRADOS.end(),&SPOTS[current->x][current->y+1])==CERRADOS.end()) {
        //esto solo occurre si no está en cerrados
        
        ABIERTOS.push_back(&SPOTS[current->x][current->y+1]);
        SPOTS[current->x][current->y+1].f=newf;
        SPOTS[current->x][current->y+1].g=newg;
        SPOTS[current->x][current->y+1].h=newh;
        SPOTS[current->x][current->y+1].parent=current;
        }
        }
        }

        //para x,y-1
        if (current->y!=0){
        double newh = h_calc(current->x, current->y-1 ,FINX,FINY);
        double newf = newg + newh;
        if (find(ABIERTOS.begin(),ABIERTOS.end(),&SPOTS[current->x][current->y-1])!=ABIERTOS.end()) {
        //si está ya en abiertos
        if (SPOTS[current->x][current->y-1].f > newf){
            //si el f nuevo es mejor que el f anterior.
            SPOTS[current->x][current->y-1].f=newf;
            SPOTS[current->x][current->y-1].g=newg;
            SPOTS[current->x][current->y-1].h=newh;
            SPOTS[current->x][current->y-1].parent=current;

        }
        //si es peor no hacemos nada
        }else{
        //si no está en abiertos, lo añadimos y updateamos los valores
        if (find(CERRADOS.begin(),CERRADOS.end(),&SPOTS[current->x][current->y-1])==CERRADOS.end()) {
        //esto solo occurre si no está en cerrados
        
        ABIERTOS.push_back(&SPOTS[current->x][current->y-1]);
        SPOTS[current->x][current->y-1].f=newf;
        SPOTS[current->x][current->y-1].g=newg;
        SPOTS[current->x][current->y-1].h=newh;
        SPOTS[current->x][current->y-1].parent=current;
        }
        }
        }

        //las diagonales



        //para x+1,y+1
        if (current->x!=ANCHO-1 && current->y!=ALTO-1){
        double newh = h_calc(current->x +1, current->y+1 ,FINX,FINY);
        double newf = newg + newh;
        if (find(ABIERTOS.begin(),ABIERTOS.end(),&SPOTS[current->x+1][current->y+1])!=ABIERTOS.end()) {
        //si está ya en abiertos
        if (SPOTS[current->x+1][current->y+1].f > newf){
            //si el f nuevo es mejor que el f anterior.
            SPOTS[current->x+1][current->y+1].f=newf+0.414213;
            SPOTS[current->x+1][current->y+1].g=newg+0.414213;
            SPOTS[current->x+1][current->y+1].h=newh;
            SPOTS[current->x+1][current->y+1].parent=current;

        }
        //si es peor no hacemos nada
        }else{
        //si no está en abiertos, lo añadimos y updateamos los valores
        if (find(CERRADOS.begin(),CERRADOS.end(),&SPOTS[current->x+1][current->y+1])==CERRADOS.end()) {
        //esto solo occurre si no está en cerrados
        ABIERTOS.push_back(&SPOTS[current->x+1][current->y+1]);
        SPOTS[current->x+1][current->y+1].f=newf+0.414213;
        SPOTS[current->x+1][current->y+1].g=newg+0.414213;
        SPOTS[current->x+1][current->y+1].h=newh;
        SPOTS[current->x+1][current->y+1].parent=current;
        }
        }
        }

        //para x-1,y+1
        if (current->x!=0 && current->y!=ALTO-1){
        double newh = h_calc(current->x -1, current->y+1 ,FINX,FINY);
        double newf = newg + newh;
        if (find(ABIERTOS.begin(),ABIERTOS.end(),&SPOTS[current->x-1][current->y+1])!=ABIERTOS.end()) {
        //si está ya en abiertos
        if (SPOTS[current->x-1][current->y+1].f > newf){
            //si el f nuevo es mejor que el f anterior.
            SPOTS[current->x-1][current->y+1].f=newf+0.414213;
            SPOTS[current->x-1][current->y+1].g=newg+0.414213;
            SPOTS[current->x-1][current->y+1].h=newh;
            SPOTS[current->x-1][current->y+1].parent=current;

        }
        //si es peor no hacemos nada
        }else{
        //si no está en abiertos, lo añadimos y updateamos los valores
        if (find(CERRADOS.begin(),CERRADOS.end(),&SPOTS[current->x-1][current->y+1])==CERRADOS.end()) {
        //esto solo occurre si no está en cerrados
        ABIERTOS.push_back(&SPOTS[current->x-1][current->y+1]);
        SPOTS[current->x-1][current->y+1].f=newf+0.414213;
        SPOTS[current->x-1][current->y+1].g=newg+0.414213;
        SPOTS[current->x-1][current->y+1].h=newh;
        SPOTS[current->x-1][current->y+1].parent=current;
        }
        }
        }

        //para x+1,y-1
        if (current->x!=ANCHO-1 && current->y!=0){
        double newh = h_calc(current->x +1, current->y-1 ,FINX,FINY);
        double newf = newg + newh;
        if (find(ABIERTOS.begin(),ABIERTOS.end(),&SPOTS[current->x+1][current->y-1])!=ABIERTOS.end()) {
        //si está ya en abiertos
        if (SPOTS[current->x+1][current->y-1].f > newf){
            //si el f nuevo es mejor que el f anterior.
            SPOTS[current->x+1][current->y-1].f=newf+0.414213;
            SPOTS[current->x+1][current->y-1].g=newg+0.414213;
            SPOTS[current->x+1][current->y-1].h=newh;
            SPOTS[current->x+1][current->y-1].parent=current;

        }
        //si es peor no hacemos nada
        }else{
        //si no está en abiertos, lo añadimos y updateamos los valores
        if (find(CERRADOS.begin(),CERRADOS.end(),&SPOTS[current->x+1][current->y-1])==CERRADOS.end()) {
        //esto solo occurre si no está en cerrados
        ABIERTOS.push_back(&SPOTS[current->x+1][current->y-1]);
        SPOTS[current->x+1][current->y-1].f=newf+0.414213;
        SPOTS[current->x+1][current->y-1].g=newg+0.414213;
        SPOTS[current->x+1][current->y-1].h=newh;
        SPOTS[current->x+1][current->y-1].parent=current;
        }
        }
        }

        //para x-1,y-1
        if (current->x!=0 && current->y!=0){
        double newh = h_calc(current->x -1, current->y-1 ,FINX,FINY);
        double newf = newg + newh;
        if (find(ABIERTOS.begin(),ABIERTOS.end(),&SPOTS[current->x-1][current->y-1])!=ABIERTOS.end()) {
        //si está ya en abiertos
        if (SPOTS[current->x-1][current->y-1].f > newf){
            //si el f nuevo es mejor que el f anterior.
            SPOTS[current->x-1][current->y-1].f=newf+0.414213;
            SPOTS[current->x-1][current->y-1].g=newg+0.414213;
            SPOTS[current->x-1][current->y-1].h=newh;
            SPOTS[current->x-1][current->y-1].parent=current;

        }
        //si es peor no hacemos nada
        }else{
        //si no está en abiertos, lo añadimos y updateamos los valores
        if (find(CERRADOS.begin(),CERRADOS.end(),&SPOTS[current->x-1][current->y-1])==CERRADOS.end()) {
        //esto solo occurre si no está en cerrados
        ABIERTOS.push_back(&SPOTS[current->x-1][current->y-1]);
        SPOTS[current->x-1][current->y-1].f=newf+0.414213;
        SPOTS[current->x-1][current->y-1].g=newg+0.414213;
        SPOTS[current->x-1][current->y-1].h=newh;
        SPOTS[current->x-1][current->y-1].parent=current;
        }
        }
        }

    }
    vector <spot*> path=getparents(current);
    char dibujo[ANCHO][ALTO];
    for(int y_=0;y_<ALTO;y_++){
        for(int x_=0;x_<ANCHO;x_++){
            if (find(ABIERTOS.begin(),ABIERTOS.end(),&SPOTS[x_][y_])!=ABIERTOS.end()) {dibujo[x_][y_] = (char)61u;}
            else if (find(CERRADOS.begin(),CERRADOS.end(),&SPOTS[x_][y_])!=CERRADOS.end()) {dibujo[x_][y_] = (char)176u;}
            else{dibujo[x_][y_] = (char)250u;}
        }
        }
    
    for (auto i:path){
        dibujo[i->x][i->y]=(char)178u;
        }
    for (auto i:BLOQUEADOS){
        dibujo[i->x][i->y]=(char)219u;
    }
    for(int y_=0;y_<ALTO;y_++){
        for(int x_=0;x_<ANCHO;x_++){
            cout << dibujo[x_][y_];
        }
        cout << "\n";
        }
    std::cout << "Presione enter para continuar...";
    std::getchar();
    return 0;
}
