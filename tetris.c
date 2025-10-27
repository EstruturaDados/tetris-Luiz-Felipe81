#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Definindo a estrutura da peça
typedef struct {
    char nome; // Tipo da peça ('I', 'O', 'T', 'L')
    int id;    // Identificador único da peça
} Peca;

// Definindo a estrutura da fila circular
typedef struct {
    Peca* pecas;   // Array de peças
    int frente;    // Índice da frente da fila
    int tras;      // Índice da traseira da fila
    int tamanho;   // Tamanho máximo da fila
    int contador;  // Contador de peças na fila
} Fila;

// Definindo a estrutura da pilha linear
typedef struct {
    Peca* pecas;   // Array de peças
    int topo;      // Índice do topo da pilha
    int tamanho;   // Tamanho máximo da pilha
} Pilha;

// Prototipagem das funções
void inicializarFila(Fila* fila, int tamanho);
void inicializarPilha(Pilha* pilha, int tamanho);
void liberarFila(Fila* fila);
void liberarPilha(Pilha* pilha);
void enqueue(Fila* fila, Peca peca);
Peca dequeue(Fila* fila);
void push(Pilha* pilha, Peca peca);
Peca pop(Pilha* pilha);
void exibirEstado(Fila* fila, Pilha* pilha);
Peca gerarPeca(int id);
int filaCheia(Fila* fila);
int filaVazia(Fila* fila);
int pilhaCheia(Pilha* pilha);
int pilhaVazia(Pilha* pilha);

// Função principal
int main() {
    Fila fila;
    Pilha pilha;
    int opcao, idAtual = 0;
    srand(time(NULL)); // Inicializa a semente para números aleatórios

    // Inicializa a fila com 5 posições e a pilha com 3 posições
    inicializarFila(&fila, 5);
    inicializarPilha(&pilha, 3);

    // Preenche a fila com 5 peças iniciais
    for (int i = 0; i < 5; i++) {
        enqueue(&fila, gerarPeca(idAtual++));
    }

    // Loop principal do programa
    do {
        // Exibe o estado atual da fila e da pilha
        exibirEstado(&fila, &pilha);

        // Exibe o menu
        printf("\nOpcoes de acao:\n");
        printf("1 - Jogar peca\n");
        printf("2 - Reservar peca\n");
        printf("3 - Usar peca reservada\n");
        printf("0 - Sair\n");
        printf("Digite sua escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: // Jogar peça (dequeue)
                if (!filaVazia(&fila)) {
                    Peca peca = dequeue(&fila);
                    printf("Peca jogada: [%c %d]\n", peca.nome, peca.id);
                    // Adiciona nova peça para manter a fila cheia
                    enqueue(&fila, gerarPeca(idAtual++));
                } else {
                    printf("Fila vazia! Nao ha pecas para jogar.\n");
                }
                break;

            case 2: // Reservar peça
                if (!filaVazia(&fila) && !pilhaCheia(&pilha)) {
                    Peca peca = dequeue(&fila);
                    push(&pilha, peca);
                    printf("Peca reservada: [%c %d]\n", peca.nome, peca.id);
                    // Adiciona nova peça para manter a fila cheia
                    enqueue(&fila, gerarPeca(idAtual++));
                } else if (filaVazia(&fila)) {
                    printf("Fila vazia! Nao ha pecas para reservar.\n");
                } else {
                    printf("Pilha cheia! Nao e possivel reservar mais pecas.\n");
                }
                break;

            case 3: // Usar peça reservada
                if (!pilhaVazia(&pilha)) {
                    Peca peca = pop(&pilha);
                    printf("Peca reservada usada: [%c %d]\n", peca.nome, peca.id);
                    // Adiciona nova peça para manter a fila cheia
                    enqueue(&fila, gerarPeca(idAtual++));
                } else {
                    printf("Pilha vazia! Nao ha pecas reservadas para usar.\n");
                }
                break;

            case 0: // Sair
                printf("Saindo do programa...\n");
                break;

            default:
                printf("Opcao invalida! Tente novamente.\n");
        }
    } while (opcao != 0);

    // Libera a memória alocada
    liberarFila(&fila);
    liberarPilha(&pilha);
    return 0;
}

