#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
#endif

// =====================================
// CONFIGURAÇÕES
// =====================================
int ALTURA;
int LARGURA;

float perc_ovelha    = 0.05;
float perc_lobo      = 0.03;
float perc_cacador   = 0.02;
float perc_obstaculo = 0.20;

#define TURNOS 500

// =====================================
// STRUCT
// =====================================
typedef struct {
    int x; 
    int y; 
    char* simbolo; 
    int ativa;
} Personagem;

// =====================================
// TABULEIRO
// =====================================
char*** tabuleiro;

// =====================================
// POPULAÇÕES (Globais para realloc)
// =====================================
Personagem *ovelhas;
Personagem *lobos;
Personagem *cacadores;
Personagem *obstaculos;

int qtdOvelhas;
int qtdLobos;
int qtdCacadores;
int qtdObstaculos;

int ovelhasVivas; 
int lobosVivos;
int totalMoveis; // Movido para global para atualizar no nascimento

// =====================================
// LIMPEZA DE TELA
// =====================================
void limparTela() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// =====================================
// PAUSA
// =====================================
void pausar() {
#ifdef _WIN32
    Sleep(500);
#else
    usleep(500000);
#endif
}

// =====================================
// CRIA TABULEIRO RETANGULAR
// =====================================
void criarTabuleiro() {
    tabuleiro = malloc(ALTURA * sizeof(char**));
    for(int i = 0; i < ALTURA; i++) {
        tabuleiro[i] = malloc(LARGURA * sizeof(char*));
    }
}

// =====================================
// LIBERA MEMÓRIA
// =====================================
void liberarMemoria() {
    for(int i = 0; i < ALTURA; i++) {
        free(tabuleiro[i]);
    }
    free(tabuleiro);

    free(ovelhas);
    free(lobos);
    free(cacadores);
    free(obstaculos);
}

// =====================================
// LIMPA TABULEIRO
// =====================================
void limparTabuleiro() {
    for(int i = 0; i < ALTURA; i++) {
        for(int j = 0; j < LARGURA; j++) {
            tabuleiro[i][j] = "  "; 
        }
    }
}

// =====================================
// VERIFICA OCUPAÇÃO
// =====================================
int ocupado(int x, int y) {
    if (tabuleiro[x][y] != "  ") {
        return 1;
    }
    return 0;
}

// =====================================
// CALCULA POPULAÇÃO
// =====================================
int calc(float perc, int min, int max) {
    int qtd = (int)(ALTURA * LARGURA * perc);
    if(qtd < min)  qtd = min;
    if(qtd > max)  qtd = max;
    return qtd;
}

// =====================================
// POSICIONAMENTO RETANGULAR
// =====================================
void posicionar() {
    limparTabuleiro();

    // OVELHAS
    for(int i = 0; i < qtdOvelhas; i++) {
        ovelhas[i].simbolo = "🐑";
        ovelhas[i].ativa = 1;
        do {
            ovelhas[i].x = rand() % ALTURA;
            ovelhas[i].y = rand() % LARGURA;
        } while(ocupado(ovelhas[i].x, ovelhas[i].y));
        tabuleiro[ovelhas[i].x][ovelhas[i].y] = ovelhas[i].simbolo;
    }

    // LOBOS
    for(int i = 0; i < qtdLobos; i++) {
        lobos[i].simbolo = "🐺";
        lobos[i].ativa = 1;
        do {
            lobos[i].x = rand() % ALTURA;
            lobos[i].y = rand() % LARGURA;
        } while(ocupado(lobos[i].x, lobos[i].y));
        tabuleiro[lobos[i].x][lobos[i].y] = lobos[i].simbolo;
    }

    // CACADORES
    for(int i = 0; i < qtdCacadores; i++) {
        cacadores[i].simbolo = "🤠";
        cacadores[i].ativa = 1;
        do {
            cacadores[i].x = rand() % ALTURA;
            cacadores[i].y = rand() % LARGURA;
        } while(ocupado(cacadores[i].x, cacadores[i].y));
        tabuleiro[cacadores[i].x][cacadores[i].y] = cacadores[i].simbolo;
    }

    // OBSTÁCULOS (Árvores 🌲)
    for(int i = 0; i < qtdObstaculos; i++) {
        obstaculos[i].simbolo = "🌲";
        obstaculos[i].ativa = 1;
        do {
            obstaculos[i].x = rand() % ALTURA;
            obstaculos[i].y = rand() % LARGURA;
        } while(ocupado(obstaculos[i].x, obstaculos[i].y));
        tabuleiro[obstaculos[i].x][obstaculos[i].y] = obstaculos[i].simbolo;
    }
}

