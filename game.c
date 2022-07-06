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

struct monster {
    struct baseAttribures monsterAtri;
    char name[20];
    int coinReward;
    int xpReward;
}monster;

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
                agente.agenteAtri.lvl = 1;
                agente.exp = 0;
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
                agente.agenteAtri.lvl = 1;
                agente.exp = 0;
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
            system("pause");
            return dano;
        }else{
            for (i=0; i<numDados; i++){
                dano += (rand()%dadoAtaque)+1;
            }
            printf("Você acertou! \n");
            printf("dano causado: %d \n", dano);
            system("pause");
            return dano;
        }
        
    }else{
        printf("Você errou! \n");
        system("pause");
        return 0;
    }

}

void localdaseta(int realPosition, int posicaoDaTecla) {
    if (realPosition == posicaoDaTecla) {
        printf("\t=> ");
    }
    else {
        printf("\t   ");
    }
}

void eventoBatalha(){
    srand(time(NULL));
    int randMonster = rand () % 2;
    int tryEscape;
    system("cls");
    switch(randMonster) {
        case 0:
        strcpy(monster.name, "Existido de energia");
        monster.monsterAtri.lvl = (rand() % 5) + 1;
        monster.monsterAtri.des = 3;
        monster.monsterAtri.stg = 2;
        monster.monsterAtri.con = 3 + monster.monsterAtri.lvl;
        monster.monsterAtri.pod = 5 + monster.monsterAtri.lvl;
        monster.monsterAtri.lp = monster.monsterAtri.con * 5;
        monster.xpReward = rand() %  100 + (100 * monster.monsterAtri.lvl);
        monster.monsterAtri.ep = monster.monsterAtri.des * 5;
        monster.coinReward = rand() % (200 - 100) + 200;

        printf("Voce se depara com um ser que incandesce uma luz azulada que parece flutuar em pleno ar, em sua face, uma expressão de puro desespero \n\n");
        system("pause");
        break;
        case 1:
        strcpy(monster.name, "Blood Zombie");
        monster.monsterAtri.lvl = (rand() % 5) + 1;
        monster.monsterAtri.des = 3;
        monster.monsterAtri.stg = 5 + monster.monsterAtri.lvl;
        monster.monsterAtri.con = 4 + monster.monsterAtri.lvl;
        monster.monsterAtri.pod = 1;
        monster.monsterAtri.lp = monster.monsterAtri.con * 5;
        monster.xpReward = rand() %  100 + (100 * monster.monsterAtri.lvl);
        monster.monsterAtri.ep = monster.monsterAtri.des * 5;
        monster.coinReward = rand() % (200 - 100) + 200;
        printf("Voce se depara com um ser bestial completamente enfurecido, toda sua pele parece estar em carne viva \n\n");
        system("pause");

    }
    
    int localReal = 1, combatEvent = 0;
    while(combatEvent == 0){
        system("cls");
        printf("Agente: %s \t\t   \t\t %s  \n", agente.name, monster.name); 
        printf("PV: %d / %d \t\t x \t\t PV: %d / %d \n", agente.agenteAtri.lp, agente.agenteAtri.con * 5, monster.monsterAtri.lp, monster.monsterAtri.con * 5);
        printf("PS: %d / %d \t\t   \t\t LVL: %d \n", agente.sp, agente.agenteAtri.pod * 7, monster.monsterAtri.lvl);
        printf("PE: %d / %d \n", agente.agenteAtri.ep, agente.agenteAtri.des * 5);

        printf("\n");

        localdaseta(1, localReal);printf("COMBATE\n");
        localdaseta(2, localReal);printf("RITUAIS\n");
        localdaseta(3, localReal);printf("ITENS\n");
        localdaseta(4, localReal);printf("FUGA\n");
        int c = getch();

        if(c == 119){
            if (localReal > 1) {
                localReal--;
            }
        } else if (c == 115) {
            if (localReal < 4) {
                localReal++;
            }
        }else if (c==13) {
            switch(localReal){
                case 1:
                monster.monsterAtri.lp -= movimentoAtaque(agente.agenteAtri.stg, 6);
                break;
                case 2:
                break;
                case 3:
                break;
                case 4:
                tryEscape = rand() % 2;
                if (tryEscape == 1) {
                    printf("Voce escapou, fim de combate \n\n");
                    system("pause");
                    combatEvent = 1;
                } else {
                    printf("Voce tenta escapar, tentativa falha \n\n");
                    system("pause");
                }
                break;
            }
        }
    }
}