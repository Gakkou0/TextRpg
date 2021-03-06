#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <string.h>
#include <windows.h>

struct atributosBasicos {
    int des; //pontos de destreza
    int stg; //pontos de força
    int con; //pontos de constituição
    int pod; //pontos de poder
    int lvl; //nivel
    int pv;  //pontos de vida atual
    int pvMAX; //pontos de vida maxima
    int pe; //pontos de esforço
};

struct rituais {
    char nome[30];
    int dado; //numero maximo do dado aleatorio
    int tipo; //tipo 0: buff/debuff de status, tipo 1: cura, tipo 2: dano
    int gastoPe; //quantidade a ser retirada de PE por uso de rituais
}ritual[3];

struct player {
    struct atributosBasicos atri;
    int exp; //experiencia do player
    int passaNivel;//experiencia necessaria para passar de nivel
    int ps; //pontos de sanidade
    char nome[20];//nome do agente
    int coin; //dinheiro do agente
    int rituaisAprendidos[3]; //1 para rituais aprendidos, 0 para slots vazios
    int contaMortes;//conta as mortes do agente 
    int pontos; //pontos utilizados para o levelUp
    int quantidadeItem[4]; //itens disponiveis no inventario
}agente;
 
typedef struct {
    struct atributosBasicos atri;
    char nome[20]; //nome do monstro
    int coinReward; //recompensa em dinheiro ao derrotar o monstro
    int xpReward; //recompensa em experiencia ao derrotar o monstro 
}monstro;

struct item {
    char nome[20];//nome do item
    int preco; //valor do item
}item[4];

int debug = 0;

int main (){
    system("cls");
    
    UINT CPAGE_UTF8 = 65001;
    UINT CPAGE_DEFAULT = GetConsoleOutputCP();
    SetConsoleOutputCP(CPAGE_UTF8);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15); //exibe os acentos e caracteres PT-BR

    mainMenu(&agente);

    if(debug == 1){
        debugMenu(&agente);
    }else{
        menuBase();    
    }

    return 0;
}