// =====================================
// ATUALIZA TABULEIRO
// =====================================
void atualizarTabuleiro() {
    limparTabuleiro();

    ovelhasVivas = 0;
    for(int i = 0; i < qtdOvelhas; i++) {
        if(ovelhas[i].ativa) {
            tabuleiro[ovelhas[i].x][ovelhas[i].y] = ovelhas[i].simbolo;
            ovelhasVivas++;
        }
    }
    
    lobosVivos = 0;
    for(int i = 0; i < qtdLobos; i++) {
        if(lobos[i].ativa) {
            tabuleiro[lobos[i].x][lobos[i].y] = lobos[i].simbolo;
            lobosVivos++;
        }
    }
    
    for(int i = 0; i < qtdCacadores; i++) {
        if(cacadores[i].ativa) {
            tabuleiro[cacadores[i].x][cacadores[i].y] = cacadores[i].simbolo;
        }
    }
    for(int i = 0; i < qtdObstaculos; i++) {
        tabuleiro[obstaculos[i].x][obstaculos[i].y] = obstaculos[i].simbolo;
    }
}

// =====================================
// DESENHAR RETÂNGULO
// =====================================
void desenhar() {
    printf("\n+");
    for(int i = 0; i < LARGURA * 2 + 1; i++) printf("-");
    printf("+\n");

    for(int i = 0; i < ALTURA; i++) {
        printf("| ");
        for(int j = 0; j < LARGURA; j++) {
            printf("%s", tabuleiro[i][j]); 
        }
        printf("|\n");
    }

    printf("+");
    for(int i = 0; i < LARGURA * 2 + 1; i++) printf("-");
    printf("+\n\n");

    printf("Ovelhas 🐑: %d/%d | Lobos 🐺: %d/%d | Cacadores 🤠: %d | Arvores 🌲: %d\n", 
            ovelhasVivas, qtdOvelhas, lobosVivos, qtdLobos, qtdCacadores, qtdObstaculos);
}

// =====================================
// FUNÇÕES DE NATALIDADE (NOVAS)
// Expanção de vetores via realloc
// =====================================
void nascerOvelha() {
    // Checa se ainda há alguma vaga física no mapa
    int vagasLivres = 0;
    for(int i = 0; i < ALTURA; i++) {
        for(int j = 0; j < LARGURA; j++) {
            if(!ocupado(i, j)) vagasLivres++;
        }
    }
    if (vagasLivres == 0) return; // Tabuleiro lotado

    qtdOvelhas++;
    ovelhas = realloc(ovelhas, qtdOvelhas * sizeof(Personagem));
    totalMoveis++; // Aumenta o range do sorteio do main

    int novoIndex = qtdOvelhas - 1;
    ovelhas[novoIndex].simbolo = "🐑";
    ovelhas[novoIndex].ativa = 1;

    do {
        ovelhas[novoIndex].x = rand() % ALTURA;
        ovelhas[novoIndex].y = rand() % LARGURA;
    } while (ocupado(ovelhas[novoIndex].x, ovelhas[novoIndex].y));

    tabuleiro[ovelhas[novoIndex].x][ovelhas[novoIndex].y] = "🐑";
    printf(" -> Um filhotinho de ovelha 🐑 nasceu na floresta!\n");
}

void nascerLobo() {
    int vagasLivres = 0;
    for(int i = 0; i < ALTURA; i++) {
        for(int j = 0; j < LARGURA; j++) {
            if(!ocupado(i, j)) vagasLivres++;
        }
    }
    if (vagasLivres == 0) return;

    qtdLobos++;
    lobos = realloc(lobos, qtdLobos * sizeof(Personagem));
    totalMoveis++;

    int novoIndex = qtdLobos - 1;
    lobos[novoIndex].simbolo = "🐺";
    lobos[novoIndex].ativa = 1;

    do {
        lobos[novoIndex].x = rand() % ALTURA;
        lobos[novoIndex].y = rand() % LARGURA;
    } while (ocupado(lobos[novoIndex].x, lobos[novoIndex].y));

    tabuleiro[lobos[novoIndex].x][lobos[novoIndex].y] = "🐺";
    printf(" -> Uma nova cria de lobo 🐺 nasceu na alcateia!\n");
}

// =====================================
// REMOVE PERSONAGENS MORTOS
// =====================================
void matarOvelhaNaPosicao(int x, int y) {
    for(int i = 0; i < qtdOvelhas; i++) {
        if(ovelhas[i].ativa && ovelhas[i].x == x && ovelhas[i].y == y) {
            ovelhas[i].ativa = 0;
            ovelhas[i].x = -1;
            ovelhas[i].y = -1;
            printf(" -> Uma ovelha 🐑 foi devorada! ☠️\n");
            return;
        }
    }
}

void matarLoboNaPosicao(int x, int y) {
    for(int i = 0; i < qtdLobos; i++) {
        if(lobos[i].ativa && lobos[i].x == x && lobos[i].y == y) {
            lobos[i].ativa = 0;
            lobos[i].x = -1;
            lobos[i].y = -1;
            printf(" -> Um lobo 🐺 foi abatido pelo cacador! 🎯\n");
            return;
        }
    }
}

