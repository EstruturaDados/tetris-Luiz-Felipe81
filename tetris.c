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
    Peca* pecas;    // Array de peças
    int frente;     // Índice da frente da fila
    int tras;       // Índice da traseira da fila
    int tamanho;    // Tamanho máximo da fila
    int contador;   // Contador de peças na fila
} Fila;

// Definindo a estrutura da pilha linear
typedef struct {
    Peca* pecas;    // Array de peças
    int topo;       // Índice do topo da pilha
    int tamanho;    // Tamanho máximo da pilha
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
void exibirMenu();
Peca gerarPeca(int id);
int filaCheia(Fila* fila);
int filaVazia(Fila* fila);
int pilhaCheia(Pilha* pilha);
int pilhaVazia(Pilha* pilha);
void trocarFrenteComTopo(Fila* fila, Pilha* pilha, int* idAtual);
void trocarTresPrimeiras(Fila* fila, Pilha* pilha, int* idAtual);

// Função principal
int main() {
    Fila fila;
    Pilha pilha;
    int opcao, idAtual = 0;

    srand(time(NULL)); // Inicializa a semente para números aleatórios

    // Inicializa fila (5) e pilha (3)
    inicializarFila(&fila, 5);
    inicializarPilha(&pilha, 3);

    // Preenche a fila com 5 peças iniciais
    for (int i = 0; i < 5; i++) {
        enqueue(&fila, gerarPeca(idAtual++));
    }

    // Loop principal
    do {
        exibirEstado(&fila, &pilha);
        exibirMenu();
        printf("Opção escolhida: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: // Jogar peça da frente da fila
                if (!filaVazia(&fila)) {
                    Peca jogada = dequeue(&fila);
                    printf("Ação: Peça jogada: [%c %d]\n", jogada.nome, jogada.id);
                    enqueue(&fila, gerarPeca(idAtual++)); // Repõe na fila
                } else {
                    printf("Erro: Fila vazia! Não há peças para jogar.\n");
                }
                break;

            case 2: // Reservar peça (fila -> pilha)
                if (!filaVazia(&fila) && !pilhaCheia(&pilha)) {
                    Peca reservada = dequeue(&fila);
                    push(&pilha, reservada);
                    printf("Ação: Peça reservada: [%c %d]\n", reservada.nome, reservada.id);
                    enqueue(&fila, gerarPeca(idAtual++)); // Repõe na fila
                } else if (filaVazia(&fila)) {
                    printf("Erro: Fila vazia! Não há peças para reservar.\n");
                } else {
                    printf("Erro: Pilha cheia! Não é possível reservar mais peças.\n");
                }
                break;

            case 3: // Usar peça reservada (pilha -> descarte)
                if (!pilhaVazia(&pilha)) {
                    Peca usada = pop(&pilha);
                    printf("Ação: Peça reservada usada: [%c %d]\n", usada.nome, usada.id);
                    enqueue(&fila, gerarPeca(idAtual++)); // Repõe na fila
                } else {
                    printf("Erro: Pilha vazia! Não há peças reservadas para usar.\n");
                }
                break;

            case 4: // Trocar frente da fila com topo da pilha
                if (!filaVazia(&fila) && !pilhaVazia(&pilha)) {
                    trocarFrenteComTopo(&fila, &pilha, &idAtual);
                } else if (filaVazia(&fila)) {
                    printf("Erro: Fila vazia! Não há peça na frente.\n");
                } else {
                    printf("Erro: Pilha vazia! Não há peça no topo.\n");
                }
                break;

            case 5: // Troca em bloco: 3 da fila <-> 3 da pilha
                if (fila.contador >= 3 && pilha.topo >= 2) {
                    trocarTresPrimeiras(&fila, &pilha, &idAtual);
                    printf("Ação: troca realizada entre os 3 primeiros da fila e as 3 da pilha.\n");
                } else {
                    printf("Erro: É necessário ter pelo menos 3 peças na fila e 3 na pilha para troca em bloco.\n");
                }
                break;

            case 0:
                printf("Saindo do programa...\n");
                break;

            default:
                printf("Erro: Opção inválida! Tente novamente.\n");
        }
        printf("\n");
    } while (opcao != 0);

    // Libera memória
    liberarFila(&fila);
    liberarPilha(&pilha);
    return 0;
}

// Inicializa a fila
void inicializarFila(Fila* fila, int tamanho) {
    fila->pecas = (Peca*)malloc(tamanho * sizeof(Peca));
    fila->tamanho = tamanho;
    fila->frente = 0;
    fila->tras = -1;
    fila->contador = 0;
}

// Inicializa a pilha
void inicializarPilha(Pilha* pilha, int tamanho) {
    pilha->pecas = (Peca*)malloc(tamanho * sizeof(Peca));
    pilha->tamanho = tamanho;
    pilha->topo = -1;
}

// Libera memória
void liberarFila(Fila* fila) { free(fila->pecas); }
void liberarPilha(Pilha* pilha) { free(pilha->pecas); }

// Verificações de estado
int filaCheia(Fila* fila) { return fila->contador == fila->tamanho; }
int filaVazia(Fila* fila) { return fila->contador == 0; }
int pilhaCheia(Pilha* pilha) { return pilha->topo == pilha->tamanho - 1; }
int pilhaVazia(Pilha* pilha) { return pilha->topo == -1; }

// Operações na fila
void enqueue(Fila* fila, Peca peca) {
    if (!filaCheia(fila)) {
        fila->tras = (fila->tras + 1) % fila->tamanho;
        fila->pecas[fila->tras] = peca;
        fila->contador++;
    }
}

Peca dequeue(Fila* fila) {
    Peca peca = {0, -1};
    if (!filaVazia(fila)) {
        peca = fila->pecas[fila->frente];
        fila->frente = (fila->frente + 1) % fila->tamanho;
        fila->contador--;
    }
    return peca;
}

// Operações na pilha
void push(Pilha* pilha, Peca peca) {
    if (!pilhaCheia(pilha)) {
        pilha->topo++;
        pilha->pecas[pilha->topo] = peca;
    }
}

Peca pop(Pilha* pilha) {
    Peca peca = {0, -1};
    if (!pilhaVazia(pilha)) {
        peca = pilha->pecas[pilha->topo];
        pilha->topo--;
    }
    return peca;
}

// Gera peça aleatória
Peca gerarPeca(int id) {
    Peca peca;
    char tipos[] = {'I', 'O', 'T', 'L'};
    peca.nome = tipos[rand() % 4];
    peca.id = id;
    return peca;
}

// Exibe estado atual com formatação clara
void exibirEstado(Fila* fila, Pilha* pilha) {
    printf("Estado atual:\n");
    printf("Fila de peças: ");
    if (filaVazia(fila)) {
        printf("[Vazia]\n");
    } else {
        int i = fila->frente;
        for (int j = 0; j < fila->contador; j++) {
            printf("[%c %d] ", fila->pecas[i].nome, fila->pecas[i].id);
            i = (i + 1) % fila->tamanho;
        }
        printf("\n");
    }

    printf("Pilha de reserva (Topo -> Base): ");
    if (pilhaVazia(pilha)) {
        printf("[Vazia]\n");
    } else {
        for (int i = pilha->topo; i >= 0; i--) {
            printf("[%c %d] ", pilha->pecas[i].nome, pilha->pecas[i].id);
        }
        printf("\n");
    }
    printf("Tabela: Visualização atual da fila de peças e da pilha de reserva.\n");
    printf("Curadoria de TI.\n");
}

// Exibe menu de opções
void exibirMenu() {
    printf("\nOpções disponíveis:\n");
    printf("Código  Ação\n");
    printf("1       Jogar peça da frente da fila\n");
    printf("2       Enviar peça da fila para a pilha de reserva\n");
    printf("3       Usar peça da pilha de reserva\n");
    printf("4       Trocar peça da frente da fila com o topo da pilha\n");
    printf("5       Trocar os 3 primeiros da fila com as 3 peças da pilha\n");
    printf("0       Sair\n");
    printf("Tabela: Comandos para movimentar peças entre a fila e a pilha de reserva.\n");
    printf("Curadoria de TI.\n");
}

// Troca a peça da frente da fila com o topo da pilha
void trocarFrenteComTopo(Fila* fila, Pilha* pilha, int* idAtual) {
    Peca temp = fila->pecas[fila->frente];  // Frente da fila
    Peca topo = pilha->pecas[pilha->topo];  // Topo da pilha

    // Troca direta
    fila->pecas[fila->frente] = topo;
    pilha->pecas[pilha->topo] = temp;

    printf("Ação: troca realizada entre frente da fila [%c %d] e topo da pilha [%c %d].\n",
           topo.nome, topo.id, temp.nome, temp.id);

    // Repõe na fila (mantém cheia)
    enqueue(fila, gerarPeca((*idAtual)++));
}

// Troca em bloco: 3 da fila <-> 3 da pilha
void trocarTresPrimeiras(Fila* fila, Pilha* pilha, int* idAtual) {
    Peca temp[3];

    // Salva as 3 primeiras da fila
    int idx = fila->frente;
    for (int i = 0; i < 3; i++) {
        temp[i] = fila->pecas[idx];
        idx = (idx + 1) % fila->tamanho;
    }

    // Move pilha (topo -> base) para fila (frente)
    idx = fila->frente;
    for (int i = 2; i >= 0; i--) {  // pilha.topo, topo-1, topo-2
        fila->pecas[idx] = pilha->pecas[pilha->topo - (2 - i)];
        idx = (idx + 1) % fila->tamanho;
    }

    // Move temp (antiga fila) para pilha
    for (int i = 0; i < 3; i++) {
        pilha->pecas[i] = temp[i];
    }
    pilha->topo = 2;  // Agora tem 3 itens

    // Repõe 3 peças novas na fila (mantém 5)
    for (int i = 0; i < 3; i++) {
        enqueue(fila, gerarPeca((*idAtual)++));
    }
}
