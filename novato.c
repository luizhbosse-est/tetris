/*
 * TETRIS STACK - SIMULADOR DE FILA DE PEÇAS
 * 
 * Programa desenvolvido em C que simula a fila de peças futuras do jogo Tetris Stack.
 * Implementa uma fila circular com operações de inserção (enqueue) e remoção (dequeue).
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// ============================================================================
// DEFINIÇÕES E ESTRUTURAS
// ============================================================================

/**
 * Estrutura que representa uma peça do Tetris
 */
typedef struct {
    char nome;  // Tipo da peça ('I', 'O', 'T', 'L')
    int id;     // Identificador único da peça
} Peca;

/**
 * Estrutura que representa a fila circular de peças
 */
typedef struct {
    Peca pecas[5];  // Array de peças com tamanho fixo de 5
    int frente;     // Índice da frente da fila
    int tras;       // Índice do final da fila
    int tamanho;    // Número atual de elementos na fila
} FilaPecas;

// Variável global para controlar o ID das peças
int proximoId = 0;

// ============================================================================
// DECLARAÇÕES DAS FUNÇÕES
// ============================================================================

void inicializarFila(FilaPecas* fila);
Peca gerarPeca();
int filaCheia(FilaPecas* fila);
int filaVazia(FilaPecas* fila);
int enqueue(FilaPecas* fila, Peca peca);
int dequeue(FilaPecas* fila, Peca* peca);
void exibirFila(FilaPecas* fila);
void exibirMenu();
int obterOpcao();

// ============================================================================
// IMPLEMENTAÇÃO DAS FUNÇÕES DA FILA
// ============================================================================

/**
 * Inicializa a fila de peças, preenchendo-a com 5 peças geradas automaticamente
 * @param fila Ponteiro para a estrutura da fila
 */
void inicializarFila(FilaPecas* fila) {
    fila->frente = 0;
    fila->tras = 0;
    fila->tamanho = 0;
    
    // Preenche a fila com 5 peças iniciais
    for (int i = 0; i < 5; i++) {
        Peca novaPeca = gerarPeca();
        enqueue(fila, novaPeca);
    }
}

/**
 * Gera uma nova peça com tipo aleatório e ID único
 * @return Nova peça gerada
 */
Peca gerarPeca() {
    Peca novaPeca;
    char tipos[] = {'I', 'O', 'T', 'L'};
    
    // Seleciona um tipo aleatório
    novaPeca.nome = tipos[rand() % 4];
    
    // Atribui ID único e incrementa para a próxima peça
    novaPeca.id = proximoId++;
    
    return novaPeca;
}

/**
 * Verifica se a fila está cheia
 * @param fila Ponteiro para a estrutura da fila
 * @return 1 se cheia, 0 caso contrário
 */
int filaCheia(FilaPecas* fila) {
    return fila->tamanho == 5;
}

/**
 * Verifica se a fila está vazia
 * @param fila Ponteiro para a estrutura da fila
 * @return 1 se vazia, 0 caso contrário
 */
int filaVazia(FilaPecas* fila) {
    return fila->tamanho == 0;
}

/**
 * Insere uma peça no final da fila (enqueue)
 * @param fila Ponteiro para a estrutura da fila
 * @param peca Peça a ser inserida
 * @return 1 se inserção bem-sucedida, 0 se fila cheia
 */
int enqueue(FilaPecas* fila, Peca peca) {
    if (filaCheia(fila)) {
        return 0; // Fila cheia, não é possível inserir
    }
    
    // Insere a peça na posição 'tras'
    fila->pecas[fila->tras] = peca;
    
    // Atualiza o índice 'tras' de forma circular
    fila->tras = (fila->tras + 1) % 5;
    
    // Incrementa o tamanho da fila
    fila->tamanho++;
    
    return 1; // Inserção bem-sucedida
}

/**
 * Remove uma peça da frente da fila (dequeue)
 * @param fila Ponteiro para a estrutura da fila
 * @param peca Ponteiro para armazenar a peça removida
 * @return 1 se remoção bem-sucedida, 0 se fila vazia
 */
