#include <stdio.h>
#include <stdlib.h>
//2° função: Definição da estrutura Pessoa e Fila
typedef struct {//Definição da estrutura Pessoa
    char nome[1];//Nome da pessoa
    int id;//Idade da pessoa
} Peca;//Definição da estrutura Pessoa

//3° Definir o tamanho da fila
#define MAX 5 //Tamanho máximo da fila

//4° função: Definição da estrutura Fila
typedef struct {//Definição da estrutura Fila
    Peca itens[MAX];//Array de pessoas na fila
    int inicio;//Índice do início da fila
    int fim;//Índice do fim da fila
    int total;//Total de elementos na fila
} Fila;//Definição da estrutura Fila

//5° função: Prototipação das funções Função de inicialização da fila, Verificação se a fila está cheia, Verificação se a fila está vazia, Inserção de uma pessoa na fila, Remoção de uma pessoa da fila, Exibição do conteúdo da fila
void inicializarFila(Fila *f) {//1° função: Inicializa a fila
    f->inicio = 0;//Inicia o índice de início
    f->fim = 0;//Inicia o índice de fim
    f->total = 0;//Total de elementos na fila
}

int filaCheia(Fila *f) {//2° função: Verifica se a fila está cheia
    return f->total == MAX;//Retorna 1 se cheia, 0 caso contrário
}

int filaVazia(Fila *f) {//3° função: Verifica se a fila está vazia
    return f->total == 0;//Retorna 1 se a fila estiver vazia, 0 caso contrário
}
//6° função: Insere uma pessoa na fila
void inserir(Fila *f, Peca p){
    if (filaCheia(f)) {//Verifica se a fila está cheia
        printf("Fila cheia. Não é possível inserir.\n");//Exibe mensagem se cheia
        return;
    }
    f->itens[f->fim] = p;//Insere a pessoa no final da fila
    f->fim = (f->fim + 1) % MAX;//Atualiza o índice de fim circularmente
    f->total++;//Incrementa o total de elementos
    printf("Peça %s inserida na fila.\n", p.nome);//Exibe mensagem
}
//7° função: Remove uma pessoa da fila
void remover(Fila *f, Peca *p) {
    if (filaVazia(f)) {// Verifica se a fila está vazia
        printf("Fila vazia. Não é possível remover.\n");//Exibe mensagem se vazia
        return;
    }
    *p = f->itens[f->inicio];// Armazena a pessoa a ser removida
    printf("Peça %s removida da fila.\n", p->nome);//Exibe mensagem
    f->inicio = (f->inicio + 1) % MAX; // Atualiza o índice de início circularmente      
    f->total--;// Decrementa o total de elementos 
}
//8° função: Mostra o conteúdo da fila
void mostrarFila(Fila *f) {
    printf("Fila: ");//Exibe o cabeçalho da fila
    for (int i = 0, idx = f->inicio; i < f->total; i++, idx = (idx + 1) % MAX) {// Percorre a fila
        printf("[%s, %d] ", f->itens[idx].nome, f->itens[idx].id);// Exibe cada pessoa na fila
    }
    printf("\n");//Nova linha após exibir a fila
}
//1° função: Inicializa a fila
int main(){
    // Exemplo de uso da fila
    Fila f;
    inicializarFila(&f);//Inicializa a fila

    int numero;

    do {
        printf("Digite 1 para Remover uma Peça: \n Digite 2 para Inserir uma Peça: \n Digite 3 para Mostrar a Fila: \n Digite 0 para Sair: ");
        scanf("%d", &numero);
        if (numero == 1) {
            if (filaVazia(&f)) {
                printf("Fila vazia. Não é possível remover.\n");
                continue;
            }
            Peca removida;
            printf("Removendo peça...\n");
            remover(&f, &removida);
            printf("Peça removida: %s, %d\n", removida.nome, removida.id);
        } else if (numero == 2) {
            if (filaCheia(&f)) {
                printf("Fila cheia. Não é possível inserir.\n");
                continue;
            }
            Peca novaPeca;
            printf("Digite a Letra da peça: ");
            scanf("%s", novaPeca.nome);
            &novaPeca.id+1;
            inserir(&f, novaPeca);
            printf("Peça %s inserida na fila.\n", novaPeca.nome);
        } else if (numero == 3) {
            mostrarFila(&f);
        } else if (numero != 0) {
            printf("Opção inválida. Tente novamente.\n");
        }
    } while (numero != 0);

    return 0;
}
