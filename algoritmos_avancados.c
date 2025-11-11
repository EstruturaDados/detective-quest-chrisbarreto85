#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*
  ----------------------------
  Estrutura de dados e funções
  ----------------------------

  Sala (nó da árvore):
    - nome: identifica a sala
    - esq, dir: ponteiros para salas adjacentes (esquerda/direita)

  criarSala():
    - aloca dinamicamente uma Sala e define seu nome

  explorarSalas():
    - permite a navegação interativa pela árvore a partir da raiz
    - opções: [e] esquerda, [d] direita, [s] sair
    - imprime cada sala visitada e ao final mostra o caminho percorrido

  liberarArvore():
    - desaloca toda a árvore (boa prática de memória)
*/

typedef struct Sala {
    char nome[64];
    struct Sala *esq;
    struct Sala *dir;
} Sala;

/* Cria, de forma dinâmica, uma sala com nome. */
Sala* criarSala(const char* nome) {
    Sala* nova = (Sala*) malloc(sizeof(Sala));
    if (!nova) {
        fprintf(stderr, "Erro: falha ao alocar memoria para a sala \"%s\".\n", nome);
        exit(EXIT_FAILURE);
    }
    /* copia o nome com segurança e garante terminador nulo */
    strncpy(nova->nome, nome, sizeof(nova->nome) - 1);
    nova->nome[sizeof(nova->nome) - 1] = '\0';
    nova->esq = NULL;
    nova->dir = NULL;
    return nova;
}

/* Desaloca toda a árvore binária (pós-ordem). */
void liberarArvore(Sala* raiz) {
    if (!raiz) return;
    liberarArvore(raiz->esq);
    liberarArvore(raiz->dir);
    free(raiz);
}

/* Acrescenta texto a um buffer de forma segura (impede overflow). */
void appendSeguro(char* dest, size_t cap, const char* src) {
    size_t lenDest = strlen(dest);
    size_t lenSrc  = strlen(src);
    if (lenDest >= cap - 1) return; /* já cheio */
    size_t espaco = cap - 1 - lenDest;
    strncat(dest, src, espaco < lenSrc ? espaco : lenSrc);
}

/* Lê a opção do usuário (primeiro caractere não espaço) como minúsculo. */
char lerOpcao() {
    char linha[128];
    if (!fgets(linha, sizeof(linha), stdin)) return 's'; /* em caso de EOF, sair */
    /* encontra primeiro caractere não-espaço */
    for (size_t i = 0; linha[i] != '\0'; i++) {
        if (!isspace((unsigned char)linha[i])) {
            return (char)tolower((unsigned char)linha[i]);
        }
    }
    return '\0'; /* vazio */
}

/* Mostra o menu de opções baseado nas saídas disponíveis. */
void mostrarMenu(const Sala* atual) {
    printf("\nO que deseja fazer?\n");
    if (atual->esq) printf("  [e] Ir para a ESQUERDA  -> %s\n", atual->esq->nome);
    else            printf("  [e] Esquerda indisponivel\n");
    if (atual->dir) printf("  [d] Ir para a DIREITA   -> %s\n", atual->dir->nome);
    else            printf("  [d] Direita indisponivel\n");
    printf("  [s] Sair da exploracao\n");
    printf("Sua escolha: ");
}

/* 
   Permite a navegação do jogador pela árvore a partir da raiz.
   Exibe cada sala visitada e, ao final, o caminho completo.
*/
void explorarSalas(Sala* raiz) {
    if (!raiz) {
        printf("Mapa vazio.\n");
        return;
    }

    /* Buffer para registrar o caminho (salas visitadas em ordem). */
    char trilha[1024];
    trilha[0] = '\0';

    Sala* atual = raiz;

    /* Entrando na primeira sala (raiz). */
    printf("\n>>> Voce entrou em: %s\n", atual->nome);
    appendSeguro(trilha, sizeof(trilha), atual->nome);

    while (1) {
        /* Verifica se é um nó-folha (sem saídas). */
        if (!atual->esq && !atual->dir) {
            printf("\nNao ha caminhos a esquerda nem a direita.\n");
            printf("Fim da exploracao: voce chegou a um comodo sem saidas.\n");
            break;
        }

        /* Mostra menu para a sala atual. */
        mostrarMenu(atual);
        char op = lerOpcao();

        if (op == 's') {
            printf("\nExploracao encerrada pelo jogador.\n");
            break;
        } else if (op == 'e') {
            if (atual->esq) {
                atual = atual->esq;
                printf("\n>>> Voce entrou em: %s\n", atual->nome);
                appendSeguro(trilha, sizeof(trilha), " -> ");
                appendSeguro(trilha, sizeof(trilha), atual->nome);
            } else {
                printf("\nCaminho a ESQUERDA inexistente a partir de \"%s\".\n", atual->nome);
            }
        } else if (op == 'd') {
            if (atual->dir) {
                atual = atual->dir;
                printf("\n>>> Voce entrou em: %s\n", atual->nome);
                appendSeguro(trilha, sizeof(trilha), " -> ");
                appendSeguro(trilha, sizeof(trilha), atual->nome);
            } else {
                printf("\nCaminho a DIREITA inexistente a partir de \"%s\".\n", atual->nome);
            }
        } else {
            printf("\nOpcao invalida. Use 'e' (esquerda), 'd' (direita) ou 's' (sair).\n");
        }
    }

    printf("\n--- Salas visitadas ---\n");
    printf("%s\n", trilha[0] ? trilha : "(nenhuma)");
}

/*
  ----------------
  Mapa pré-definido
  ----------------

  A árvore é criada manualmente na main() usando criarSala(), por exemplo:

              [Hall de entrada]
                  /        \
        [Sala de Estar]   [Biblioteca]
           /      \          /      \
     [Cozinha]  [Jardim] [Escritorio] [Porao]

  Ajuste os nomes/estrutura como preferir; a árvore não muda em tempo de execução.
*/

int main(void) {
    /* Criação das salas (nós) */
    Sala* hall        = criarSala("Hall de entrada");
    Sala* estar       = criarSala("Sala de Estar");
    Sala* biblioteca  = criarSala("Biblioteca");
    Sala* cozinha     = criarSala("Cozinha");
    Sala* jardim      = criarSala("Jardim");
    Sala* escritorio  = criarSala("Escritorio");
    Sala* porao       = criarSala("Porao");

    /* Ligações (estrutura binária fixa) */
    hall->esq = estar;
    hall->dir = biblioteca;

    estar->esq = cozinha;
    estar->dir = jardim;

    biblioteca->esq = escritorio;
    biblioteca->dir = porao;

    /* Título / introdução */
    printf("=== Exploracao da Mansao (Arvore Binaria) ===\n");
    printf("Comece no Hall de entrada e escolha seu caminho.\n");

    /* Inicia a exploração interativa */
    explorarSalas(hall);

    /* Libera memória alocada */
    liberarArvore(hall);

    printf("\nObrigado por jogar!\n");
    return 0;
}