void agenteStatus() { //exibe na tela um resumo dos status do agente
    system("cls");
    printf("Agente: %s \n", agente.nome);
    printf("Pontos de vida: %d / %d \n", agente.atri.pv, agente.atri.pvMAX);
    printf("Pontos de sanidade: %d / %d \n", agente.ps, agente.atri.pod * 7);
    printf("Pontos de esforco: %d / %d \n", agente.atri.pe, agente.atri.pod * 5);
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
            getch();
            return dano;
        }else{
            for (i=0; i<numDados; i++){ //acerto não critico
                dano += (rand()%dadoAtaque)+1;
            }
            printf("Sucesso no teste! \n");
            printf("dano causado: %d \n", dano);
            getch();
            return dano;
        }
        
    }else{
        printf("Falha no teste \n"); //falha
        getch();
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

    monstro *monster = malloc(sizeof(monstro));
    system("cls");
    switch(randMonster) { //Gera o monstro
        case 0:
        strcpy(monster->nome, "Existido de energia");
        monster->atri.lvl = (rand() % agente.atri.lvl) + 1;
        monster->atri.des = 3;
        monster->atri.stg = 2;
        monster->atri.con = 3 + monster->atri.lvl;
        monster->atri.pod = 5 + monster->atri.lvl;
        monster->xpReward = rand() %  100 + (150 * monster->atri.lvl);
        monster->atri.pe = monster->atri.pod * 5;
        monster->coinReward = rand() % (200 - 100) + 200;
        monster->atri.pvMAX = monster->atri.con * 5;
        monster->atri.pv = monster->atri.pvMAX;

        dialogo("você se depara com um ser que incandesce uma luz azulada que parece flutuar em pleno ar, em sua face, uma expressão de puro desespero",0);
        break;
        case 1:
        strcpy(monster->nome, "Zombie de Sangue");
        monster->atri.lvl = (rand() % agente.atri.lvl) + 1;
        monster->atri.des = 3;
        monster->atri.stg = 5 + monster->atri.lvl;
        monster->atri.con = 4 + monster->atri.lvl;
        monster->atri.pod = 1;
        monster->xpReward = rand() %  100 + (150 * monster->atri.lvl);
        monster->atri.pe = monster->atri.pod * 5;
        monster->coinReward = rand() % (200 - 100) + 100;
        monster->atri.pvMAX = monster->atri.con * 5;
        monster->atri.pv = monster->atri.pvMAX;
        dialogo("você se depara com um ser bestial completamente enfurecido, toda sua pele parece estar em carne viva",0);

    }
    
    int localReal = 1, combatEvent = 0, turno = 0, vezPlayer = 0, vezMonstro = 0;
    while(combatEvent == 0){
        turno++;
        vezPlayer = 0; 
        vezMonstro = 0;
        while(vezPlayer==0){
            system("cls");
            printf("Turno: %d \n", turno);
            printf("Agente: %s \t\t   \t %s  \n", agente.nome, monster->nome); 
            printf("PV: %d / %d \t\t x \t\t PV: %d / %d \n", agente.atri.pv, agente.atri.pvMAX, monster->atri.pv, monster->atri.pvMAX);
            printf("PS: %d / %d \t\t   \t\t LVL: %d \n", agente.ps, agente.atri.pod * 7, monster->atri.lvl);
            printf("PE: %d / %d \n", agente.atri.pe, agente.atri.pod * 5);

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
            }else if (c == 13) {
                switch(localReal){
                    case 1:
                    monster->atri.pv = monster->atri.pv - movimentoAtaque(agente.atri.stg, 6);
                    vezPlayer = 1;
                    break;
                    case 2:
                    setRitual();
                    int dano = viewRitual(&agente, agente.rituaisAprendidos);
                    if (dano!=0){
                        monster->atri.pv = monster->atri.pv - dano;
                        vezPlayer = 1;
                    }
                    break;
                    case 3:
                    setItem();
                    if (viewItem(&agente, &item, agente.quantidadeItem)!=0){
                        vezPlayer = 1;
                    }
                    break;
                    case 4:
                    tryEscape = rand() % 2;
                    if (tryEscape == 1) {
                        printf("Você escapou, fim de combate \n\n");
                        system("pause");
                        combatEvent = 1;
                        vezMonstro = 1;
                        vezPlayer = 1;
                        free(monster);
                    } else {
                        printf("Você tenta escapar, tentativa falha \n\n");
                        system("pause");
                        vezPlayer = 1;
                    }
                    break;
                }
            }

            if (verificaMorte(monster->atri.pv)==1){
                agente.coin += monster->coinReward;
                agente.exp += monster->xpReward;

                printf("FIM DE COMBATE \n");
                printf("Recompensas: \n \n");
                printf("xp: +%d \n", monster->xpReward);
                printf("dinheiro: +%d \n", monster->coinReward);

                eventoSubirNivel(agente.atri.lvl, agente.exp);

                system("pause");
                free(monster);
                vezMonstro = 1;
                combatEvent = 1;
            }
        } while(vezMonstro==0){
            system("cls");
            printf("Turno: %d \n", turno);
            printf("Agente: %s \t\t   \t %s  \n", agente.nome, monster->nome); 
            printf("PV: %d / %d \t\t x \t\t PV: %d / %d \n", agente.atri.pv, agente.atri.pvMAX, monster->atri.pv, monster->atri.pvMAX);
            printf("PS: %d / %d \t\t   \t\t LVL: %d \n", agente.ps, agente.atri.pod * 7, monster->atri.lvl);
            printf("PE: %d / %d \n", agente.atri.pe, agente.atri.pod * 5);

            printf("\n");

            printf("Movimento do monstro \n\n");
            system("pause");
            if(randMonster == 1){
                agente.atri.pv -= movimentoAtaque(monster->atri.stg, 2);
            }else{
                agente.atri.pv -= movimentoAtaque(monster->atri.pod, 2);
            }
            vezMonstro = 1;
        }
        if(verificaMorte(agente.atri.pv)==1){
            eventoMorteAgente();
            combatEvent = 1;
            free(monster);
        }
    }
}

