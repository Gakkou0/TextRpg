#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <ctype.h>
#include <string.h>

struct baseAttribures {
    int des; //pontos de destreza
    int stg; //pontos de força
    int con; //pontos de constituição
    int pod; //pontos de poder
    int lvl; //nivel
    int lp;  //pontos de vida atual
    int lpMAX; //potnos de vida maxima
    int ep; //pontos de esforço
};

struct rituais {
    char nome[30];
    int dado; //numero maximo do dado aleatorio
    int tipo; //tipo 0: buff/debuff de status, tipo 1: cura, tipo 2: dano
    int gastoPe; //quantidade a ser retirada de PE por uso de rituais
}ritual[3];

struct player {
    struct baseAttribures atri;
    int exp; //experiencia do player
    int passaNivel;
    int sp; //pontos de sanidade
    char name[20];
    int coin; //dinheiro do Agente
    int rituaisAprendidos[3]; //1 para rituais aprendidos, 0 para slots vazios
    int contaMortes;
    int pontos;
}agente;

struct monster {
    struct baseAttribures atri;
    char name[20];
    int coinReward; //recompensa em dinheiro ao derrotar o monstro
    int xpReward; //recompensa em experiencia ao derrotar o monstro 
}monster;

int main (){
    system("cls");

    firstMenu(&agente);

    mainMenu(&agente);

    return 0;
}

void agenteStatus() { //exibe na tela um resumo dos status do agente
    system("cls");
    printf("Agente: %s \n", agente.name);
    printf("Pontos de vida: %d / %d \n", agente.atri.lp, agente.atri.lpMAX);
    printf("Pontos de sanidade: %d / %d \n", agente.sp, agente.atri.pod * 7);
    printf("Pontos de esforco: %d / %d \n", agente.atri.ep, agente.atri.pod * 5);
    printf("Dinheiro: %d \n", agente.coin);
    printf("Nivel do Agente: %d \n", agente.atri.lvl);
    printf("XP: %d / %d \n", agente.exp, agente.atri.lvl * 1000);

    system("pause");
}

int movimentoAtaque(int atriAtacante, int dadoAtaque) { //execulta a verificação de sucesso do teste, calcula e retorna o dano do ataque
    
    srand(time(NULL));
    int numDados = atriAtacante/2 + atriAtacante%2, i = 0; //definição do numero de dados a serem girados tanto para o teste de sucesso quanto o valor maximo do dano
    int dado, maiorDado = 0, dt = 20 - (numDados * 5), dano = 0; //dt = dificuldade do teste, quanto maior o atributo utilizado para o ataque, mais facil se torna o teste

    for (i=0; i<numDados; i++){ //vericação de sucesso do teste
        dado = (rand()%20)+1;
        if (i==0){
            maiorDado = dado;
        }
        if (dado > maiorDado){
            maiorDado = dado;
        }
    }

    if (maiorDado >= dt) {
        if (maiorDado == 20){ //acerto critico
        
            dano = numDados * dadoAtaque;
            printf("Acerto Critico! \n");
            printf("dano causado: %d \n", dano);
            system("pause");
            return dano;
        }else{
            for (i=0; i<numDados; i++){ //acerto não critico
                dano += (rand()%dadoAtaque)+1;
            }
            printf("Sucesso no teste! \n");
            printf("dano causado: %d \n", dano);
            system("pause");
            return dano;
        }
        
    }else{
        printf("Falha no teste \n"); //falha
        system("pause");
        return 0;
    }

}

void localdaseta(int realPosition, int posicaoDaTecla) {  //função responsavel pela a seta de todos os menus
    if (realPosition == posicaoDaTecla) {
        printf("\t=> ");
    }
    else {
        printf("\t   ");
    }
}

