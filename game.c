#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct atributosBase {
    int des;
    int stg;
    int con;
    int pow;
    int lvl;
    int lp;
    int ep;
};

struct player {
    struct atributosBase agenteAtri;
    int exp;
    int sp;
    char nome[20];
    int coin;

}agente;

void telaAgente() {
    system("cls");
    printf("Agente: %s \n", agente.nome);
    printf("Pontos de vida: %d / %d \n", agente.agenteAtri.lp, agente.agenteAtri.con * 5);
    printf("Pontos de sanidade: %d / %d \n", agente.sp, agente.agenteAtri.pow * 7);
    printf("Pontos de esforco: %d / %d \n", agente.agenteAtri.ep, agente.agenteAtri.des * 5);
}

int main (){
    system("cls");
    int *opicao, *deci;
    printf("Qual o seu nome agente? \n");
    gets(agente.nome);
    
    deci = (int *)(malloc(sizeof(int)));
    while (deci!=1){
        printf("\nPara o inicio de nossa jornada, defina seu estilo de combate: \n");
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
                agente.agenteAtri.pow = 1;
                agente.agenteAtri.lp = agente.agenteAtri.con * 5;
                agente.sp = agente.agenteAtri.pow * 7;
                agente.agenteAtri.ep = agente.agenteAtri.des * 5;
                deci = 1;
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
                agente.agenteAtri.pow = 5;
                agente.agenteAtri.lp = agente.agenteAtri.con * 5;
                agente.sp = agente.agenteAtri.pow * 7;
                agente.agenteAtri.ep = agente.agenteAtri.des * 5;
                deci = 1;
            }
        } else {
            printf("Essa classe não existe ainda!\n");
        }
    }

    telaAgente();

    free(deci);
    free(opicao);

    return 0;
}