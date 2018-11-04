#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
    char codigo_de_voo[7];
    char modo_de_voo;
    int nivel_de_combustivel; 
} Aviao;

int existe(int *valores, int tam, int valor);
void rand_voos(int *numeros, int quantNumeros);

int main() {

    srand(time(NULL));
    
    int NAproximacoes = 10 + (rand() % 23); //valor entre 10 e 32
    int NDecolagens = 10 + (rand() % 23); 
    int NVoos = NAproximacoes + NDecolagens;

    // int CombA = (rand() % 13); // valor entre 0 e 12

    printf("\"Aeroporto Internacional de Brasilia\"\n");
    printf("Hora Inicial:\n");
    printf("Fila de Pedidos:\n");
    printf("NVoos = %d\n", NVoos);
    printf("NAproximacoes = %d\n", NAproximacoes);
    printf("NDecolagens = %d\n", NDecolagens);
    printf("--------------------------------------\n");
    printf("Listagem de eventos:\n");
    // printf("CombA = %d\n", CombA);

    char lista_voos[64][7] = {
        "VG3001", "JJ4404", "LN7001", "TG1501", "GL7602", "TT1010", "AZ1009", "AZ1008", 
        "AZ1010", "TG1506", "VG3002", "JJ4402", "GL7603", "RL7880", "AL0012", "TT4544", 
        "TG1505", "VG3003", "JJ4403", "JJ4401", "LN7002", "AZ1002", "AZ1007", "GL7604", 
        "AZ1006", "TG1503", "AZ1003", "JJ4406", "AZ1001", "LN7003", "AZ1004", "TG1504", 
        "AZ1005", "TG1502", "GL7601", "TT4500", "RL7801", "JJ4410", "GL7607", "AL0029", 
        "VV3390", "VV3392", "GF4681", "GF4690", "AZ1020", "JJ4435", "VG3010", "LF0920", 
        "AZ1065", "LF0978", "RL7867", "TT4502", "GL7645", "LF0932", "JJ4434", "TG1510",
        "TT1020", "AZ1098", "BA2312", "VG3030", "BA2304", "KL5609", "KL5610", "KL5611"
    };
 
    printf("\n");

    int voos[NVoos];
    rand_voos(voos, NVoos);

    for(int i=1; i<=NVoos; i++) {
        if(i==1)
            printf("APROXIMACOES:\n");
        if(i==NAproximacoes+1)
            printf("DECOLAGENS:\n");
        printf("%s\n", lista_voos[voos[i]]);
    }
    return 0;
}

int existe(int *valores, int tam, int valor) {
    for(int i=1; i<=tam; i++){
        if(valores[i]==valor)
            return 1;
    }
    return 0;
}

void rand_voos(int *numeros, int quantNumeros){
    int v;
    for(int i=1; i<=quantNumeros; i++){
        v = (rand() % 64);
        while(existe(numeros,i,v)){
            v = (rand() % 64);
        }
        numeros[i] = v;
    }	
}