void eventoBatalha(){ //Gera o um monstro de nivel de 1 a 3, exibe e controla o menu de combate
    srand(time(NULL));
    int randMonster = rand () % 2;
    int tryEscape;
    system("cls");
    switch(randMonster) { //Gera o monstro
        case 0:
        strcpy(monster.name, "Existido de energia");
        monster.atri.lvl = (rand() % 3) + 1;
        monster.atri.des = 3;
        monster.atri.stg = 2;
        monster.atri.con = 3 + monster.atri.lvl;
        monster.atri.pod = 5 + monster.atri.lvl;
        monster.xpReward = rand() %  100 + /*(100 * monster.atri.lvl)*/ 1000;
        monster.atri.ep = monster.atri.pod * 5;
        monster.coinReward = rand() % (200 - 100) + 200;
        monster.atri.lpMAX = monster.atri.con * 5;
        monster.atri.lp = monster.atri.lpMAX;

        printf("Voce se depara com um ser que incandesce uma luz azulada que parece flutuar em pleno ar, em sua face, uma expressão de puro desespero \n\n");
        system("pause");
        break;
        case 1:
        strcpy(monster.name, "Zombie de Sangue");
        monster.atri.lvl = (rand() % 3) + 1;
        monster.atri.des = 3;
        monster.atri.stg = 5 + monster.atri.lvl;
        monster.atri.con = 4 + monster.atri.lvl;
        monster.atri.pod = 1;
        monster.xpReward = rand() %  100 + /*(100 * monster.atri.lvl)*/ 1000;
        monster.atri.ep = monster.atri.pod * 5;
        monster.coinReward = rand() % (200 - 100) + 100;
        monster.atri.lpMAX = monster.atri.con * 5;
        monster.atri.lp = monster.atri.lpMAX;
        printf("Voce se depara com um ser bestial completamente enfurecido, toda sua pele parece estar em carne viva \n\n");
        system("pause");

    }
    
    int localReal = 1, combatEvent = 0, turno = 0, vezPlayer = 0, vezMonstro = 0;
    while(combatEvent == 0){
        turno++;
        vezPlayer = 0; 
        vezMonstro = 0;
        while(vezPlayer==0){
            system("cls");
            printf("Turno: %d \n", turno);
            printf("Agente: %s \t\t   \t\t %s  \n", agente.name, monster.name); 
            printf("PV: %d / %d \t\t x \t\t PV: %d / %d \n", agente.atri.lp, agente.atri.lpMAX, monster.atri.lp, monster.atri.lpMAX);
            printf("PS: %d / %d \t\t   \t\t LVL: %d \n", agente.sp, agente.atri.pod * 7, monster.atri.lvl);
            printf("PE: %d / %d \n", agente.atri.ep, agente.atri.pod * 5);

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
                    monster.atri.lp -= movimentoAtaque(agente.atri.stg, 6);
                    vezPlayer = 1;
                    break;
                    case 2:
                    setRitual();
                    if (viewRitual(&agente, &monster, agente.rituaisAprendidos)!=0){
                        vezPlayer = 1;
                    }
                    break;
                    case 3:
                    break;
                    case 4:
                    tryEscape = rand() % 2;
                    if (tryEscape == 1) {
                        printf("Voce escapou, fim de combate \n\n");
                        system("pause");
                        combatEvent = 1;
                        vezMonstro = 1;
                        vezPlayer = 1;
                    } else {
                        printf("Voce tenta escapar, tentativa falha \n\n");
                        system("pause");
                        vezPlayer = 1;
                    }
                    break;
                }
            }

            if (verificaMorte(monster.atri.lp)==1){
                agente.coin += monster.coinReward;
                agente.exp += monster.xpReward;

                printf("FIM DE COMBATE \n");
                printf("Recompensas: \n \n");
                printf("xp: +%d \n", monster.xpReward);
                printf("dinheiro: +%d \n", monster.coinReward);

                eventoSubirNivel(agente.atri.lvl, agente.exp);

                system("pause");
                vezMonstro = 1;
                combatEvent = 1;
            }
        } while(vezMonstro==0){
            system("cls");
            printf("Turno: %d \n", turno);
            printf("Agente: %s \t\t   \t\t %s  \n", agente.name, monster.name); 
            printf("PV: %d / %d \t\t x \t\t PV: %d / %d \n", agente.atri.lp, agente.atri.lpMAX, monster.atri.lp, monster.atri.lpMAX);
            printf("PS: %d / %d \t\t   \t\t LVL: %d \n", agente.sp, agente.atri.pod * 7, monster.atri.lvl);
            printf("PE: %d / %d \n", agente.atri.ep, agente.atri.pod * 5);

            printf("\n");

            printf("Movimento do monstro \n\n");
            system("pause");
            if(randMonster == 1){
                agente.atri.lp -= movimentoAtaque(monster.atri.stg, 2);
            }else{
                agente.atri.lp -= movimentoAtaque(monster.atri.pod, 2);
            }
            system("pause");
            vezMonstro = 1;
        }
        if(verificaMorte(agente.atri.lp)==1){
            eventoMorteAgente();
            combatEvent = 1;
        }
    }
}