void debugMenu(struct player *agente){ //exibe o menu de debug
    
    int localReal = 1, debugMenu = 0;
    while(debugMenu == 0){
        system("cls");
        printf("\n\tMENU DE DEBUG \n \n"); 

        printf("\n");

        localdaseta(1, localReal);printf("TESTE COMBATE\n");
        localdaseta(2, localReal);printf("STATUS\n");
        localdaseta(3, localReal);printf("ITENS\n");
        localdaseta(4, localReal);printf("CARREGAR\n");
        localdaseta(5, localReal);printf("SALVAR\n");
        localdaseta(6, localReal);printf("TESTE BASE\n");
        localdaseta(7, localReal);printf("MAIN MENU\n");
        int c = getch();

        if(c == 119){
            if (localReal > 1) {
                localReal--;
            }
        } else if (c == 115) {
            if (localReal < 7) {
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
                setItem();
                viewItemMenu(agente->quantidadeItem);
                break;
                case 4:
                carregarJogo(agente);
                break;
                case 5:
                salvarJogo(agente);
                break;
                case 6:
                menuBase();
                break;
                case 7:
                mainMenu(&agente);
                debug = 0;
                debugMenu = 1;
            }
        }
    }
}

int confirmOption(int ph){ //função de confirmação binaria
    int localReal = 1, opcao = ph;
    while(opcao == 0){

        system("cls");

        printf("\n\tDeseja confirmar?\n\n");
    
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

void novoSave() { //Menu exibido apenas na criação de um novo save
    int deci = 0, localReal = 1, i;

    system("cls");
    dialogo("O paranormal, não vem para nosso mundo de maneira fácil", 0);
    dialogo("Uma membrana separa e protege a Realidade do Outro Lado", 0);
    dialogo("a dimensão dos monstros e demônios.", 0);
    dialogo("Essa barreira, porém, pode ser enfraquecida pelo Medo.", 0);
    dialogo("Se aproveitando disso, cultistas fazem rituais para romper a membrana e invocar seres sobrenaturais, causando caos e destruição.", 0);
    dialogo("Para impedir que esses planos malignos se concretizem", 0);
    dialogo("organizações de investigadores se mantêm em atividade por todo o globo.", 0);
    dialogo("Contra o paranormal, esses agentes são nossa primeira e última linha de defesa.", 0);
    system("cls");
    dialogo("O nome da organização é cursed seed", 0);
    dialogo("Você foi convidado a se tornar mais um combatente do paranormal", 0);

    system("cls");
    printf("Agente, por favor informe como deseja ser chamado: \n");
    printf("Agente: ");
    gets(agente.nome);

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
                agente.atri.pv = agente.atri.con * 8;
                agente.ps = agente.atri.pod * 7;
                agente.atri.pe = agente.atri.pod * 5;
                agente.coin=2000;
                agente.atri.pvMAX = agente.atri.con * 8;
                for (i = 0; i<3; i++){
                    agente.rituaisAprendidos[i] = 0;
                }
                agente.contaMortes = 0;
                agente.passaNivel = agente.atri.lvl * 1000;
                agente.pontos = 0;
                agente.quantidadeItem[1] = 1;
                agente.quantidadeItem[2] = 3;
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
                agente.atri.pv = agente.atri.con * 8;
                agente.ps = agente.atri.pod * 7;
                agente.atri.pe = agente.atri.pod * 5;
                agente.coin=2000;
                agente.atri.pvMAX = agente.atri.con * 8;
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
                agente.quantidadeItem[0] = 1;
                agente.quantidadeItem[3] = 3;


                deci = 1;  
                }
                
                break;
            }   
        }
    }

    system("cls");
    dialogo("Após uma longa viagem", 0);
    dialogo("Você chega até a base da Cursed Seed", 0);
    dialogo("estranhante, não tem ninguem na base", 0);
    dialogo("Ao que parece, todos os agentes estão em missões", 0);
    system("cls");
    dialogo("Srta. Rodrigues: Seja bem vindo agente, sugiro que se prepare bem", 0);
    dialogo("Sua primeira missão está proxima", 0);
    dialogo("Mas por enquanto, por que não anda pela base?", 0);
}

