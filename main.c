/*
 * O QUE FAZER:
 * - Montar uma fila de voos e orgorirar ela pelo 'A' e fuel 0
 * - Pegar hora inicial de simulação e preencher dados iniciais corretamentes
 * - Na simulação: diminuir a unidade de tempo dos aviões e de talvez outra coisa
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define U_TEMPO 5
#define FUEL_D 0


/*
 * STRUCTS
 */
typedef struct {
    char codigo_de_voo[7];
    char modo_de_voo;
    int nivel_de_combustivel;
}Aviao;

typedef struct fila{
  Aviao *info;
  struct fila *prox;
}Fila;

typedef struct {
    Fila *inicio, *fim;
}Cabecalho;


/*
 * ASSINATURA DAS FUNÇÕES
 */
int existe(int *valores, int tam, int valor); //OK
void rand_voos(int *numeros, int quantNumeros); //OK
Fila *cria_aviao(); //OK
Cabecalho inicia_fila(int NAproximacoes, int NDecolagens, int NVoos);
void imprime_fila_de_pedidos();
void simula_voos();


/*
 * FUNÇÃO PRINCIPAL
 */
int main(){

    srand(time(NULL));

    int NAproximacoes = 10 + (rand() % 23); //valor entre 10 e 32
    int NDecolagens = 10 + (rand() % 23);
    int NVoos = NAproximacoes + NDecolagens;

    Cabecalho c;
    c = inicia_fila(NAproximacoes, NDecolagens, NVoos);

    return 0;
}/*FIM-main*/


/*
 * ESCOPO DAS FUNÇÕES
 */
void simula_voos(){

}/*FIM-simula_voos*/

Cabecalho inicia_fila(int NAproximacoes, int NDecolagens, int NVoos){

  Cabecalho c;
  Fila *aux;
  int hours, min, num_aleatorios[NVoos], tipo, cont_A = 0, cont_D = 0;

  //Inicia Cabecalho - 1º item da lista sem a info preenchida
  c.inicio = cria_aviao();
  c.fim = c.inicio;
  c.inicio->prox = NULL;

  //Tempo inicial da simulação
  hours = rand()%24;
  do{ //Pega min divisivel por 5
    min = rand()%56;
  }while(min%5 != 0);

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

  //Gera voos aleatorios
  rand_voos(num_aleatorios, NVoos);

  //Monta fila de avioes
  for(int i = 0; i < NVoos; i++){
    if(i != 0){ //Caso exista só 1 avião
      c.fim->prox = cria_aviao();
      c.fim = c.fim->prox;
      c.fim->prox = NULL;
    }

    //Preenche o código aleatório do avião
    strncpy(c.fim->info->codigo_de_voo, lista_voos[num_aleatorios[i]], 7);
    //strcpy(c.fim->info->codigo_de_voo, lista_voos[num_aleatorios[i]]);

    tipo = rand() % 2;
    if(tipo == FUEL_D && cont_D < NDecolagens){
        c.fim->info->modo_de_voo = 'D';
        cont_D++;

    }else{
      if(cont_A < NAproximacoes){
        c.fim->info->modo_de_voo = 'A';
        c.fim->info->nivel_de_combustivel = rand()%13;
        cont_A++;

      }else{
        c.fim->info->modo_de_voo = 'D';
        cont_D++;
      }
    }
  }/*FIM-for*/

  //Dados iniciais
  aux = c.inicio;
  printf("---------------------------------------------------------------------------------\n");
  printf("Aeroporto Internacional de Brasília\n");
  printf("Hora Inicial: %dh %dmin\n", hours, min);
  printf("Fila de Pedidos:");

  while (aux !=NULL) {
    printf("\n\t[Código do vôo: %s - ", aux->info->codigo_de_voo);
    if(aux->info->modo_de_voo == 'A'){
      printf(" P - %d]", aux->info->nivel_de_combustivel);
    }else{
      printf(" D - --]");
    }
    aux = aux->prox;
  }

  printf("\nNVoos: %d\n", NVoos);
  printf("Naproximações: %d\n", NAproximacoes);
  printf("NDecolagens: %d\n", NDecolagens);
  printf("---------------------------------------------------------------------------------\n");
  printf("Listagem de eventos:\n");


  printf("\n");

  /*for(int i=1; i<=NVoos; i++) {
      if(i==1)
          printf("APROXIMACOES:\n");
      if(i==NAproximacoes+1)
          printf("DECOLAGENS:\n");
      printf("%s\n", lista_voos[num_aleatorios[i]]);
  } */

  return c;
}/*FIM-inicia_fila*/

Fila *cria_aviao(){
  Fila *novo = (Fila*) malloc(sizeof(Fila));
  Aviao *n_av = (Aviao*) malloc(sizeof(Aviao));
  novo->info = n_av;
  return novo;
}/*FIM-cria_aviao*/

int existe(int *valores, int tam, int valor) {
    for(int i=1; i<=tam; i++){
        if(valores[i] == valor)
            return 1;
    }
    return 0;
}/*FIM-existe*/

void rand_voos(int *numeros, int quantNumeros){
    int v;
    for(int i=1; i<=quantNumeros; i++){
        v = (rand() % 64);
        while(existe(numeros,i,v)){
            v = (rand() % 64);
        }
        numeros[i] = v;
    }
}/*FIM-rand_voos*/
