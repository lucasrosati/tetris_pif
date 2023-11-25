#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <ncurses.h>

#define LINHAS 21
#define COLUNAS 10
#define VERDADEIRO 1
#define FALSO 0

char Tabela[LINHAS][COLUNAS] = {0};
int pontuacao = 0;
int contador = 0;
int nivel = 0;
char JogoAtivo = FALSO;
suseconds_t temporizador = 400000; // diminua isso para acelerar o jogo
int diminuir = 7000;

typedef struct Forma{
    char **matriz;
    int largura, linha, coluna;
} Forma;

Forma atual;
Forma proxima;

const Forma Tetrominos[7] = {
    {(char *[]){
                (char []){0, 1, 1}, 
                (char []){1, 1, 0}, 
                (char []){0, 0, 0}
                }, 3}, // tetromino S

    {(char *[]){
                (char []){1, 1, 0}, 
                (char []){0, 1, 1}, 
                (char []){0, 0, 0}
                }, 3}, // tetromino Z

    {(char *[]){
                (char []){0, 1, 0}, 
                (char []){1, 1, 1}, 
                (char []){0, 0, 0}
                }, 3}, // tetromino T

    {(char *[]){
                (char []){0, 0, 1}, 
                (char []){1, 1, 1}, 
                (char []){0, 0, 0}
                }, 3}, // tetromino L

    {(char *[]){
                (char []){1, 0, 0}, 
                (char []){1, 1, 1}, 
                (char []){0, 0, 0}
                }, 3}, // tetromino J

    {(char *[]){
                (char []){1, 1}, 
                (char []){1, 1}
                }, 2}, // tetromino O

    {(char *[]){
                (char []){0, 0, 0, 0},
                (char []){1, 1, 1, 1}, 
                (char []){0, 0, 0, 0}, 
                (char []){0, 0, 0, 0}
        }, 4} // tetromino I
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

void DefinirProximaForma(){
    proxima = CopiarForma(Tetrominos[rand() % 7]);
}

void DefinirFormaAleatoria() {
    Forma nova_forma = CopiarForma(proxima);

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
        for (j = 0, k = largura - 1; j < largura; j++, k--){
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
    int i, j, soma, sequencia = 0;
    for (i = 0; i < LINHAS; i++) {
        soma = 0;
        for (j = 0; j < COLUNAS; j++) {
            soma += Tabela[i][j];
        }
        if (soma == COLUNAS) {
            sequencia++;
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
    if((contador % 10 > 9))
        nivel++;

    switch (sequencia)
    {
    case 1:
        pontuacao += 40 * (nivel + 1);
        break;
    case 2:
        pontuacao += 100 * (nivel + 1);
        break;
    case 3:
        pontuacao += 300 * (nivel + 1);
        break;
    case 4:
        pontuacao += 1200 * (nivel + 1);
        break;
    }

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
    for (i = 0; i < COLUNAS - 5; i++)
        printw(" ");
    printw("PIF Tetris\n");
    printw("                   Nível: %i\n",nivel + 1);
    printw("                   Seguinte:\n");
    for(i = 0; i < 2; i++){
        printw("                            ");
        for(j = 0; j < proxima.largura; j++){
            if(proxima.matriz[i][j]){
                printw("# ");
            }else{
                printw(". ");
            }
        }
        printw("\n");
    }
    printw("\n");
    for (i = 0; i < LINHAS; i++) {
        for (j = 0; j < COLUNAS; j++) {
            if (Tabela[i][j] + Buffer[i][j]){
                printw("# ");
            }else{
                printw(". ");
            }
        }
        printw("\n");
    }
    printw("\nPontuação: %d\n", pontuacao);
    printw("\nPressione 'p' para pausar ou 'l' para sair do jogo.\n");
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
            DefinirFormaAleatoria();
            DefinirProximaForma();
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

void ImprimirTelaInicial(){
    int c;
printw ("                           TETRIS\n\n\n");
printw("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
printw("@#                  #@#                  #@#                  #@\n");
printw("@#                  #@#                  #@#                  #@\n");
printw("@#                  #@#                  #@#                  #@\n");
printw("@#                  #@#                  #@#                  #@\n");
printw("@#                  #@#                  #@#                  #@\n");
printw("@#                  #@#                  #@#                  #@\n");
printw("@#                  #@#                  #@#                  #@\n");
printw("@#                  #@#                  #@#                  #@\n");
printw("@#                  #@#                  #@#                  #@\n");
printw("@#                  #@#                  #@#                  #@\n");
printw("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
printw("                     @#                  #@\n");                   
printw("                     @#                  #@\n");                
printw("                     @#                  #@\n");                 
printw("                     @#                  #@\n");
printw("                     @#                  #@\n");
printw("                     @#                  #@\n");
printw("                     @#                  #@\n");
printw("                     @#                  #@\n");
printw("                     @#                  #@\n");
printw("                     @#                  #@\n");                  
printw("                     @@@@@@@@@@@@@@@@@@@@@@\n");                 
                    
printw("\n\nPressione qualquer tecla para jogar");
    if ((c = getch()) != ERR)
        JogoAtivo = VERDADEIRO;
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
    ImprimirTelaInicial();
    gettimeofday(&antes_agora, NULL);
    timeout(1);
    DefinirProximaForma();
    DefinirFormaAleatoria();
    ImprimirTabela();
    while (JogoAtivo) {
        if ((c = getch()) != ERR) {
            if (c == 'p' || c == 'P') {
                timeout(-1);
                printw("Jogo Pausado. Pressione 'p' para continuar.");
                do {
                    c = getch();
                } while (c != 'p' && c != 'P' && c == 'l' && c == 'L');
                if (c == 'l' || c == 'L')
                    break;
                timeout(1);
            } else if (c == 'l' || c == 'L') {
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
            if (Tabela[i][j]){
                printf("# ");
            }else{
                printf(". ");
            }
        }
        printf("\n");
    }
    printf("\nJogo encerrado!\n");
    printf("\nPontuação: %d\n", pontuacao);
    return 0;
}
