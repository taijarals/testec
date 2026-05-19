#include <stdio.h>

#define ESQUERDA 0
#define DIREITA 1

typedef struct {
    int lado;
} Personagem;

Personagem lobo;
Personagem ovelha;
Personagem cacador;

void limparTela() {
    for(int i = 0; i < 30; i++) {
        printf("\n");
    }
}

void desenharJogo() {

    printf("========================================\n");
    printf("        LOBO, OVELHA E CACADOR\n");
    printf("========================================\n\n");

    printf("ESQUERDA                     DIREITA\n");
    printf("----------------------------------------\n");

    printf("Lobo:     %s\n",
           lobo.lado == ESQUERDA ? "ESQUERDA" : "DIREITA");

    printf("Ovelha:   %s\n",
           ovelha.lado == ESQUERDA ? "ESQUERDA" : "DIREITA");

    printf("Cacador:  %s\n",
           cacador.lado == ESQUERDA ? "ESQUERDA" : "DIREITA");

    printf("\n");
}

int verificarDerrota() {

    if(lobo.lado == ovelha.lado &&
       cacador.lado != lobo.lado) {
        return 1;
    }

    return 0;
}

int verificarVitoria() {

    if(lobo.lado == DIREITA &&
       ovelha.lado == DIREITA &&
       cacador.lado == DIREITA) {
        return 1;
    }

    return 0;
}

void mover(int opcao) {

    switch(opcao) {

        case 1:

            if(cacador.lado == lobo.lado) {
                lobo.lado = !lobo.lado;
                cacador.lado = !cacador.lado;
            }

            break;

        case 2:

            if(cacador.lado == ovelha.lado) {
                ovelha.lado = !ovelha.lado;
                cacador.lado = !cacador.lado;
            }

            break;

        case 3:

            cacador.lado = !cacador.lado;

            break;

        default:
            printf("Opcao invalida!\n");
    }
}

int main() {

    int opcao;

    lobo.lado = ESQUERDA;
    ovelha.lado = ESQUERDA;
    cacador.lado = ESQUERDA;

    while(1) {

        limparTela();

        desenharJogo();

        if(verificarDerrota()) {
            printf("GAME OVER!\n");
            printf("O lobo ficou sozinho com a ovelha.\n");
            break;
        }

        if(verificarVitoria()) {
            printf("PARABENS! VOCE VENCEU!\n");
            break;
        }

        printf("Escolha uma acao:\n");
        printf("1 - Levar lobo\n");
        printf("2 - Levar ovelha\n");
        printf("3 - Ir sozinho\n");

        printf("Opcao: ");
        scanf("%d", &opcao);

        mover(opcao);
    }

    return 0;
}