// =====================================
// MOVIMENTO RETANGULAR COM REPRODUÇÃO
// =====================================
void mover(Personagem *p) {
    if(!p->ativa) return;

    int direcao = rand() % 4;
    int novoX = p->x;
    int novoY = p->y;

    switch(direcao) {
        case 0: novoX--; break;
        case 1: novoX++; break;
        case 2: novoY--; break;
        case 3: novoY++; break;
    }

    if(novoX >= 0 && novoX < ALTURA && novoY >= 0 && novoY < LARGURA) {
        char* destino = tabuleiro[novoX][novoY];

        // --- REGRAS DA OVELHA ---
        if(p->simbolo == "🐑") {
            if(destino == "🐺") {
                tabuleiro[p->x][p->y] = "  "; 
                p->ativa = 0; p->x = -1; p->y = -1;
                printf(" -> A ovelha 🐑 andou direto para um lobo! ☠️\n");
                return;
            }
            if(destino == "🐑") {
                // Encontro de ovelhas gera reprodução (Elas voltam para trás/não andam sobre a outra)
                nascerOvelha();
                return;
            }
        }

        // --- REGRAS DO LOBO ---
        if(p->simbolo == "🐺") {
            if(destino == "🐑") {
                tabuleiro[p->x][p->y] = "  ";
                matarOvelhaNaPosicao(novoX, novoY);
                p->x = novoX; p->y = novoY;
                tabuleiro[p->x][p->y] = p->simbolo;
                return;
            }
            if(destino == "🤠") {
                tabuleiro[p->x][p->y] = "  ";
                p->ativa = 0; p->x = -1; p->y = -1;
                printf(" -> O lobo 🐺 tentou atacar o cacador e morreu! 🎯\n");
                return;
            }
            if(destino == "🐺") {
                // Encontro de lobos gera reprodução
                nascerLobo();
                return;
            }
        }

        // --- REGRAS DO CAÇADOR ---
        if(p->simbolo == "🤠" && destino == "🐺") {
            tabuleiro[p->x][p->y] = "  ";
            matarLoboNaPosicao(novoX, novoY);
            p->x = novoX; p->y = novoY;
            tabuleiro[p->x][p->y] = p->simbolo;
            return;
        }

        // --- MOVIMENTAÇÃO PADRÃO ---
        tabuleiro[p->x][p->y] = "  ";
        if(!ocupado(novoX, novoY)) {
            p->x = novoX;
            p->y = novoY;
        }
        tabuleiro[p->x][p->y] = p->simbolo;
    }
}

// =====================================
// MAIN
// =====================================
int main() {
    #ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8);
    #endif

    setbuf(stdout, NULL);
    srand(time(NULL));

    printf("Digite a altura do tabuleiro (5-100): ");
    if (scanf("%d", &ALTURA) != 1) {
        ALTURA = 10;
    }

    if(ALTURA < 5)   ALTURA = 5;
    if(ALTURA > 100) ALTURA = 100;

    LARGURA = (int)(ALTURA * 1.5);

    qtdOvelhas    = calc(perc_ovelha, 1, 1000);
    qtdLobos      = calc(perc_lobo, 1, 500);
    qtdCacadores  = calc(perc_cacador, 1, 500);
    qtdObstaculos = calc(perc_obstaculo, 1, 1000);

    criarTabuleiro();

    ovelhas    = malloc(qtdOvelhas * sizeof(Personagem));
    lobos      = malloc(qtdLobos * sizeof(Personagem));
    cacadores  = malloc(qtdCacadores * sizeof(Personagem));
    obstaculos = malloc(qtdObstaculos * sizeof(Personagem));

    posicionar();

    // Define o valor inicial da população total que se mexe
    totalMoveis = qtdOvelhas + qtdLobos + qtdCacadores;

    // LOOP PRINCIPAL
    for(int turno = 1; turno <= TURNOS; turno++) {
        
        limparTela();

        atualizarTabuleiro();

        printf("\n=========== TURNO %d ===========\n", turno);
        desenhar();

        int sorteado = rand() % totalMoveis;

        if (sorteado < qtdOvelhas) {
            if(ovelhas[sorteado].ativa) {
                printf("Movimento: OVELHA 🐑 [%d] se mexeu", sorteado);
                mover(&ovelhas[sorteado]);
            } else {
                printf("Movimento: Uma ovelha morta 🐑 [%d] foi sorteada (passou o turno).\n", sorteado);
            }
        } 
        else if (sorteado < (qtdOvelhas + qtdLobos)) {
            int indiceLobo = sorteado - qtdOvelhas;
            if(lobos[indiceLobo].ativa) {
                printf("Movimento: LOBO 🐺 [%d] se mexeu", indiceLobo);
                mover(&lobos[indiceLobo]);
                printf("\n");
            } else {
                printf("Movimento: Um lobo morto 🐺 [%d] foi sorteado (passou o turno).\n", indiceLobo);
            }
        } 
        else {
            int indiceCacador = sorteado - (qtdOvelhas + qtdLobos);
            printf("Movimento: CACADOR 🤠 [%d] se mexeu.\n", indiceCacador);
            mover(&cacadores[indiceCacador]);
        }

        pausar();
    }

    printf("\nSIMULACAO FINALIZADA!\n");

    liberarMemoria();
    return 0;
}