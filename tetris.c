#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <ncurses.h>

#define LINHAS 20
#define COLUNAS 15
#define VERDADEIRO 1
#define FALSO 0

char Tabela[LINHAS][COLUNAS] = {0};
int pontuacao = 0;
char JogoAtivo = VERDADEIRO;
suseconds_t temporizador = 400000; // diminua isso para acelerar o jogo
int diminuir = 1000;

typedef struct {
    char **matriz;
    int largura, linha, coluna;
} Forma;

Forma atual;

const Forma FormasArray[7] = {
    {(char *[]){(char []){0, 1, 1}, (char []){1, 1, 0}, (char []){0, 0, 0}}, 3},                           
    {(char *[]){(char []){1, 1, 0}, (char []){0, 1, 1}, (char []){0, 0, 0}}, 3},                           
    {(char *[]){(char []){0, 1, 0}, (char []){1, 1, 1}, (char []){0, 0, 0}}, 3},                           
    {(char *[]){(char []){0, 0, 1}, (char []){1, 1, 1}, (char []){0, 0, 0}}, 3},                           
    {(char *[]){(char []){1, 0, 0}, (char []){1, 1, 1}, (char []){0, 0, 0}}, 3},                           
    {(char *[]){(char []){1, 1}, (char []){1, 1}}, 2},                                                   
    {(char *[]){(char []){0, 0, 0, 0}, (char []){1, 1, 1, 1}, (char []){0, 0, 0, 0}, (char []){0, 0, 0, 0}}, 4} 
};

Forma CopiarForma(Forma forma) {
    Forma nova_forma = forma;
    char **copia_forma = forma.matriz;
    nova_forma.matriz = (char **)malloc(nova_forma.largura * sizeof(char *));
    int i, j;
    for (i = 0; i < nova_forma.largura; i++) {
        nova_forma.matriz[i] = (char *)malloc(nova_forma.largura * sizeof(char));
        for (j = 0; j < nova_forma.largura; j++) {
            nova_forma.matriz[i][j] = copia_forma[i][j];
        }
    }
    return nova_forma;
}

void DeletarForma(Forma forma) {
    int i;
    for (i = 0; i < forma.largura; i++) {
        free(forma.matriz[i]);
    }
    free(forma.matriz);
}

int VerificarPosicao(Forma forma) {
    char **matriz = forma.matriz;
    int i, j;
    for (i = 0; i < forma.largura; i++) {
        for (j = 0; j < forma.largura; j++) {
            if ((forma.coluna + j < 0 || forma.coluna + j >= COLUNAS || forma.linha + i >= LINHAS)) { 
                if (matriz[i][j]) 
                    return FALSO;
            } else if (Tabela[forma.linha + i][forma.coluna + j] && matriz[i][j])
                return FALSO;
        }
    }
    return VERDADEIRO;
}

void DefinirNovaFormaAleatoria() {
    Forma nova_forma = CopiarForma(FormasArray[rand() % 7]);

    nova_forma.coluna = rand() % (COLUNAS - nova_forma.largura + 1);
    nova_forma.linha = 0;
    DeletarForma(atual);
    atual = nova_forma;
    if (!VerificarPosicao(atual)) {
        JogoAtivo = FALSO;
    }
}

void RotacionarForma(Forma forma) {
    Forma temp = CopiarForma(forma);
    int i, j, k, largura;
    largura = forma.largura;
    for (i = 0; i < largura; i++) {
        for (j = 0, k = largura - 1; j < largura; j++, k--) {
            forma.matriz[i][j] = temp.matriz[k][i];
        }
    }
    DeletarForma(temp);
}

void EscreverNaTabela() {
    int i, j;
    for (i = 0; i < atual.largura; i++) {
        for (j = 0; j < atual.largura; j++) {
            if (atual.matriz[i][j])
                Tabela[atual.linha + i][atual.coluna + j] = atual.matriz[i][j];
        }
    }
}

