#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define U_TEMPO 5
#define FUEL_D 0
#define TRUE 1
#define FALSE 0


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
int existe(int *valores, int tam, int valor);
void rand_voos(int *numeros, int quantNumeros);
Fila *cria_aviao();
Cabecalho inicia_fila(int NAproximacoes, int NDecolagens, int NVoos, int *h, int *m);
void priorizar_combustivel_0(Cabecalho *c);
int alerta_de_emergencia(Cabecalho *c);
void imprime_evento(Cabecalho *c, int h, int m, int pista);
void simula_voos(Cabecalho *c, int *hours, int *min);


/*
 * FUNÇÃO MAIN
 */
int main(){

    srand(time(NULL));

    int NAproximacoes = 10 + (rand() % 23); //valor entre 10 e 32
    int NDecolagens = 10 + (rand() % 23);
    int NVoos = NAproximacoes + NDecolagens;
    int hours, min;

    Cabecalho c;
    c = inicia_fila(NAproximacoes, NDecolagens, NVoos, &hours, &min);

    simula_voos(&c, &hours, &min);

    return 0;
}/*FIM-main*/


/*
 * ESCOPO DAS FUNÇÕES
 */
void priorizar_combustivel_0(Cabecalho *c){ //Passa para o inicio da fila todos aviões com modo_de_voo 'A' e com 0 de combustivel
  if(c->inicio != NULL){
    Fila *atual = c->inicio->prox;
    Fila *ant = c->inicio;

    while(atual != NULL && ant != NULL){
      if (atual->info->modo_de_voo == 'A') {
        if (atual->info->nivel_de_combustivel == 0) {
          ant->prox = atual->prox;
          atual->prox = c->inicio;
          c->inicio = atual;
          atual = ant->prox;

        }else{
          ant = atual;
          atual = atual->prox;
        }

      }else{
        ant = atual;
        atual = atual->prox;
      }
    }/*FIM-while*/
  }
}/*FIM-priorizar_combustivel_0*/

int alerta_de_emergencia(Cabecalho *c){
  int emergencia = FALSE;

  if(c->inicio != NULL && c->inicio->prox != NULL && c->inicio->prox->prox != NULL){
    Fila *voo = c->inicio->prox->prox;
    if (voo->info->modo_de_voo == 'A' && voo->info->nivel_de_combustivel == 0){
      emergencia = TRUE;
      printf("\nALERTA GERAL DE DESVIO DE AERONAVE\n\n");
    }
  }
  return emergencia;
}/*FIM-alerta_de_emergencia*/

void imprime_evento(Cabecalho *c, int h, int m, int pista){ //Imprime e deleta o início da fila
    Fila *voo = c->inicio;
    c->inicio = c->inicio->prox;

    printf("Código de Voo: %s\n", voo->info->codigo_de_voo);

    if(voo->info->modo_de_voo == 'D')
      printf("Status: aeronave decolou");
    else
      printf("Status: aeronave pousou");

    printf("\nHorário do início do procedimento: %.2d:%.2d\n", h, m);
    printf("Número da pista: %d\n", pista);
    printf("\n");

    free(voo->info);
    free(voo);
}/*FIM-imprime_evento*/

