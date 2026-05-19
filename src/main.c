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
int TAM;

float perc_ovelha    = 0.05;
float perc_lobo      = 0.03;
float perc_cacador   = 0.02;
float perc_obstaculo = 0.10;

#define TURNOS 50

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
// POPULAÇÕES
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
// CRIA TABULEIRO
// =====================================
void criarTabuleiro() {
    tabuleiro = malloc(TAM * sizeof(char**));
    for(int i = 0; i < TAM; i++) {
        tabuleiro[i] = malloc(TAM * sizeof(char*));
    }
}

// =====================================
// LIBERA MEMÓRIA
// =====================================
void liberarMemoria() {
    for(int i = 0; i < TAM; i++) {
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
    for(int i = 0; i < TAM; i++) {
        for(int j = 0; j < TAM; j++) {
            tabuleiro[i][j] = ". "; 
        }
    }
}

// =====================================
// VERIFICA OCUPAÇÃO
// =====================================
int ocupado(int x, int y) {
    if (tabuleiro[x][y] != ". ") {
        return 1;
    }
    return 0;
}

// =====================================
// CALCULA POPULAÇÃO
// =====================================
int calc(float perc, int min, int max) {
    int qtd = (int)(TAM * TAM * perc);
    if(qtd < min)  qtd = min;
    if(qtd > max)  qtd = max;
    return qtd;
}

// =====================================
// POSICIONAMENTO
// =====================================
void posicionar() {
    limparTabuleiro();

    // OVELHAS
    for(int i = 0; i < qtdOvelhas; i++) {
        ovelhas[i].simbolo = "🐑";
        ovelhas[i].ativa = 1;
        do {
            ovelhas[i].x = rand() % TAM;
            ovelhas[i].y = rand() % TAM;
        } while(ocupado(ovelhas[i].x, ovelhas[i].y));
        tabuleiro[ovelhas[i].x][ovelhas[i].y] = ovelhas[i].simbolo;
    }

    // LOBOS
    for(int i = 0; i < qtdLobos; i++) {
        lobos[i].simbolo = "🐺";
        lobos[i].ativa = 1;
        do {
            lobos[i].x = rand() % TAM;
            lobos[i].y = rand() % TAM;
        } while(ocupado(lobos[i].x, lobos[i].y));
        tabuleiro[lobos[i].x][lobos[i].y] = lobos[i].simbolo;
    }

    // CACADORES
    for(int i = 0; i < qtdCacadores; i++) {
        cacadores[i].simbolo = "🤠";
        cacadores[i].ativa = 1;
        do {
            cacadores[i].x = rand() % TAM;
            cacadores[i].y = rand() % TAM;
        } while(ocupado(cacadores[i].x, cacadores[i].y));
        tabuleiro[cacadores[i].x][cacadores[i].y] = cacadores[i].simbolo;
    }

    // OBSTÁCULOS (Árvores 🌲)
    for(int i = 0; i < qtdObstaculos; i++) {
        obstaculos[i].simbolo = "🌲";
        obstaculos[i].ativa = 1;
        do {
            obstaculos[i].x = rand() % TAM;
            obstaculos[i].y = rand() % TAM;
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
// DESENHAR
// =====================================
void desenhar() {
    printf("\n+");
    for(int i = 0; i < TAM * 2 + 1; i++) printf("-");
    printf("+\n");

    for(int i = 0; i < TAM; i++) {
        printf("| ");
        for(int j = 0; j < TAM; j++) {
            printf("%s", tabuleiro[i][j]); 
        }
        printf("|\n");
    }

    printf("+");
    for(int i = 0; i < TAM * 2 + 1; i++) printf("-");
    printf("+\n\n");

    printf("Ovelhas 🐑: %d/%d | Lobos 🐺: %d/%d | Cacadores 🤠: %d | Arvores 🌲: %d\n", 
            ovelhasVivas, qtdOvelhas, lobosVivos, qtdLobos, qtdCacadores, qtdObstaculos);
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
// MOVIMENTO
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

    if(novoX >= 0 && novoX < TAM && novoY >= 0 && novoY < TAM) {
        char* destino = tabuleiro[novoX][novoY];

        // --- REGRAS DA OVELHA ---
        if(p->simbolo == "🐑" && destino == "🐺") {
            tabuleiro[p->x][p->y] = ". "; 
            p->ativa = 0; p->x = -1; p->y = -1;
            printf(" -> A ovelha 🐑 andou direto para um lobo! ☠️\n");
            return;
        }

        // --- REGRAS DO LOBO ---
        if(p->simbolo == "🐺") {
            if(destino == "🐑") {
                tabuleiro[p->x][p->y] = ". ";
                matarOvelhaNaPosicao(novoX, novoY);
                p->x = novoX; p->y = novoY;
                tabuleiro[p->x][p->y] = p->simbolo;
                return;
            }
            if(destino == "🤠") {
                tabuleiro[p->x][p->y] = ". ";
                p->ativa = 0; p->x = -1; p->y = -1;
                printf(" -> O lobo 🐺 tentou atacar o cacador e morreu! 🎯\n");
                return;
            }
        }

        // --- REGRAS DO CAÇADOR ---
        if(p->simbolo == "🤠" && destino == "🐺") {
            tabuleiro[p->x][p->y] = ". ";
            matarLoboNaPosicao(novoX, novoY);
            p->x = novoX; p->y = novoY;
            tabuleiro[p->x][p->y] = p->simbolo;
            return;
        }

        // --- MOVIMENTAÇÃO PADRÃO ---
        tabuleiro[p->x][p->y] = ". ";
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

    printf("Digite o tamanho do tabuleiro (5-100): ");
    if (scanf("%d", &TAM) != 1) {
        TAM = 10;
    }

    if(TAM < 5)   TAM = 5;
    if(TAM > 100) TAM = 100;

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

    int totalMoveis = qtdOvelhas + qtdLobos + qtdCacadores;

    // LOOP PRINCIPAL
    for(int turno = 1; turno <= TURNOS; turno++) {
        
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