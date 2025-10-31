#include <stdio.h>
#include <stdlib.h>
#include <time.h> // Necessário para gerar peças aleatórias

// Definição da estrutura Peca
typedef struct {
    char nome; // Nome/Tipo da peça (char único)
    int id;    // ID único da peça
} Peca;

// Definir os tamanhos
#define MAX_FILA 5  // Tamanho máximo da fila
#define MAX_PILHA 3 // Capacidade limitada da pilha de reserva

// Definição da estrutura Fila
typedef struct {
    Peca itens[MAX_FILA]; // Array de peças na fila
    int inicio;          // Índice do início da fila
    int fim;             // Índice do fim da fila
    int total;           // Total de elementos na fila
} Fila;

// Estrutura da Pilha
typedef struct {
    Peca itens[MAX_PILHA]; // Array de peças na pilha
    int topo;             // Índice do topo da pilha
} Pilha;

// Variável global para garantir IDs únicos
int proximoId = 1;

// --- Funções da Fila ---

void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->fim = 0;
    f->total = 0;
}

int filaCheia(Fila *f) {
    return f->total == MAX_FILA;
}

int filaVazia(Fila *f) {
    return f->total == 0;
}

void enfileirar(Fila *f, Peca p) {
    if (filaCheia(f)) {
        printf("Fila cheia. Não é possível inserir.\n");
        return;
    }
    f->itens[f->fim] = p;
    f->fim = (f->fim + 1) % MAX_FILA;
    f->total++;
}

int desenfileirar(Fila *f, Peca *p) {
    if (filaVazia(f)) {
        printf("Fila vazia. Não é possível remover.\n");
        return 0; // Indica falha
    }
    *p = f->itens[f->inicio];
    f->inicio = (f->inicio + 1) % MAX_FILA;
    f->total--;
    return 1; // Indica sucesso
}

void mostrarFila(Fila *f) {
    printf("FILA (frente -> fundo):\n");
    if (filaVazia(f)) {
        printf("[ VAZIA ]\n");
        return;
    }
    printf("FRENTE -> ");
    int idx = f->inicio;
    for (int i = 0; i < f->total; i++) {
        printf("[%c, %d] ", f->itens[idx].nome, f->itens[idx].id);
        idx = (idx + 1) % MAX_FILA;
    }
    printf("<- FUNDO\n");
}

// --- Funções da Pilha ---

void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

int pilhaVazia(Pilha *p) {
    return p->topo == -1;
}

int pilhaCheia(Pilha *p) {
    return p->topo == MAX_PILHA - 1; // Usa a constante da pilha
}

void empilhar(Pilha *p, Peca nova) {
    if (pilhaCheia(p)) {
        printf("Pilha de reserva cheia. Não é possível inserir.\n");
        return;
    }
    p->topo++;
    p->itens[p->topo] = nova;
}

int desempilhar(Pilha *p, Peca *removida) {
    if (pilhaVazia(p)) {
        printf("Pilha de reserva vazia. Não é possível remover.\n");
        return 0; // Indica falha
    }
    *removida = p->itens[p->topo];
    p->topo--;
    return 1; // Indica sucesso
}

void mostrarPilha(Pilha *p) {
    printf("PILHA (topo -> base):\n");
    if (pilhaVazia(p)) {
        printf("[ VAZIA ]\n");
        return;
    }
    for (int i = p->topo; i >= 0; i--) {
        printf("  [%c, %d]\n", p->itens[i].nome, p->itens[i].id);
    }
    printf("------------------\n");
}

// --- Funções do Jogo ---

// Nova função para gerar peças automaticamente
Peca gerarPeca() {
    Peca nova;
    char tipos[] = {'I', 'O', 'T', 'L'};
    nova.nome = tipos[rand() % 4]; // Escolhe um tipo aleatório
    nova.id = proximoId++;         // Atribui o ID único e incrementa
    return nova;
}

// NOVA FUNÇÃO: Troca a peça da frente da fila com o topo da pilha
void trocarPecaAtual(Fila *f, Pilha *p) {
    if (filaVazia(f)) {
        printf("\n>>> ERRO: Fila vazia! Não pode trocar.\n");
        return;
    }
    if (pilhaVazia(p)) {
        printf("\n>>> ERRO: Pilha vazia! Não pode trocar.\n");
        return;
    }

    // Realiza a troca
    Peca temp = p->itens[p->topo];
    p->itens[p->topo] = f->itens[f->inicio];
    f->itens[f->inicio] = temp;

    printf("\n>>> AÇÃO: Peça [%c, %d] (Fila) TROCADA com [%c, %d] (Pilha).\n", 
           p->itens[p->topo].nome, p->itens[p->topo].id, 
           f->itens[f->inicio].nome, f->itens[f->inicio].id);
}