void mainMenu(struct player *agente) {//exibe o primeiro menu do jogo
    int localReal = 1, mainMenu = 0;
    while(mainMenu == 0){
        system("cls");
        
        printf("  _____                         _ \n");
        printf(" / ____|                       | |\n");
        printf("| |    _   _ _ __ ___  ___   __| |\n");
        printf("| |   | | | | '__/ __|/ _ |/  _` |\n");
        printf("| |___| |_| | |  |__ |  __/| (_| |\n");
        printf(" |_____|__,_|_|  |___/|___|| __,_|\n");
                                        
                                        


        printf("\n");

        localdaseta(1, localReal);printf("NOVO JOGO\n");
        localdaseta(2, localReal);printf("CARREGAR\n");
        localdaseta(3, localReal);printf("CREDITOS\n");
        localdaseta(4, localReal);printf("SAIR\n");
        localdaseta(5, localReal);printf("\n");
        int c = getch();

        if(c == 119){
            if (localReal > 1) {
                localReal--;
            }
        } else if (c == 115) {
            if (localReal < 4) {
                localReal++;
            }
        }else if (c==100){
            localReal=5;
        }else if (c==97){
            localReal=1;
        }else if (c==13) {
            switch(localReal){
                case 1:
                printf("Essa opção apagara seu ultimo save, deseja continuar? \n");
                system("pause");
                if (confirmOption(0)==1){
                    novoSave();
                    mainMenu = 1;
                }
                break;
                case 2:
                carregarJogo(agente);
                mainMenu = 1;
                break;
                case 3:
                system("cls");
                printf("---------CREDITOS---------\n\n");
                printf("Hugo Rodrigues\n");
                printf("Mabel Frota\n");
                printf("Ianny Gabriely\n");
                system("pause");
                break;
                case 4:
                exit(0);
                break;
                case 5:
                carregarJogo(agente);
                debug=1;
                mainMenu=1;
                break;

            }
        }
    }
}

salvarJogo(struct player *agente) {//Salva o progresso do agente
    FILE *save;
    save = fopen("save.txt", "w");
    fprintf(save, "nome: %s\n", agente->nome);
    fprintf(save, "vida: %d\n", agente->atri.pv);
    fprintf(save, "pontos de esforco %d\n", agente->atri.pe);
    fprintf(save, "vida maxima %d\n", agente->atri.pvMAX);
    fprintf(save, "sanidade: %d\n", agente->ps);
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
    fprintf(save, "quantidade item 1: %d \n", agente->quantidadeItem[0]);
    fprintf(save, "quantidade item 2: %d \n", agente->quantidadeItem[1]);
    fprintf(save, "quantidade item 3: %d \n", agente->quantidadeItem[2]);
    fprintf(save, "quantidade item 4: %d \n", agente->quantidadeItem[3]);

    fclose(save);

    printf("JOGO SALVO \n");
    system("pause");
}

