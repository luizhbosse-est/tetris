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
 * Estrutura que representa a fila circular de peças futuras
 */
typedef struct {
    Peca pecas[5];  // Array de peças com tamanho fixo de 5
    int frente;     // Índice da frente da fila
    int tras;       // Índice do final da fila
    int tamanho;    // Número atual de elementos na fila (sempre 5)
} FilaPecas;

/**
 * Estrutura que representa a pilha de peças reservadas
 */
typedef struct {
    Peca pecas[3];  // Array de peças com capacidade máxima de 3
    int topo;       // Índice do topo da pilha (-1 quando vazia)
} PilhaReserva;

// Variável global para controlar o ID das peças
int proximoId = 0;

// ============================================================================
// DECLARAÇÕES DAS FUNÇÕES
// ============================================================================

// Funções da fila
void inicializarFila(FilaPecas* fila);
int enqueueAutomatico(FilaPecas* fila);
int dequeueFila(FilaPecas* fila, Peca* peca);
void exibirFila(FilaPecas* fila);

// Funções da pilha
void inicializarPilha(PilhaReserva* pilha);
int pilhaCheia(PilhaReserva* pilha);
int pilhaVazia(PilhaReserva* pilha);
int pushPilha(PilhaReserva* pilha, Peca peca);
int popPilha(PilhaReserva* pilha, Peca* peca);
void exibirPilha(PilhaReserva* pilha);

// Funções de troca (NOVAS)
int trocarSimples(FilaPecas* fila, PilhaReserva* pilha);
int trocarMultipla(FilaPecas* fila, PilhaReserva* pilha);

// Funções auxiliares
Peca gerarPeca();
void exibirEstadoCompleto(FilaPecas* fila, PilhaReserva* pilha);
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
        enqueueAutomatico(fila);
    }
}

/**
 * Adiciona automaticamente uma nova peça ao final da fila
 * @param fila Ponteiro para a estrutura da fila
 * @return 1 se inserção bem-sucedida, 0 caso contrário
 */
