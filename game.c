#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <locale.h>

struct baseAttribures {
    int des;
    int stg;
    int con;
    int pod;
    int lvl;
    int lp;
    int ep;
};

struct player {
    struct baseAttribures agenteAtri;
    int exp;
    int sp;
    char name[20];
    int coin;

}agente;

void agenteStatus();
int movimentoAtaque(int atriAtacante, int dadoAtaque);
void localdaseta(int realPosition, int posicaoDaTecla);
void eventoBatalha();

int main (){
    setlocale(LC_ALL, "Portuguese");
    system("cls");
    int *opicao, *deci;
    printf("Qual o seu nome agente? \n");
    gets(agente.name);
    
    deci = (int *)(malloc(sizeof(int)));
    while (*deci!=1){
        printf("\nPara o inicio de nossa jornada, defina seu estilo de combaté: \n");
        printf("1 - Combatente \n");
        printf("2 - Ocultista \n");
        
        opicao = (int *)(malloc(sizeof(int)));
        scanf("%d", opicao);

        if (*opicao == 1){
            system("cls");
            printf("Combatentes sao uma classe focada em forca fisica e combate corpo a corpo \nno entanto nao possuem tanto poder vindo 'do outro lado', deseja selecionar essa classe? \n \n ");
            printf("Item exclusivo: Katana \n");
            printf("1 - confirmar classe \n2 - voltar\n \n");
            scanf("%d", opicao);
            if(*opicao == 1){
                agente.agenteAtri.des = 3;
                agente.agenteAtri.stg = 4;
                agente.agenteAtri.con = 4;
                agente.agenteAtri.pod = 1;
                agente.agenteAtri.lp = agente.agenteAtri.con * 5;
                agente.sp = agente.agenteAtri.pod * 7;
                agente.agenteAtri.ep = agente.agenteAtri.des * 5;
                agente.coin=2000;
                *deci = 1;
            }
            
        } else if (*opicao == 2){
            system("cls");
            printf("Ocultistas sao expecialistas no uso de rituais para os mais diversos fins \nno entanto sao muito fracos em combates corpor a corpo, deseja selecionar essa classe? \n \n");
            printf("Item exclusivo: Grimorio \n");
            printf("1 - confirmar classe \n2 - voltar\n \n");
            scanf("%d", opicao);
            if(*opicao == 1){
                agente.agenteAtri.des = 2;
                agente.agenteAtri.stg = 1;
                agente.agenteAtri.con = 3;
                agente.agenteAtri.pod = 5;
                agente.agenteAtri.lp = agente.agenteAtri.con * 5;
                agente.sp = agente.agenteAtri.pod * 7;
                agente.agenteAtri.ep = agente.agenteAtri.des * 5;
                agente.coin=2000;
                *deci = 1;
            }
        } else {
            printf("Essa classe não existe ainda!\n");
        }
    }

    eventoBatalha();

    free(deci);
    free(opicao);

    return 0;
}

void agenteStatus() {
    //system("cls");
    printf("Agente: %s \n", agente.name);
    printf("Pontos de vida: %d / %d \n", agente.agenteAtri.lp, agente.agenteAtri.con * 5);
    printf("Pontos de sanidade: %d / %d \n", agente.sp, agente.agenteAtri.pod * 7);
    printf("Pontos de esforco: %d / %d \n", agente.agenteAtri.ep, agente.agenteAtri.des * 5);
}

int movimentoAtaque(int atriAtacante, int dadoAtaque) {
    
    srand(time(NULL));
    int numDados = atriAtacante/2 + atriAtacante%2, i = 0;
    int dado, maiorDado = 0, dt = 20 - (numDados * 5), dano = 0; 

    for (i=0; i<numDados; i++){
        dado = (rand()%20)+1;
        if (i==0){
            maiorDado = dado;
        }
        if (dado > maiorDado){
            maiorDado = dado;
        }
    }

    if (maiorDado >= dt) {
        if (maiorDado == 20){
            dano = 2 * dadoAtaque;
            printf("Você acertou em cheio! \n");
            printf("dano causado: %d \n", dano);
            return dano;
        }else{
            for (i=0; i<numDados; i++){
                dano += (rand()%dadoAtaque)+1;
            }
            printf("Você acertou! \n");
            printf("dano causado: %d \n", dano);
            return dano;
        }
        
    }else{
        printf("Você errou! \n");
        return 0;
    }

}

void localdaseta(int realPosition, int posicaoDaTecla) {
    if (realPosition == posicaoDaTecla) {
        printf("\t\t\t=> ");
    }
    else {
        printf("\t\t\t   ");
    }
}

void eventoBatalha(){
    
}