// Inicializa a fila com o tamanho especificado
void inicializarFila(Fila* fila, int tamanho) {
    fila->pecas = (Peca*)malloc(tamanho * sizeof(Peca));
    fila->tamanho = tamanho;
    fila->frente = 0;
    fila->tras = -1;
    fila->contador = 0;
}

// Inicializa a pilha com o tamanho especificado
void inicializarPilha(Pilha* pilha, int tamanho) {
    pilha->pecas = (Peca*)malloc(tamanho * sizeof(Peca));
    pilha->tamanho = tamanho;
    pilha->topo = -1; // Pilha vazia
}

// Libera a memória alocada para a fila
void liberarFila(Fila* fila) {
    free(fila->pecas);
}

// Libera a memória alocada para a pilha
void liberarPilha(Pilha* pilha) {
    free(pilha->pecas);
}

// Verifica se a fila está cheia
int filaCheia(Fila* fila) {
    return fila->contador == fila->tamanho;
}

// Verifica se a fila está vazia
int filaVazia(Fila* fila) {
    return fila->contador == 0;
}

// Verifica se a pilha está cheia
int pilhaCheia(Pilha* pilha) {
    return pilha->topo == pilha->tamanho - 1;
}

// Verifica se a pilha está vazia
int pilhaVazia(Pilha* pilha) {
    return pilha->topo == -1;
}

// Insere uma peça no final da fila
void enqueue(Fila* fila, Peca peca) {
    if (!filaCheia(fila)) {
        fila->tras = (fila->tras + 1) % fila->tamanho; // Avança o índice traseiro circularmente
        fila->pecas[fila->tras] = peca;
        fila->contador++;
    }
}

// Remove e retorna a peça da frente da fila
Peca dequeue(Fila* fila) {
    Peca peca = {0, -1}; // Peça padrão para caso de erro
    if (!filaVazia(fila)) {
        peca = fila->pecas[fila->frente];
        fila->frente = (fila->frente + 1) % fila->tamanho; // Avança o índice frontal circularmente
        fila->contador--;
    }
    return peca;
}

// Insere uma peça no topo da pilha
void push(Pilha* pilha, Peca peca) {
    if (!pilhaCheia(pilha)) {
        pilha->topo++;
        pilha->pecas[pilha->topo] = peca;
    }
}

// Remove e retorna a peça do topo da pilha
Peca pop(Pilha* pilha) {
    Peca peca = {0, -1}; // Peça padrão para caso de erro
    if (!pilhaVazia(pilha)) {
        peca = pilha->pecas[pilha->topo];
        pilha->topo--;
    }
    return peca;
}

// Exibe o estado atual da fila e da pilha
void exibirEstado(Fila* fila, Pilha* pilha) {
    printf("\nEstado atual:\n");
    printf("Fila de pecas: ");
    if (filaVazia(fila)) {
        printf("[Vazia]");
    } else {
        int i = fila->frente;
        for (int j = 0; j < fila->contador; j++) {
            printf("[%c %d] ", fila->pecas[i].nome, fila->pecas[i].id);
            i = (i + 1) % fila->tamanho;
        }
    }

    printf("\nPilha de reserva (Topo -> Base): ");
    if (pilhaVazia(pilha)) {
        printf("[Vazia]");
    } else {
        for (int i = pilha->topo; i >= 0; i--) {
            printf("[%c %d] ", pilha->pecas[i].nome, pilha->pecas[i].id);
        }
    }
    printf("\n");
}

// Gera uma nova peça com nome aleatório e id fornecido
Peca gerarPeca(int id) {
    Peca peca;
    char tipos[] = {'I', 'O', 'T', 'L'};
    peca.nome = tipos[rand() % 4]; // Escolhe um tipo aleatoriamente
    peca.id = id;
    return peca;
}