int enqueueAutomatico(FilaPecas* fila) {
    if (fila->tamanho >= 5) {
        return 0; // Fila já está cheia
    }
    
    Peca novaPeca = gerarPeca();
    
    // Insere a peça na posição 'tras'
    fila->pecas[fila->tras] = novaPeca;
    
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
int dequeueFila(FilaPecas* fila, Peca* peca) {
    if (fila->tamanho == 0) {
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
    printf("Fila de pecas: ");
    
    if (fila->tamanho == 0) {
        printf("Fila vazia!");
    } else {
        // Percorre a fila de forma circular para exibir as peças
        int indice = fila->frente;
        for (int i = 0; i < fila->tamanho; i++) {
            printf("[%c %d] ", fila->pecas[indice].nome, fila->pecas[indice].id);
            indice = (indice + 1) % 5;
        }
    }
    printf("\n");
}

// ============================================================================
// IMPLEMENTAÇÃO DAS FUNÇÕES DA PILHA
// ============================================================================

/**
 * Inicializa a pilha de reserva (vazia)
 * @param pilha Ponteiro para a estrutura da pilha
 */
void inicializarPilha(PilhaReserva* pilha) {
    pilha->topo = -1; // Pilha vazia
}

/**
 * Verifica se a pilha está cheia
 * @param pilha Ponteiro para a estrutura da pilha
 * @return 1 se cheia, 0 caso contrário
 */
int pilhaCheia(PilhaReserva* pilha) {
    return pilha->topo == 2; // Índice máximo é 2 (capacidade 3)
}

/**
 * Verifica se a pilha está vazia
 * @param pilha Ponteiro para a estrutura da pilha
 * @return 1 se vazia, 0 caso contrário
 */
int pilhaVazia(PilhaReserva* pilha) {
    return pilha->topo == -1;
}

/**
 * Insere uma peça no topo da pilha (push)
 * @param pilha Ponteiro para a estrutura da pilha
 * @param peca Peça a ser inserida
 * @return 1 se inserção bem-sucedida, 0 se pilha cheia
 */
int pushPilha(PilhaReserva* pilha, Peca peca) {
    if (pilhaCheia(pilha)) {
        return 0; // Pilha cheia, não é possível inserir
    }
    
    // Incrementa o topo e insere a peça
    pilha->topo++;
    pilha->pecas[pilha->topo] = peca;
    
    return 1; // Inserção bem-sucedida
}

/**
 * Remove uma peça do topo da pilha (pop)
 * @param pilha Ponteiro para a estrutura da pilha
 * @param peca Ponteiro para armazenar a peça removida
 * @return 1 se remoção bem-sucedida, 0 se pilha vazia
 */
int popPilha(PilhaReserva* pilha, Peca* peca) {
    if (pilhaVazia(pilha)) {
        return 0; // Pilha vazia, não é possível remover
    }
    
    // Copia a peça do topo para o ponteiro fornecido
    *peca = pilha->pecas[pilha->topo];
    
    // Decrementa o topo
    pilha->topo--;
    
    return 1; // Remoção bem-sucedida
}

/**
 * Exibe o estado atual da pilha de reserva
 * @param pilha Ponteiro para a estrutura da pilha
 */
void exibirPilha(PilhaReserva* pilha) {
    printf("Pilha de reserva (Topo -> Base): ");
    
    if (pilhaVazia(pilha)) {
        printf("Vazia");
    } else {
        // Exibe da posição do topo até a base
        for (int i = pilha->topo; i >= 0; i--) {
            printf("[%c %d] ", pilha->pecas[i].nome, pilha->pecas[i].id);
        }
    }
    printf("\n");
}

// ============================================================================
// IMPLEMENTAÇÃO DAS FUNÇÕES DE TROCA (NOVAS)
// ============================================================================

/**
 * Realiza troca simples entre a frente da fila e o topo da pilha
 * @param fila Ponteiro para a estrutura da fila
 * @param pilha Ponteiro para a estrutura da pilha
 * @return 1 se troca bem-sucedida, 0 caso contrário
 */
int trocarSimples(FilaPecas* fila, PilhaReserva* pilha) {
    // Validações: ambas devem ter pelo menos 1 peça
    if (fila->tamanho == 0) {
        printf("\nErro: Fila vazia! Nao e possivel realizar a troca.\n");
        return 0;
    }
    
    if (pilhaVazia(pilha)) {
        printf("\nErro: Pilha vazia! Nao e possivel realizar a troca.\n");
        return 0;
    }
    
    // Salva as peças que serão trocadas
    Peca pecaFila = fila->pecas[fila->frente];
    Peca pecaPilha = pilha->pecas[pilha->topo];
    
    // Realiza a troca
    fila->pecas[fila->frente] = pecaPilha;
    pilha->pecas[pilha->topo] = pecaFila;
    
    printf("\nTroca simples realizada: [%c %d] da fila <-> [%c %d] da pilha\n",
           pecaFila.nome, pecaFila.id, pecaPilha.nome, pecaPilha.id);
    
    return 1; // Troca bem-sucedida
}

/**
 * Realiza troca múltipla entre os 3 primeiros da fila e toda a pilha
 * @param fila Ponteiro para a estrutura da fila
 * @param pilha Ponteiro para a estrutura da pilha
 * @return 1 se troca bem-sucedida, 0 caso contrário
 */
int trocarMultipla(FilaPecas* fila, PilhaReserva* pilha) {
    // Validações: fila deve ter 5 peças E pilha deve ter exatamente 3 peças
    if (fila->tamanho != 5) {
        printf("\nErro: Fila deve ter exatamente 5 pecas para troca multipla.\n");
        return 0;
    }
    
    if (pilha->topo != 2) { // topo == 2 significa 3 peças (índices 0, 1, 2)
        printf("\nErro: Pilha deve ter exatamente 3 pecas para troca multipla.\n");
        return 0;
    }
    
    // Arrays temporários para armazenar as peças durante a troca
    Peca tempFila[3];
    Peca tempPilha[3];
    
    // Salva os 3 primeiros da fila
    int indiceFila = fila->frente;
    for (int i = 0; i < 3; i++) {
        tempFila[i] = fila->pecas[indiceFila];
        indiceFila = (indiceFila + 1) % 5;
    }
    
    // Salva as 3 peças da pilha
    for (int i = 0; i < 3; i++) {
        tempPilha[i] = pilha->pecas[i];
    }
    
    // Coloca as peças da pilha nos 3 primeiros lugares da fila
    indiceFila = fila->frente;
    for (int i = 2; i >= 0; i--) { // Inverte a ordem (topo da pilha vai para frente da fila)
        fila->pecas[indiceFila] = tempPilha[i];
        indiceFila = (indiceFila + 1) % 5;
    }
    
    // Coloca as peças da fila na pilha (invertendo a ordem para manter LIFO)
    for (int i = 0; i < 3; i++) {
        pilha->pecas[i] = tempFila[2 - i]; // Inverte: primeiro da fila vai para base da pilha
    }
    
    printf("\nTroca multipla realizada: 3 primeiros da fila <-> 3 pecas da pilha\n");
    
    return 1; // Troca bem-sucedida
}

// ============================================================================
// IMPLEMENTAÇÃO DAS FUNÇÕES AUXILIARES
// ============================================================================

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
 * Exibe o estado completo do sistema (fila + pilha)
 * @param fila Ponteiro para a estrutura da fila
 * @param pilha Ponteiro para a estrutura da pilha
 */
void exibirEstadoCompleto(FilaPecas* fila, PilhaReserva* pilha) {
    printf("\n=== ESTADO ATUAL ===\n");
    exibirFila(fila);
    exibirPilha(pilha);
}

/**
 * Exibe o menu de opções para o usuário
 */
void exibirMenu() {
    printf("\nOpcoes disponiveis:\n");
    printf("1 - Jogar peca da frente da fila\n");
    printf("2 - Enviar peca da fila para a pilha de reserva\n");
    printf("3 - Usar peca da pilha de reserva\n");
    printf("4 - Trocar peca da frente da fila com o topo da pilha\n");
    printf("5 - Trocar os 3 primeiros da fila com as 3 pecas da pilha\n");
    printf("6 - Exibir estado atual\n");
    printf("0 - Sair\n");
    printf("Opcao escolhida: ");
}

/**
 * Obtém a opção escolhida pelo usuário
 * @return Opção escolhida (0-6)
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
 * Função principal do programa Tetris Stack Expert
 * Implementa o loop principal de interação com o usuário
 */
int main() {
    // Inicializa o gerador de números aleatórios
    srand(time(NULL));
    
    // Declara e inicializa as estruturas
    FilaPecas fila;
    PilhaReserva pilha;
    inicializarFila(&fila);
    inicializarPilha(&pilha);
    
    // Variáveis para controle do loop e operações
    int opcao;
    Peca pecaProcessada;
    
    printf("=== TETRIS STACK - SISTEMA EXPERT ===\n");
    printf("Bem-vindo ao simulador expert do Tetris Stack!\n");
    printf("Gerencie suas pecas com operacoes avancadas de troca.\n");
    
    // Loop principal do programa
    do {
        // Exibe o estado atual do sistema
        exibirEstadoCompleto(&fila, &pilha);
        
        // Exibe o menu e obtém a opção do usuário
        exibirMenu();
        opcao = obterOpcao();
        
        // Processa a opção escolhida
        switch (opcao) {
            case 1: // Jogar peça da frente da fila
                if (dequeueFila(&fila, &pecaProcessada)) {
                    printf("\nPeca jogada: [%c %d]\n", 
                           pecaProcessada.nome, pecaProcessada.id);
                    
                    // Gera automaticamente uma nova peça para manter a fila cheia
                    enqueueAutomatico(&fila);
                    printf("Nova peca gerada automaticamente para a fila.\n");
                } else {
                    printf("\nErro: Nao foi possivel jogar a peca.\n");
                }
                break;
                
            case 2: // Enviar peça da fila para a pilha de reserva
                if (pilhaCheia(&pilha)) {
                    printf("\nErro: Pilha de reserva cheia! Nao e possivel reservar mais pecas.\n");
                    printf("Use uma peca reservada primeiro para liberar espaco.\n");
                } else if (dequeueFila(&fila, &pecaProcessada)) {
                    if (pushPilha(&pilha, pecaProcessada)) {
                        printf("\nPeca enviada para reserva: [%c %d]\n", 
                               pecaProcessada.nome, pecaProcessada.id);
                        
                        // Gera automaticamente uma nova peça para manter a fila cheia
                        enqueueAutomatico(&fila);
                        printf("Nova peca gerada automaticamente para a fila.\n");
                    } else {
                        printf("\nErro: Nao foi possivel enviar a peca para reserva.\n");
                    }
                } else {
                    printf("\nErro: Nao foi possivel remover peca da fila.\n");
                }
                break;
                
            case 3: // Usar peça da pilha de reserva
                if (popPilha(&pilha, &pecaProcessada)) {
                    printf("\nPeca da reserva usada: [%c %d]\n", 
                           pecaProcessada.nome, pecaProcessada.id);
                } else {
                    printf("\nErro: Pilha de reserva vazia! Nao ha pecas reservadas para usar.\n");
                    printf("Envie uma peca para a reserva primeiro.\n");
                }
                break;
                
            case 4: // Trocar peça da frente da fila com o topo da pilha
                trocarSimples(&fila, &pilha);
                break;
                
            case 5: // Trocar os 3 primeiros da fila com as 3 peças da pilha
                trocarMultipla(&fila, &pilha);
                break;
                
            case 6: // Exibir estado atual
                printf("\nExibindo estado atual do sistema...\n");
                // O estado será exibido no início do próximo loop
                break;
                
            case 0: // Sair
                printf("\nSaindo do programa...\n");
                printf("Obrigado por jogar Tetris Stack Expert!\n");
                break;
                
            default: // Opção inválida
                printf("\nOpcao invalida! Por favor, escolha uma opcao de 0 a 6.\n");
                break;
        }
        
        // Pausa para melhor visualização (apenas em modo interativo)
        if (opcao != 0 && opcao >= 1 && opcao <= 6) {
            printf("\nPressione Enter para continuar...");
            while (getchar() != '\n'); // Limpa o buffer de entrada
        }
        
    } while (opcao != 0);
    
    return 0;
}