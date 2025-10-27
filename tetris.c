#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Definindo a estrutura da peça
typedef struct {
    char nome;  // Tipo da peça ('I', 'O', 'T', 'L')
    int id;     // Identificador único da peça
} Peca;

// Definindo a estrutura da fila circular
typedef struct {
    Peca* pecas;     // Array de peças
    int frente;      // Índice da frente da fila
    int tras;        // Índice da traseira da fila
    int tamanho;     // Tamanho máximo da fila
    int contador;    // Contador de peças na fila
} Fila;

// Prototipagem das funções
void inicializarFila(Fila* fila, int tamanho);
void liberarFila(Fila* fila);
void enqueue(Fila* fila, Peca peca);
Peca dequeue(Fila* fila);
void exibirFila(Fila* fila);
Peca gerarPeca(int id);
int filaCheia(Fila* fila);
int filaVazia(Fila* fila);

// Função principal
int main() {
    Fila fila;
    int opcao, idAtual = 0;
    srand(time(NULL)); // Inicializa a semente para números aleatórios

    // Inicializa a fila com 5 posições
    inicializarFila(&fila, 5);

    // Preenche a fila com 5 peças iniciais
    for (int i = 0; i < 5; i++) {
        enqueue(&fila, gerarPeca(idAtual++));
    }

    // Loop principal do programa
    do {
        // Exibe o estado atual da fila
        exibirFila(&fila);

        // Exibe o menu
        printf("\nOpcoes de acao:\n");
        printf("1 - Jogar peca (dequeue)\n");
        printf("2 - Inserir nova peca (enqueue)\n");
        printf("0 - Sair\n");
        printf("Digite sua escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: // Jogar peça (dequeue)
                if (!filaVazia(&fila)) {
                    Peca peca = dequeue(&fila);
                    printf("Peca jogada: [%c %d]\n", peca.nome, peca.id);
                } else {
                    printf("Fila vazia! Nao ha pecas para jogar.\n");
                }
                break;

            case 2: // Inserir nova peça (enqueue)
                if (!filaCheia(&fila)) {
                    enqueue(&fila, gerarPeca(idAtual++));
                    printf("Nova peca inserida.\n");
                } else {
                    printf("Fila cheia! Nao e possivel inserir nova peca.\n");
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

// Libera a memória alocada para a fila
void liberarFila(Fila* fila) {
    free(fila->pecas);
}

// Verifica se a fila está cheia
int filaCheia(Fila* fila) {
    return fila->contador == fila->tamanho;
}

// Verifica se a fila está vazia
int filaVazia(Fila* fila) {
    return fila->contador == 0;
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

// Exibe o estado atual da fila
void exibirFila(Fila* fila) {
    printf("\nFila de pecas: ");
    if (filaVazia(fila)) {
        printf("[Vazia]\n");
        return;
    }

    int i = fila->frente;
    for (int j = 0; j < fila->contador; j++) {
        printf("[%c %d] ", fila->pecas[i].nome, fila->pecas[i].id);
        i = (i + 1) % fila->tamanho;
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