int dequeue(FilaPecas* fila, Peca* peca) {
    if (filaVazia(fila)) {
        return 0; // Fila vazia, não é possível remover
    }
    
    // Copia a peça da frente para o ponteiro fornecido
    *peca = fila->pecas[fila->frente];
    
    // Atualiza o índice 'frente' de forma circular
    fila->frente = (fila->frente + 1) % 5;
    
    // Decrementa o tamanho da fila
    fila->tamanho--;
    
    return 1; // Remoção bem-sucedida
}

/**
 * Exibe o estado atual da fila de peças
 * @param fila Ponteiro para a estrutura da fila
 */
void exibirFila(FilaPecas* fila) {
    printf("\nFila de pecas\n");
    
    if (filaVazia(fila)) {
        printf("Fila vazia!\n");
        return;
    }
    
    // Percorre a fila de forma circular para exibir as peças
    int indice = fila->frente;
    for (int i = 0; i < fila->tamanho; i++) {
        printf("[%c %d] ", fila->pecas[indice].nome, fila->pecas[indice].id);
        indice = (indice + 1) % 5;
    }
    printf("\n");
}

/**
 * Exibe o menu de opções para o usuário
 */
void exibirMenu() {
    printf("\nOpcoes de acao:\n");
    printf("1 - Jogar peca (dequeue)\n");
    printf("2 - Inserir nova peca (enqueue)\n");
    printf("0 - Sair\n");
    printf("Escolha uma opcao: ");
}

/**
 * Obtém a opção escolhida pelo usuário
 * @return Opção escolhida (0, 1 ou 2)
 */
int obterOpcao() {
    int opcao;
    scanf("%d", &opcao);
    return opcao;
}

// ============================================================================
// PROGRAMA PRINCIPAL
// ============================================================================

/**
 * Função principal do programa Tetris Stack
 * Implementa o loop principal de interação com o usuário
 */
int main() {
    // Inicializa o gerador de números aleatórios
    srand(time(NULL));
    
    // Declara e inicializa a fila de peças
    FilaPecas fila;
    inicializarFila(&fila);
    
    // Variáveis para controle do loop e operações
    int opcao;
    Peca pecaRemovida;
    Peca novaPeca;
    
    printf("=== TETRIS STACK - FILA DE PECAS ===\n");
    printf("Bem-vindo ao simulador da fila de pecas do Tetris Stack!\n");
    
    // Loop principal do programa
    do {
        // Exibe o estado atual da fila
        exibirFila(&fila);
        
        // Exibe o menu e obtém a opção do usuário
        exibirMenu();
        opcao = obterOpcao();
        
        // Processa a opção escolhida
        switch (opcao) {
            case 1: // Jogar peça (dequeue)
                if (dequeue(&fila, &pecaRemovida)) {
                    printf("\nPeca jogada: [%c %d]\n", 
                           pecaRemovida.nome, pecaRemovida.id);
                } else {
                    printf("\nErro: Fila vazia! Nao e possivel jogar uma peca.\n");
                }
                break;
                
            case 2: // Inserir nova peça (enqueue)
                if (!filaCheia(&fila)) {
                    novaPeca = gerarPeca();
                    if (enqueue(&fila, novaPeca)) {
                        printf("\nNova peca inserida: [%c %d]\n", 
                               novaPeca.nome, novaPeca.id);
                    } else {
                        printf("\nErro: Nao foi possivel inserir a peca.\n");
                    }
                } else {
                    printf("\nErro: Fila cheia! Nao e possivel inserir nova peca.\n");
                    printf("Jogue uma peca primeiro para liberar espaco.\n");
                }
                break;
                
            case 0: // Sair
                printf("\nSaindo do programa...\n");
                printf("Obrigado por jogar Tetris Stack!\n");
                break;
                
            default: // Opção inválida
                printf("\nOpcao invalida! Por favor, escolha 0, 1 ou 2.\n");
                break;
        }
        
        // Pausa para melhor visualização (apenas em modo interativo)
        if (opcao != 0 && opcao >= 1 && opcao <= 2) {
            printf("\nPressione Enter para continuar...");
            while (getchar() != '\n'); // Limpa o buffer de entrada
        }
        
    } while (opcao != 0);
    
    return 0;
}