carregarJogo(struct player *agente) {//Carrega o progresso do agente
    FILE *save;
    save = fopen("save.txt", "r");
    fscanf(save, "nome: %s\n", agente->nome);
    fscanf(save, "vida: %d\n", &agente->atri.pv);
    fscanf(save, "pontos de esforco %d\n", &agente->atri.pe);
    fscanf(save, "vida maxima %d\n", &agente->atri.pvMAX);
    fscanf(save, "sanidade: %d\n", &agente->ps);
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
    fscanf(save, "quantidade item 1: %d \n", &agente->quantidadeItem[0]);
    fscanf(save, "quantidade item 2: %d \n", &agente->quantidadeItem[1]);
    fscanf(save, "quantidade item 3: %d \n", &agente->quantidadeItem[2]);
    fscanf(save, "quantidade item 4: %d \n", &agente->quantidadeItem[3]);

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

int viewRitual(struct player *agente, int rituais[]) {
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
            if (localReal < 4) {
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
                    int valor = acaoCura(agente->atri.pod, ritual[1].dado, agente->atri.pe, ritual[1].gastoPe);
                    if (valor != 0){
                        agente->atri.pv += valor;
                        agente->atri.pe -= ritual[1].gastoPe;
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
                    int dano = movimentoAtaque(agente->atri.pod, ritual[2].dado);
                    agente->atri.pe -= ritual[2].gastoPe;
                    rituMenu = 1;
                    return dano;
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
            if (agente.atri.pvMAX == agente.atri.pv){
                printf("Vida cheia, impossivel usar \n");
                system("pause");
                return 0;
            }
            if (cura > (agente.atri.pvMAX - agente.atri.pv)){
                cura = agente.atri.pvMAX - agente.atri.pv;
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
            if (agente.atri.pvMAX == agente.atri.pv){
                printf("Vida cheia, impossivel usar \n");
                system("pause");
                return 0;
            }
            if (cura > (agente.atri.pvMAX - agente.atri.pv)){
                cura = agente.atri.pvMAX - agente.atri.pv;
                printf("Você se curou! \n");
                printf("vida restalrada: %d \n", cura);
                system("pause");
                return cura;
            }else{
                printf("Você se curou! \n");
                printf("vida restalrada: %d \n", cura);
                system("pause");
                return cura;
            }
            printf("Você se curou! \n");
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
        dialogo("Você sucumbiu ao paranormal",0);
        printf("\n");
        getch();
        agente.contaMortes++;
    }
    else if(agente.contaMortes==1){
        printf("Na sua cabeça...\n");
        getch();
        printf("Só se passa uma coisa...\n");
        getch();
        dialogo("Você sucumbiu ao paranormal",0);
        printf("\n");
        getch();
        agente.contaMortes++;
    }
    else if(agente.contaMortes==2){
        printf("Você lembra de todos que te amam...\n");
        getch();
        printf("Agora já é tarde...\n");
        getch();
        dialogo("Você sucumbiu ao paranormal",0);
        printf("\n");
        getch();
        agente.contaMortes++;
    }
    else if(agente.contaMortes==3){
        printf("Sua derrota já era certa...\n");
        getch();
        printf("Sempre foi...\n");
        getch();
        dialogo("Você sucumbiu ao paranormal",0);
        printf("\n");
        getch();
        agente.contaMortes++;
    }
    else if(agente.contaMortes>3){
        dialogo("Você sucumbiu ao paranormal",0);
        printf("\n");
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
        printf("NONO NIVEL DE AGENTE!\n\n");
        printf("+1 pontos de destribuição\n");
        agente.pontos++;
        getch();
    }
}

void menuBase() {
    int localReal = 1, mainBase = 0;
    while(mainBase == 0){
        system("cls");
        printf("\n\t\nCURSED SEED \n\n"); 

        printf("\n");

        localdaseta(1, localReal);printf("AGENTE\n");
        localdaseta(2, localReal);printf("LOJA\n");
        localdaseta(3, localReal);printf("TRANSCENDER\n");
        localdaseta(4, localReal);printf("DESCANSO\n");
        localdaseta(5, localReal);printf("SAIR DA BASE\n");
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
                agenteMenu(&agente);
                break;
                case 2:
                viewLoja ();
                break;
                case 3:
                transcender();
                break;
                case 4:
                printf("Você descansou o maximo que conseguiu\n");
                printf("Todos os atributos foram restaurados\n");
                agente.atri.pv = agente.atri.pvMAX;
                agente.atri.pe = agente.atri.pod * 5;
                agente.ps = agente.atri.pod * 7;
                salvarJogo(&agente);
                break;
                case 5:
                printf("\n Sair da base, significa sair em missão, tem certeza? \n");
                getch();
                if (confirmOption(0)!=0){
                    investigacao();
                }
                break;
            }
        }
    }
}

void transcender(){
    int localReal = 1, transcender = 0;
    
    if (agente.pontos==0){
        system("cls");
        printf("Você sente uma pressão gigantesca sobre seus ombros\n");
        printf("Você não tem pontos\n");
        getch();

        printf("Dano de sanidade: 10\n");
        agente.ps -= 10;
        getch();
        transcender = 1;
    }

    while(transcender == 0){
        if (agente.pontos == 0){
            transcender = 1;
        }
        system("cls");
        printf("TRANSCENDER \n\n");

        printf("Distribua seus pontos como achar necessario:\n");
        printf("Pontos disponiveis: %d \n\n", agente.pontos);

        printf("Pontos de força: %d\n",agente.atri.stg);
        printf("Pontos de constituição: %d\n",agente.atri.con);
        printf("Pontos de poder: %d\n",agente.atri.pod);
        printf("Pontos de destreza: %d\n",agente.atri.des);

        printf("\n");

        localdaseta(1, localReal);printf("FORÇA\n");
        localdaseta(2, localReal);printf("CONSTITUIÇÃO\n");
        localdaseta(3, localReal);printf("PODER\n");
        localdaseta(4, localReal);printf("DESTREZA\n");
        localdaseta(5, localReal);printf("SAIR DO CIRCULO\n");
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
                agente.pontos-=1;
                agente.atri.stg++;
                break;
                case 2:
                agente.pontos-=1;
                agente.atri.con++;
                break;
                case 3:
                agente.pontos-=1;
                agente.atri.pod++;
                break;
                case 4:
                agente.pontos-=1;
                agente.atri.des++;
                break;
                case 5:
                transcender = 1;
                break;
            }
        }
    }
}