void mainMenu(struct player *agente){ //exibe o menu principal
    
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

int confirmOption(int ph){ //função de confirmação binaria
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

void firstOpen() { //Menu exibido apenas na criação de um novo save
    int deci = 0, localReal = 1, i;

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
                if (confirmOption(0)==1){//atribuição dos status do agente
                agente.atri.lvl = 1;
                agente.exp = 0;
                agente.atri.des = 3;
                agente.atri.stg = 4;
                agente.atri.con = 4;
                agente.atri.pod = 1;
                agente.atri.lp = agente.atri.con * 8;
                agente.sp = agente.atri.pod * 7;
                agente.atri.ep = agente.atri.pod * 5;
                agente.coin=2000;
                agente.atri.lpMAX = agente.atri.con * 8;
                for (i = 0; i<3; i++){
                    agente.rituaisAprendidos[i] = 0;
                }
                agente.contaMortes = 0;
                agente.passaNivel = agente.atri.lvl * 1000;
                agente.pontos = 0;
                deci = 1;    
                }
                break;
                case 2:
                system("cls");
                printf("Ocultistas sao expecialistas no uso de rituais para os mais diversos fins \nno entanto sao muito fracos em combates corpor a corpo, deseja selecionar essa classe? \n\n");
                printf("Item exclusivo: Grimorio \n");
                system("pause");
                if (confirmOption(0)==1){//atribuição dos status do agente
                agente.atri.lvl = 1;
                agente.exp = 0;
                agente.atri.des = 2;
                agente.atri.stg = 1;
                agente.atri.con = 3;
                agente.atri.pod = 5;
                agente.atri.lp = agente.atri.con * 8;
                agente.sp = agente.atri.pod * 7;
                agente.atri.ep = agente.atri.pod * 5;
                agente.coin=2000;
                agente.atri.lpMAX = agente.atri.con * 8;
                for (i = 0; i<3; i++){
                    if(i==0){
                        agente.rituaisAprendidos[i] = 0;
                    }else{
                        agente.rituaisAprendidos[i] = 1;
                    }
                }
                agente.contaMortes = 0;
                agente.passaNivel = agente.atri.lvl * 1000;
                agente.pontos = 0;
                deci = 1;  
                }
                
                break;
            }   
        }
    }
}

