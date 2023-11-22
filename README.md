## Tetris PIF

**README**

Este é um simples jogo Tetris implementado em C utilizando a biblioteca Ncurses para a interface gráfica no terminal. O código foi desenvolvido como parte de um projeto acadêmico da disciplina Programação Imperativa Funcional na Cesar School.

### Instruções de Compilação

Certifique-se de ter o Ncurses instalado em seu sistema antes de compilar o código. Se estiver utilizando um sistema baseado em Debian/Ubuntu, você pode instalar o Ncurses com o seguinte comando:

```bash
sudo apt-get install libncurses5-dev libncursesw5-dev
```

Após a instalação do Ncurses, você pode compilar o código com o seguinte comando:

### Instruções de Compilação com Makefile


 **Compilação com Makefile:**
   - Abra um terminal no diretório que contém o arquivo `tetris.c` e o `Makefile`.
   - Execute o seguinte comando para compilar o código:

     ```bash
     make
     ```

   Isso utilizará as regras definidas no Makefile para compilar o código-fonte Tetris e gerar um executável chamado `tetris`.

2. **Execução:**
   - Após a compilação bem-sucedida, execute o programa com:

     ```bash
     ./tetris
     ```

   - Pressione qualquer tecla para iniciar o jogo.

Certifique-se de ter o Ncurses instalado em seu sistema antes de compilar, conforme mencionado nas instruções anteriores.

### Instruções de Uso

1. **Execução:**
   - Após compilar o código, execute o programa com `./tetris`.
   - Pressione qualquer tecla para iniciar o jogo.

2. **Controles:**
   - **Movimento para Baixo (Soft Drop):** Pressione a tecla 's'.
   - **Movimento para a Direita:** Pressione a tecla 'd'.
   - **Movimento para a Esquerda:** Pressione a tecla 'a'.
   - **Rotacionar a Peça:** Pressione a tecla 'w'.
   - **Pausar o Jogo:** Pressione a tecla 'p'.
   - **Sair do Jogo:** Pressione a tecla 'l'.

3. **Objetivo:**
   - O objetivo do jogo é organizar as peças que caem para criar linhas completas.
   - Cada linha completa removida adiciona pontos à sua pontuação.
   - O jogo continua até que não seja possível posicionar uma nova peça.

4. **Encerramento:**
   - Para encerrar o jogo a qualquer momento, pressione 'l'.