void RemoverLinhasCompletasEAtualizarPontuacao() {
    int i, j, soma, contador = 0;
    for (i = 0; i < LINHAS; i++) {
        soma = 0;
        for (j = 0; j < COLUNAS; j++) {
            soma += Tabela[i][j];
        }
        if (soma == COLUNAS) {
            contador++;
            int l, k;
            for (k = i; k >= 1; k--)
                for (l = 0; l < COLUNAS; l++)
                    Tabela[k][l] = Tabela[k - 1][l];
            for (l = 0; l < COLUNAS; l++)
                Tabela[k][l] = 0;
            temporizador -= diminuir--;
        }
    }
    pontuacao += 100 * contador;
}

void ImprimirTabela() {
    char Buffer[LINHAS][COLUNAS] = {0};
    int i, j;
    for (i = 0; i < atual.largura; i++) {
        for (j = 0; j < atual.largura; j++) {
            if (atual.matriz[i][j])
                Buffer[atual.linha + i][atual.coluna + j] = atual.matriz[i][j];
        }
    }
    clear();
    for (i = 0; i < COLUNAS - 9; i++)
        printw(" ");
    printw("PIF Tetris\n");
    for (i = 0; i < LINHAS; i++) {
        for (j = 0; j < COLUNAS; j++) {
            printw("%c ", (Tabela[i][j] + Buffer[i][j]) ? '#' : '.');
        }
        printw("\n");
    }
    printw("\nPontuação: %d\n", pontuacao);
}

void ManipularAtual(int acao) {
    Forma temp = CopiarForma(atual);
    switch (acao) {
    case 's':
        temp.linha++;
        if (VerificarPosicao(temp))
            atual.linha++;
        else {
            EscreverNaTabela();
            RemoverLinhasCompletasEAtualizarPontuacao();
            DefinirNovaFormaAleatoria();
        }
        break;
    case 'd':
        temp.coluna++;
        if (VerificarPosicao(temp))
            atual.coluna++;
        break;
    case 'a':
        temp.coluna--;
        if (VerificarPosicao(temp))
            atual.coluna--;
        break;
    case 'w':
        RotacionarForma(temp);
        if (VerificarPosicao(temp))
            RotacionarForma(atual);
        break;
    }
    DeletarForma(temp);
    ImprimirTabela();
}

struct timeval antes_agora, agora;
int precisaAtualizar() {
    return ((suseconds_t)(agora.tv_sec * 1000000 + agora.tv_usec) - ((suseconds_t)antes_agora.tv_sec * 1000000 + antes_agora.tv_usec)) > temporizador;
}

int main() {
    srand(time(0));
    pontuacao = 0;
    int c;
    initscr();
    gettimeofday(&antes_agora, NULL);
    timeout(1);
    DefinirNovaFormaAleatoria();
    ImprimirTabela();
    while (JogoAtivo) {
        if ((c = getch()) != ERR) {
            if (c == 'p' || c == 'P') {
                timeout(-1);
                printw("Jogo Pausado. Pressione 'p' para continuar.");
                do {
                    c = getch();
                } while (c != 'p' && c != 'P');
                timeout(1);
            } else if (c == 'q' || c == 'Q') {
                break;
            } else {
                ManipularAtual(c);
            }
        }
        gettimeofday(&agora, NULL);
        if (precisaAtualizar()) {
            ManipularAtual('s');
            gettimeofday(&antes_agora, NULL);
        }
    }
    DeletarForma(atual);
    endwin();
    int i, j;
    for (i = 0; i < LINHAS; i++) {
        for (j = 0; j < COLUNAS; j++) {
            printf("%c ", Tabela[i][j] ? '#' : '.');
        }
        printf("\n");
    }
    printf("\nJogo encerrado!\n");
    printf("\nPontuação: %d\n", pontuacao);
    return 0;
}