void viewLoja (){
    int localReal = 1, lojaMenu= 0;
    while(lojaMenu == 0){
        system("cls");
        printf("OCULTA'S STORE \n\n"); 

        if(localReal == 1){
            printf("DESCRIÇÃO: Poção\n\n");
            printf("Esse item cura 10 pontos de vida\n");
            printf("Status: Esse item pode passar do número de PV MAXIMO até seu proximo descanso\n\n");
        }
        else if(localReal == 2){
            printf("DESCRIÇÃO: Pergaminho\n\n");
            printf("Esse item cura 10 pontos de esforço\n");
            printf("Status: Esse item pode passar do número de PE MAXIMO até seu proximo descanso\n\n");
        }else{
            printf("\n\n\n\n\n");
        }

        printf("\n");

        localdaseta(1, localReal);printf("POÇÃO - 200g\n");
        localdaseta(2, localReal);printf("PERGAMINHO - 300G\n");
        localdaseta(3, localReal);printf("SAIR\n");
        int c = getch();

        if(c == 119){
            if (localReal > 1) {
                localReal--;
            }
        } else if (c == 115) {
            if (localReal < 3) {
                localReal++;
            }
        }else if (c==13) {
            switch(localReal){
                case 1:
                if(agente.coin<200){
                    printf("saldo abaixo do valor\n");
                    system("pause");
                }else{
                    agente.quantidadeItem[2] += 1;
                    agente.coin -= 200;
                    printf("Item comprado\n");
                    system("pause");
                }
                break;
                case 2:
                if(agente.coin<300){
                    printf("saldo abaixo do valor\n");
                    system("pause");
                }else{
                    agente.quantidadeItem[3] += 1;
                    agente.coin -= 300;
                    printf("Item comprado\n");
                    system("pause");
                }
                break;
                case 3:
                lojaMenu = 1;
                break;
            }
        }
    }
}