void firstMenu(struct player *agente) {//exibe o primeiro menu do jogo
    int localReal = 1, firstMenu = 0;
    while(firstMenu == 0){
        system("cls");
        
        printf("  _____                        _ \n");
        printf(" / ____|                      | |\n");
        printf("| |    _   _ _ __ ___  ___  __| |\n");
        printf("| |   | | | | '__/ __|/ _ || _` |\n");
        printf("| |___| |_| | |  |__ |  __/ (_| |\n");
        printf(" |_____|__,_|_|  |___/|___||__,_|\n");
                                        
                                        


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
                    firstMenu = 1;
                }
                break;
                case 2:
                carregarJogo(agente);
                firstMenu = 1;
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

salvarJogo(struct player *agente) {//Salva o progresso do agente
    FILE *save;
    save = fopen("save.txt", "w");
    fprintf(save, "nome: %s\n", agente->name);
    fprintf(save, "vida: %d\n", agente->atri.lp);
    fprintf(save, "pontos de esforco %d\n", agente->atri.ep);
    fprintf(save, "vida maxima %d\n", agente->atri.lpMAX);
    fprintf(save, "sanidade: %d\n", agente->sp);
    fprintf(save, "nivel: %d\n", agente->atri.lvl);
    fprintf(save, "dinheiro: %d\n", agente->coin);
    fprintf(save, "força: %d\n", agente->atri.stg);
    fprintf(save, "destreza: %d\n", agente->atri.des);
    fprintf(save, "poder: %d\n", agente->atri.pod);
    fprintf(save, "constiruição: %d\n", agente->atri.con);
    fprintf(save, "xp: %d\n", agente->exp);
    fprintf(save, "Aprendeu Ritual 1: %d\n", agente->rituaisAprendidos[0]);
    fprintf(save, "Aprendeu Ritual 2: %d\n", agente->rituaisAprendidos[1]);
    fprintf(save, "Aprendeu Ritual 3: %d\n", agente->rituaisAprendidos[2]);
    fprintf(save, "Numero de mortes: %d\n", agente->contaMortes);
    fprintf(save, "Proximo nivel: %d \n", agente->passaNivel);
    fprintf(save, "pontos de nivel %d \n", agente->pontos);


    fclose(save);

    printf("JOGO SALVO \n");
    system("pause");
}

carregarJogo(struct player *agente) {//Carrega o progresso do agente
    FILE *save;
    save = fopen("save.txt", "r");
    fscanf(save, "nome: %s\n", agente->name);
    fscanf(save, "vida: %d\n", &agente->atri.lp);
    fscanf(save, "pontos de esforco %d\n", &agente->atri.ep);
    fscanf(save, "vida maxima %d\n", &agente->atri.lpMAX);
    fscanf(save, "sanidade: %d\n", &agente->sp);
    fscanf(save, "nivel: %d\n", &agente->atri.lvl);
    fscanf(save, "dinheiro: %d\n", &agente->coin);
    fscanf(save, "força: %d\n", &agente->atri.stg);
    fscanf(save, "destreza: %d\n", &agente->atri.des);
    fscanf(save, "poder: %d\n", &agente->atri.pod);
    fscanf(save, "constiruição: %d\n", &agente->atri.con);
    fscanf(save, "xp: %d\n", &agente->exp);
    fscanf(save, "Aprendeu Ritual 1: %d\n", &agente->rituaisAprendidos[0]);
    fscanf(save, "Aprendeu Ritual 2: %d\n", &agente->rituaisAprendidos[1]);
    fscanf(save, "Aprendeu Ritual 3: %d\n", &agente->rituaisAprendidos[2]);
    fscanf(save, "Numero de mortes: %d\n", &agente->contaMortes);
    fscanf(save, "Proximo nivel: %d \n", &agente->passaNivel);
    fscanf(save, "pontos de nivel %d \n", &agente->pontos);

    fclose(save);

    printf("JOGO CARREGADO \n");
    system("pause");
}

void setRitual(){
strcpy(ritual[0].nome, "PARADOXO");
ritual[0].dado = 3;
ritual[0].tipo = 0;
ritual[0].gastoPe = 1;

strcpy(ritual[1].nome, "ENVELHECIMENTO LOCALIZADO");
ritual[1].dado = 6;
ritual[1].tipo = 1;
ritual[1].gastoPe = 3;

strcpy(ritual[2].nome, "DECADENCIA");
ritual[2].dado = 6;
ritual[2].tipo = 2;
ritual[2].gastoPe = 2;
}

int viewRitual(struct player *agente, struct monster *monster, int rituais[]) {
    int localReal = 1, rituMenu = 0, i;
    char opcao[3][30];
    
        for(i=0; i<3; i++){
            if(rituais[i]==0){
                strcpy(opcao[i], "Nao aprendido");
            }
            if(rituais[i]==1){
                strcpy(opcao[i], ritual[i].nome);
            }
        }
    
    while(rituMenu == 0){
        system("cls");
        printf("RITUAIS \n\n");

        localdaseta(1, localReal);printf("%s \n", opcao[0]);
        localdaseta(2, localReal);printf("%s \n", opcao[1]);
        localdaseta(3, localReal);printf("%s \n", opcao[2]);
        localdaseta(4, localReal);printf("VOLTAR \n");
        int c = getch();

        if(c == 119){
            if (localReal > 1) {
                localReal--;
            }
        } else if (c == 115) {
            if (localReal < 5) {
                localReal++;
            }
        }else if (c==13) {
            switch(localReal){
                case 1:
                if (strlen(opcao[0])==13){
                    printf("Ritual não aprendido\n\n");
                    system("pause");
                }else{
                    
                }
                break;
                case 2:
                if (strlen(opcao[1])==13){
                    printf("Ritual não aprendido\n\n");
                    system("pause");
                }else{
                    int valor = acaoCura(agente->atri.pod, ritual[1].dado, agente->atri.ep, ritual[1].gastoPe);
                    if (valor != 0){
                        agente->atri.lp += valor;
                        agente->atri.ep -= ritual[1].gastoPe;
                        rituMenu = 1;
                        return 1;
                    }
                }
                break;
                case 3:
                if (strlen(opcao[2])==13){
                    printf("Ritual não aprendido\n\n");
                    system("pause");
                }else{ 
                    monster->atri.lp -= movimentoAtaque(agente->atri.pod, ritual[2].dado);
                    agente->atri.ep -= ritual[2].gastoPe;
                    rituMenu = 1;
                    return 1;
                }
                break;
                case 4:
                return 0;
                rituMenu = 1;
                break;
            }
        }
    }
}

int acaoCura(int poderAgente, int dadoCura, int peAtual, int gastoPe) {//calcula e retorna a cura de um item ou ritual
    
    if (peAtual < gastoPe){
        printf("PE insuficiente");
        system("pause");
        return 0;
    }
    srand(time(NULL));
    int numDados = poderAgente/2 + poderAgente%2, i;
    int dado, maiorDado = 0, dt = 20 - (numDados * 5), cura = 0; 

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
            cura = numDados * dadoCura;
            if (agente.atri.lpMAX == agente.atri.lp){
                printf("Vida cheia, impossivel usar \n");
                system("pause");
                return 0;
            }
            if (cura > (agente.atri.lpMAX - agente.atri.lp)){
                cura = agente.atri.lpMAX - agente.atri.lp;
                printf("Uma cura poderosa! \n");
                printf("vida restalrada: %d \n", cura);
                system("pause");
                return cura;
            }else{
                printf("Uma cura poderosa! \n");
                printf("vida restalrada: %d \n", cura);
                system("pause");
                return cura;
            }
        }else{
            for (i=0; i<numDados; i++){
                cura += (rand()%dadoCura)+1;
            }
            if (agente.atri.lpMAX == agente.atri.lp){
                printf("Vida cheia, impossivel usar \n");
                system("pause");
                return 0;
            }
            if (cura > (agente.atri.lpMAX - agente.atri.lp)){
                cura = agente.atri.lpMAX - agente.atri.lp;
                printf("Voce se curou! \n");
                printf("vida restalrada: %d \n", cura);
                system("pause");
                return cura;
            }else{
                printf("Voce se curou! \n");
                printf("vida restalrada: %d \n", cura);
                system("pause");
                return cura;
            }
            printf("Voce se curou! \n");
            printf("vida restalrada: %d \n", cura);
            system("pause");
            return cura;
        }
        
    }
}