// NOVA FUNÇÃO: Troca as 3 primeiras da fila com as 3 do topo da pilha
void trocaMultipla(Fila *f, Pilha *p) {
    // Verifica se há peças suficientes
    if (f->total < 3) {
        printf("\n>>> ERRO: Fila não tem 3 peças! (%d/3)\n", f->total);
        return;
    }
    // p->topo é 0-indexado, topo 2 = 3 peças (0, 1, 2)
    if (p->topo < 2) { 
        printf("\n>>> ERRO: Pilha não tem 3 peças! (%d/3)\n", p->topo + 1);
        return;
    }

    // Realiza a troca de 3 peças
    for (int i = 0; i < 3; i++) {
        int fila_idx = (f->inicio + i) % MAX_FILA; // Índice circular da fila
        int pilha_idx = p->topo - i;            // Índice linear da pilha

        Peca temp = p->itens[pilha_idx];
        p->itens[pilha_idx] = f->itens[fila_idx];
        f->itens[fila_idx] = temp;
    }

    printf("\n>>> AÇÃO: 3 peças da Fila trocadas com 3 peças da Pilha.\n");
}


// Função principal com a lógica do jogo
int main() {
    Fila f;
    Pilha p;
    int numero;
    int acaoRealizada = 0; // Flag para controlar geração de nova peça
    int mostrarEstado = 1; // Flag para controlar exibição do estado

    // Inicializa o gerador de números aleatórios
    srand(time(NULL));

    // Inicializa as estruturas
    inicializarFila(&f);
    inicializarPilha(&p);

    // 1. Inicializa a fila de peças com 5 elementos
    printf("Inicializando o jogo com 5 peças...\n");
    for (int i = 0; i < MAX_FILA; i++) {
        enfileirar(&f, gerarPeca());
    }

    // Mostra o estado inicial
    printf("\n--- ESTADO INICIAL ---\n");
    mostrarFila(&f);
    mostrarPilha(&p);
    printf("----------------------\n");

    do {
        printf("\nEscolha sua ação:\n");
        printf("1. Jogar peça (remove da fila)\n");
        printf("2. Reservar peça (fila -> pilha)\n");
        printf("3. Usar peça reservada (remove da pilha)\n");
        printf("4. Trocar peça atual (fila <-> pilha)\n");
        printf("5. Troca múltipla (3 fila <-> 3 pilha)\n");
        printf("6. Visualizar estado atual\n");
        printf("0. Sair\n");
        printf("Opção: ");
        scanf("%d", &numero);

        acaoRealizada = 0; // Reseta a flag
        mostrarEstado = 1; // Mostra o estado por padrão
        Peca pecaTemp;

        switch (numero) {
            case 1:
                // 3. Jogar uma peça
                if (desenfileirar(&f, &pecaTemp)) {
                    printf("\n>>> AÇÃO: Peça [%c, %d] JOGADA.\n", pecaTemp.nome, pecaTemp.id);
                    acaoRealizada = 1; // Fila diminuiu, precisa gerar nova peça
                }
                break;

            case 2:
                // 4. Reservar uma peça
                if (pilhaCheia(&p)) {
                    printf("\n>>> ERRO: Pilha de reserva está cheia! Não pode reservar.\n");
                } else if (filaVazia(&f)) {
                     printf("\n>>> ERRO: Fila está vazia! Não pode reservar.\n");
                } else {
                    desenfileirar(&f, &pecaTemp);
                    empilhar(&p, pecaTemp);
                    printf("\n>>> AÇÃO: Peça [%c, %d] RESERVADA.\n", pecaTemp.nome, pecaTemp.id);
                    acaoRealizada = 1; // Fila diminuiu, precisa gerar nova peça
                }
                break;

            case 3:
                // 5. Usar uma peça reservada
                if (desempilhar(&p, &pecaTemp)) {
                    printf("\n>>> AÇÃO: Peça reservada [%c, %d] USADA.\n", pecaTemp.nome, pecaTemp.id);
                    // Não gera nova peça, pois a fila não foi alterada
                }
                break;

            case 4:
                // NOVA AÇÃO: Trocar peça atual
                trocarPecaAtual(&f, &p);
                break;
            
            case 5:
                // NOVA AÇÃO: Troca múltipla
                trocaMultipla(&f, &p);
                break;

            case 6:
                // NOVA AÇÃO: Apenas visualizar
                printf("\n>>> VISUALIZANDO ESTADO ATUAL.\n");
                break;

            case 0:
                printf("Saindo do jogo...\n");
                mostrarEstado = 0; // Não mostrar o estado final
                break;

            default:
                printf("Opção inválida. Tente novamente.\n");
                mostrarEstado = 0; // Não mostrar estado se a opção for inválida
                break;
        }

        // Gera uma nova peça APENAS se a fila diminuiu (Jogar ou Reservar)
        if (acaoRealizada) {
            Peca nova = gerarPeca();
            printf("... Nova peça [%c, %d] gerada e adicionada ao final da fila.\n", nova.nome, nova.id);
            enfileirar(&f, nova);
        }

        // Exibe o estado atual após a ação
        if (mostrarEstado) {
            printf("\n--- ESTADO ATUAL ---\n");
            mostrarFila(&f);
            mostrarPilha(&p);
            printf("----------------------\n");
        }

    } while (numero != 0);

    return 0;
}