void setItem(){
    strcpy(item[0].nome, "Grimorio");
    item[0].preco = 0;
    
    strcpy(item[1].nome, "Katana");
    item[1].preco = 0;

    strcpy(item[2].nome, "Poção");
    item[2].preco = 200;
    
    strcpy(item[3].nome, "Pergaminho");
    item[3].preco = 300;
}

int viewItem(struct player *agente, struct item *item, int quantItem[]) {
    int localReal = 1, itemMenu = 0, i, k = 1;
    char opcao[3][20];
        
        if(quantItem[0]==1){
            strcpy(opcao[0], item[0].nome);
        }else if (quantItem[1]==1){
            strcpy(opcao[0], item[1].nome);
        }

        for(i=2; i<4; i++){
            if(quantItem[i]>0){
                strcpy(opcao[k], item[i].nome);
                 k++;
            } else {
                strcpy(opcao[k], "------------------");
                k++;
            }
        }
    
    while(itemMenu == 0){
        system("cls");
        printf("ITENS \n\n");

        localdaseta(1, localReal);printf("%s \n", opcao[0]);
        localdaseta(2, localReal);printf("%s - %d \n", opcao[1], quantItem[2]);
        localdaseta(3, localReal);printf("%s - %d \n", opcao[2], quantItem[3]);
        localdaseta(4, localReal);printf("VOLTAR \n");
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
                break;
                case 2:
                if(quantItem[2]!=0){
                    agente->atri.pv += 10;
                    agente->quantidadeItem[2] -= 1;
                    printf("Vida Curada em 10 pontos");
                    getch();
                    itemMenu = 1;
                    return 1;
                }else{
                    printf("quantidade insulficiente");
                    getch();
                }
                break;
                case 3:
                if(quantItem[3]!=0){
                    agente->atri.pe += 10;
                    agente->quantidadeItem[3] -= 1;
                    printf("Esforço regenerado em 10 pontos");
                    getch();
                    itemMenu = 1;
                    return 1;
                }else{
                    printf("quantidade insulficiente");
                    getch();
                }
                break;
                case 4:
                return 0;
                itemMenu = 1;
                break;
            }
        }
    }
}

void viewItemMenu(int quantItem[]) {
    int localReal = 1, itemMenu = 0, i, k = 1;
    char opcao[3][20];
        
        if(quantItem[0]==1){
            strcpy(opcao[0], item[0].nome);
        }else if (quantItem[1]==1){
            strcpy(opcao[0], item[1].nome);
        }

        for(i=2; i<4; i++){
            if(quantItem[i]>0){
                strcpy(opcao[k], item[i].nome);
                 k++;
            } else {
                strcpy(opcao[k], "------------------");
                k++;
            }
        }
    
    while(itemMenu == 0){
        system("cls");
        printf("ITENS \n\n");

        if(localReal == 1 && quantItem[1]==1){
            printf("DESCRIÇÃO: Katana\n\n");
            printf("Arma de curta distância usada para destruir o paranormal\n");
            printf("Status: +2d10 de dano ao atacar usando gopves físicos \n\n");
        }
        else if(localReal == 1 && quantItem[0]==1){
            printf("DESCRIÇÃO: Grimorio\n\n");
            printf("Esse aplifica seus poderes paranormais\n");
            printf("Status: +2 pontos na pericia de PODER \n\n");
        }
        else if(localReal == 2){
            printf("DESCRIÇÃO: Poção\n\n");
            printf("Esse item cura 10 pontos de vida\n");
            printf("Status: Esse item pode passar do número de PV MAXIMO até seu proximo descanso\n\n");
        }
        else if(localReal == 3){
            printf("DESCRIÇÃO: Pergaminho\n\n");
            printf("Esse item cura 10 pontos de esforço\n");
            printf("Status: Esse item pode passar do número de PE MAXIMO até seu proximo descanso\n\n");
        }else{
            printf("\n\n\n\n\n");
        }

        localdaseta(1, localReal);printf("%s \n", opcao[0]);
        localdaseta(2, localReal);printf("%s - %d \n", opcao[1], quantItem[2]);
        localdaseta(3, localReal);printf("%s - %d \n", opcao[2], quantItem[3]);
        localdaseta(4, localReal);printf("VOLTAR \n");
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
                break;
                case 2:
                if(quantItem[2]!=0){
                    agente.atri.pv += 10;
                    agente.quantidadeItem[2] -= 1;
                    printf("Vida Curada em 10 pontos");
                    getch();
                    itemMenu = 1;
                    return 1;
                }else{
                    printf("quantidade insulficiente");
                    getch();
                }
                break;
                case 3:
                if(quantItem[3]!=0){
                    agente.atri.pe += 10;
                    agente.quantidadeItem[3] -= 1;
                    printf("Esforço regenerado em 10 pontos");
                    getch();
                    itemMenu = 1;
                    return 1;
                }else{
                    printf("quantidade insulficiente");
                    getch();
                }
                break;
                case 4:
                return 0;
                itemMenu = 1;
                break;
            }
        }
    }
}