int verificaMorte(int vidaAlvo){
    if (vidaAlvo <= 0){
        return 1;
    }else{
        return 0;
    }
}

void eventoMorteAgente() {
    system("cls");
    if(agente.contaMortes==0){
        printf("Em seu ultimo momento...\n");
        getch();
        printf("Você percebe...\n");
        getch();
        printf("Você sucumbiu ao paranormal\n");
        getch();
        agente.contaMortes++;
    }
    else if(agente.contaMortes==1){
        printf("Na sua cabeça...\n");
        getch();
        printf("Só se passa uma coisa...\n");
        getch();
        printf("Você sucumbiu ao paranormal\n");
        getch();
        agente.contaMortes++;
    }
    else if(agente.contaMortes==2){
        printf("Você lembra de todos que te amam...\n");
        getch();
        printf("Agora já é tarde...\n");
        getch();
        printf("Você sucumbiu ao paranormal\n");
        getch();
        agente.contaMortes++;
    }
    else if(agente.contaMortes==3){
        printf("Sua derrota já era certa...\n");
        getch();
        printf("Sempre foi...\n");
        getch();
        printf("Você sucumbiu ao paranormal\n");
        getch();
        agente.contaMortes++;
    }
    else if(agente.contaMortes>3){
        printf("Você sucumbiu ao paranormal\n");
        getch();
        agente.contaMortes++;
    }

    int localReal = 1, morteMenu = 0;
    while(morteMenu == 0){
        system("cls");

        printf("Agente, deseja continuar? \n");

        localdaseta(1, localReal);printf("Devo continuar em frente \n");
        localdaseta(2, localReal);printf("Desistir \n");

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
                carregarJogo(&agente);
                morteMenu = 1;
                break;
                case 2:
                exit(0);
            }
        }
    }
}

void eventoSubirNivel(int nivel, int xp){
    int passarNivel = nivel * 1000;
    if (xp>=passarNivel){
        agente.atri.lvl++;
        agente.passaNivel = agente.atri.lvl * 1000;
        printf("NONO NIVEL DE AGENTE!");
        printf("+1 pontos de destribuição");
        agente.pontos++;
        getch();
    }
}