void simula_voos(Cabecalho *c, int *hours, int *min){
  int pista_1 = TRUE, pista_2 = TRUE, pista_3 = TRUE, emergencia;
  int tempo_1, tempo_2, tempo_3;
  int tempo_tanque = 0;

  priorizar_combustivel_0(c);
  emergencia = alerta_de_emergencia(c);

  while (c->inicio != NULL){

    if(pista_1 && c->inicio != NULL){
      if(c->inicio->info->modo_de_voo == 'D')
        tempo_1 = 2*U_TEMPO;
      else
        tempo_1 = 4*U_TEMPO;
      imprime_evento(c, *hours, *min, 1);
      pista_1 = FALSE;
    }

    if(pista_2 && c->inicio != NULL){
      if(c->inicio->info->modo_de_voo == 'D')
        tempo_2 = 2*U_TEMPO;
      else
        tempo_2 = 4*U_TEMPO;
      imprime_evento(c, *hours, *min, 2 );
      pista_2 = FALSE;
    }

    if(pista_3 && c->inicio != NULL){
      if(emergencia){
        tempo_3 = 4*U_TEMPO;
        imprime_evento(c, *hours, *min, 3);
        pista_3 = FALSE;
        emergencia = FALSE;
      }else if(c->inicio->info->modo_de_voo == 'D'){
        tempo_3 = 2*U_TEMPO;
        imprime_evento(c, *hours, *min, 3);
        pista_3 = FALSE;
      }
    }

    //Verifica se as pistas estão ocupadas
    if(!pista_1){
      tempo_1-= U_TEMPO;
      if(!tempo_1) {
        pista_1= TRUE;
      }
    }
    if(!pista_2){
      tempo_2 -= U_TEMPO;
      if (!tempo_2) {
        pista_2 = TRUE;
      }
    }
    if(!pista_3){
      tempo_3 -= U_TEMPO;
      if(!tempo_3){
        pista_3 = TRUE;
      }
    }

    //Diminui uma unidade do nível de combustível a cada 10*U_TEMPO
    tempo_tanque+= U_TEMPO;
    if((tempo_tanque % (10*U_TEMPO)) == 0 && tempo_tanque != 0){
      Fila *atual, *ant;
      atual = ant = c->inicio;

      while(atual != NULL){
        if(atual->info->modo_de_voo == 'A'){
          atual->info->nivel_de_combustivel--;
          if(atual->info->nivel_de_combustivel == -1){
            if(atual != ant)
              ant->prox = atual->prox;
              free(atual->info);
              free(atual);
              printf("\nALERTA CRÍTICO, AERONAVE IRÁ CAIR\n\n");
          }
        }

        ant = atual;
        atual = atual->prox;
      }
      priorizar_combustivel_0(c);
      tempo_tanque = 0;
    }

    //Incrementa o tempo de 5 em 5 minutos
    *min+= U_TEMPO;
    if(*min == 60){
      *min= 0;
      if(*hours != 23)
        (*hours)++;
      else
        *hours= 0;
    }


    emergencia = alerta_de_emergencia(c);
  }
}/*FIM-simula_voos*/

Cabecalho inicia_fila(int NAproximacoes, int NDecolagens, int NVoos, int *h, int *m){

  Cabecalho c;
  Fila *aux;
  int hours, min, num_aleatorios[NVoos], tipo, cont_A = 0, cont_D = 0;

  //Inicia Cabecalho - 1º item da lista sem a info preenchida
  c.inicio = cria_aviao();
  c.fim = c.inicio;
  c.inicio->prox = NULL;

  //Tempo inicial da simulação
  hours= (rand()%17) + 7;
  do{ //Pega min divisivel por 5
    min= rand()%56;
  }while(min%5 != 0);
  *h= hours;
  *m= min;

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

  //Gera índices aleatorios para preencher os códigos com o lista_voos[]
  rand_voos(num_aleatorios, NVoos);

  //Monta fila de aviões
  for(int i = 0; i < NVoos; i++){
    if(i != 0){ //Caso exista só 1 avião
      c.fim->prox = cria_aviao();
      c.fim = c.fim->prox;
      c.fim->prox = NULL;
    }

    //Preenche o código aleatório do avião
    strncpy(c.fim->info->codigo_de_voo, lista_voos[num_aleatorios[i]], 7);

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
  printf("-----------------------------------------------------------------------------\n");
  printf("Aeroporto Internacional de Brasília\n");
  printf("Hora Inicial: %.2dh %.2dmin\n", hours, min);
  printf("Fila de Pedidos:");

  while (aux !=NULL) {
    printf("\n\t[%s - ", aux->info->codigo_de_voo);
    if(aux->info->modo_de_voo == 'A'){
      printf(" A - %.2d]", aux->info->nivel_de_combustivel);
    }else{
      printf(" D -   ]");
    }
    aux = aux->prox;
  }

  printf("\nNVoos: %d\n", NVoos);
  printf("Naproximações: %d\n", NAproximacoes);
  printf("NDecolagens: %d\n", NDecolagens);
  printf("-----------------------------------------------------------------------------\n");
  printf("Listagem de eventos:\n");
  printf("\n");

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
