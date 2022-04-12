#include <iostream>
#include <vector>
#include <algorithm>

std::vector <int> whatsum(std::vector <int> lista,int target){
    std::sort(lista.begin(),lista.end());
    std::vector <int> whatsum;
    for (auto i:lista){
        if (i<=target){
            for (auto j:lista){
                if (j>=i && j<=target){
                    if (j+i==target){whatsum.push_back(i);whatsum.push_back(j);}
                }
            }
        }
    }



    
    return whatsum;

}

int main(){
    std::vector <int> lista = {3,4,5,2,1,0};
    int target = 9;

    std::vector <int> sum = whatsum(lista,target);
    for (auto i:sum){
        std::cout << i <<" ";
    }
    std::cout << "Presione enter para continuar...";
    std::getchar();
    return 0;
}