void dialogo(char text[], int seconds)
{   
    int x; double y;
    // Adicionar delay de x segundos
    sleep(seconds);
    
        // Mostrar o texto
        for(x=0; text[x]!=NULL; x++)
        {
        printf("%c",text[x]);
        for(y=0; y<=899999; y++)
        {
        }
        }

    char enter = 0;
    while (enter != '\r' && enter != '\n')
    {
        enter = getchar();
    }
}

void agenteMenu (struct player *agente){ //exibe o menu do agente
    
    int localReal = 1, agenteMenu = 0;
    while(agenteMenu == 0){
        system("cls");
        printf("\n\tMENU DO AGENTE \n \n"); 

        printf("\n");

        localdaseta(1, localReal);printf("STATUS\n");
        localdaseta(2, localReal);printf("ITENS\n");
        localdaseta(3, localReal);printf("CARREGAR\n");
        localdaseta(4, localReal);printf("SALVAR\n");
        localdaseta(5, localReal);printf("VOLTAR\n");
        int c = getch();

        if(c == 119){
            if (localReal > 1) {
                localReal--;
            }
        } else if (c == 115) {
            if (localReal < 7) {
                localReal++;
            }
        }else if (c==13) {
            switch(localReal){
                case 1:
                agenteStatus();
                break;
                case 2:
                setItem();
                viewItemMenu(agente->quantidadeItem);
                break;
                case 3:
                carregarJogo(agente);
                break;
                case 4:
                salvarJogo(agente);
                break;
                case 5:
                agenteMenu = 1;
                break;
            }
        }
    }
}

void investigacao(){
    system("cls");
    dialogo("Você chega no local da missão, uma manssão completamente abandonada e aparentemente vazia",0);
    dialogo("Você sente ar muito denso e espesso",0);
    dialogo("Assim que aproxima da porta, você sente algo",0);
    dialogo("Quando você menos espera, ao entrar na massão...",0);
    
    eventoBatalha();

    if (agente.exp > 0 && agente.atri.lvl == 1){
    system("cls");
    dialogo("???: nada mal para uma primeira missão",0);
    dialogo("???: Vai ter tanta sorte na proxima?",0);
    dialogo("???: ...",0);
    dialogo("Um completo silêncio toma o local",0);
    dialogo("Você retorna para a base...",0);

    system("cls");
    dialogo("Srta. Rodrigues: Pelo visto voltou vivo, ótimo!",0);
    dialogo("Vejo que é um agente mais forte que pensei.", 0);
    dialogo("Sempre que concluir missões serão concedidas recompensas a você", 0);
    dialogo("tanto em experiência quanto dinheiro", 0);
    dialogo("Logo, mais missões vão aparecer, até lá você pode continuar treinando e melhorando seu nivel de agente", 0);

    system("cls");
    dialogo("fim do modo historia(por enquanto)",0);
    }
}