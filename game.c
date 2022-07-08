#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <ctype.h>
#include <string.h>

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
void mainMenu();
void firstOpen();
int confirmOption(int ph);
void firstMenu();
salvarJogo(struct player *agente);

int main (){
    system("cls");

    firstMenu();

    mainMenu(&agente);

    return 0;
}

void agenteStatus() {
    system("cls");
    printf("Agente: %s \n", agente.name);
    printf("Pontos de vida: %d / %d \n", agente.agenteAtri.lp, agente.agenteAtri.con * 5);
    printf("Pontos de sanidade: %d / %d \n", agente.sp, agente.agenteAtri.pod * 7);
    printf("Pontos de esforco: %d / %d \n", agente.agenteAtri.ep, agente.agenteAtri.des * 5);
    printf("Dinheiro: %d \n", agente.coin);
    printf("XP: %d / %d \n", agente.exp, agente.agenteAtri.lvl * 1000);
    system("pause");
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

        if (monster.monsterAtri.lp<=0){
            agente.coin += monster.coinReward;
            agente.exp += monster.xpReward;

            printf("FIM DE COMBATE \n");
            printf("Recompensas: \n \n");
            printf("xp: +%d \n", monster.xpReward);
            printf("dinheiro: +%d \n", monster.coinReward);

            system("pause");
            combatEvent = 1;
        }
    }
}

void mainMenu(struct player *agente){
    
    int localReal = 1, mainMenu = 0;
    while(mainMenu == 0){
        system("cls");
        printf("MENU PRINCIPAL \n \n"); 

        printf("\n");

        localdaseta(1, localReal);printf("TESTE COMBATE\n");
        localdaseta(2, localReal);printf("STATUS\n");
        localdaseta(3, localReal);printf("ITENS\n");
        localdaseta(4, localReal);printf("SALVAR\n");
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
                eventoBatalha();
                break;
                case 2:
                agenteStatus();
                break;
                case 3:
                break;
                case 4:
                salvarJogo(agente);

                break;
            }
        }
    }
}

int confirmOption(int ph){
    int localReal = 1, opcao = ph;
    while(opcao == 0){

        system("cls");
        localdaseta(1, localReal);printf("confirmar\n");
        localdaseta(2, localReal);printf("voltar\n");

        int c = getch();

        if(c == 119){
            if (localReal > 1) {
                localReal--;
            }
        } else if (c == 115) {
            if (localReal < 2) {
                localReal++;
            }
        }else if (c==13) {
            switch(localReal){
                case 1:
                opcao=1;
                return 1;
                break;
                case 2:
                opcao=1;
                return 2;
                break;
            }
        }
    }

}

void firstOpen() {
    int deci = 0, localReal = 1;

    system("cls");
    printf("Seja Bem vindo Agente, por favor informe como deseja ser chamado: \n");
    printf("Agente: ");
    gets(agente.name);

    while (deci==0){
        system("cls");
        printf("\nPara o inicio de nossa jornada, defina seu estilo de combate: \n");
        localdaseta(1, localReal);printf("Combatente \n");
        localdaseta(2, localReal);printf("Ocultista \n");

        int c = getch();

        if(c == 119){
            if (localReal > 1) {
                localReal--;
            }
        } else if (c == 115) {
            if (localReal < 2) {
                localReal++;
            }
        }else if (c==13) {
            switch(localReal){
                case 1:
                system("cls");
                printf("Combatentes sao uma classe focada em forca fisica e combate corpo a corpo \nno entanto nao possuem tanto poder vindo 'do outro lado', deseja selecionar essa classe? \n \n ");
                printf("Item exclusivo: Katana \n");
                system("pause");
                if (confirmOption(0)==1){
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
                deci = 1;    
                }
                break;
                case 2:
                system("cls");
                printf("Ocultistas sao expecialistas no uso de rituais para os mais diversos fins \nno entanto sao muito fracos em combates corpor a corpo, deseja selecionar essa classe? \n\n");
                printf("Item exclusivo: Grimorio \n");
                system("pause");
                if (confirmOption(0)==1){
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
                deci = 1;  
                }
                
                break;
            }   
        }
    }
}

void firstMenu() {
    int localReal = 1, mainMenu = 0;
    while(mainMenu == 0){
        system("cls");
        printf("RPG *coloque o nome aqui* \n \n"); 

        printf("\n");

        localdaseta(1, localReal);printf("NOVO JOGO\n");
        localdaseta(2, localReal);printf("CARREGAR\n");
        localdaseta(3, localReal);printf("CREDITOS\n");
        localdaseta(4, localReal);printf("SAIR\n");
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
                printf("Essa opção apagara seu ultimo save, deseja continuar? \n");
                system("pause");
                if (confirmOption(0)==1){
                    firstOpen();
                    mainMenu = 1;
                }
                break;
                case 2:
                break;
                case 3:
                system("cls");
                printf("---------CREDITOS---------\n\n");
                printf("Desenvolvedor: Hugo Rodrigues\n");
                //vejo depois
                system("pause");
                break;
                case 4:
                exit(0);
                break;
            }
        }
    }
}

salvarJogo(struct player *agente) {
    FILE *save;
    save = fopen("save.txt", "w");
    fprintf(save, "nome: %s\n", agente->name);
    fprintf(save, "vida: %d\n", agente->agenteAtri.lp);
    fprintf(save, "sanidade: %d\n", agente->sp);
    fprintf(save, "nivel: %d\n", agente->agenteAtri.lvl);
    fprintf(save, "dinheiro: %d\n", agente->coin);
    fprintf(save, "força: %d\n", agente->agenteAtri.stg);
    fprintf(save, "destreza: %d\n", agente->agenteAtri.des);
    fprintf(save, "poder: %d\n", agente->agenteAtri.pod);
    fprintf(save, "constiruição: %d\n", agente->agenteAtri.con);
    fprintf(save, "xp: %d\n", agente->exp);

    fclose(save);

    printf("JOGO SALVO \n");
    system("pause");
}