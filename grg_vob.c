#include <stdio.h>
#include <stdlib.h>

struct no {
  int valor;
  struct no* esquerda;
  struct no* direita;
  int altura;
};

//helpers
int altura(struct no* node);
int maximo(int a, int b);
//

//AVL builders
struct no* criarFolha(int valor);
void atualizaAltura(struct no* node);
struct no* inserirAVL(int valor, struct no* raiz);
//

//Rotacoes
struct no* rotacaoSimplesDireita(struct no* raiz);
struct no* rotacaoDuplaDireita(struct no* raiz);
struct no* rotacaoSimplesEsquerda(struct no* raiz);
struct no* rotacaoDuplaEsquerda(struct no* raiz);
//

//Descrever
struct no* arvore_in_ordem2(struct no* node, int valor, int valor_desejado);
void descreveFolha(struct no* node, int index, int valor_desejado);
//

int myindex = 0;
int flag = 0;

int main() {
  int instrucoes;
  int query;
  int valor_passado;
  struct no* raiz = (struct no*) malloc (sizeof(struct no));
  if (!raiz) {
    printf("ERROR\n");
    exit(-1);
  }
  raiz = NULL;
  scanf("%d",&instrucoes);
  scanf("%d %d",&query,&valor_passado);
  raiz = criarFolha(valor_passado);
  instrucoes--;
  for (int i=0; i<instrucoes; i++) {
    myindex = 0;
    flag = 0;
    scanf("%d %d",&query,&valor_passado);
    if (query == 1) { //codigo para inserir na arvore
      raiz = inserirAVL(valor_passado, raiz);
    }
    else {
      struct no* x = arvore_in_ordem2(raiz,valor_passado,valor_passado);
      //printf("iosjidofsio = %d\n",x->valor);
      if (!flag) {
        printf("Data tidak ada\n");
      }
    }
  }
  return 0;
}

int altura (struct no* node) {
  if (!node) {
    return 0;
  }
  return node->altura;
}

int maximo(int a, int b) {
  if (a > b) {
    return a;
  }
  return b;
}

struct no* criarFolha(int valor) {
  struct no* node = (struct no*)malloc(sizeof(struct no));
  node->valor = valor;
  node->esquerda = NULL;
  node->direita = NULL;
  node->altura = 1;
  return node;
}

void atualizaAltura(struct no* node) {
  int altura_direita;
  int altura_esquerda;
  if (node) {
    altura_direita = altura(node->direita);
    altura_esquerda = altura(node->esquerda);
    node->altura = maximo(altura_direita, altura_esquerda) + 1;
  }
}

struct no* inserirAVL(int valor, struct no* node) {
  if (!node) {
    return criarFolha(valor);
  }
  else {
    if (valor < node->valor) {
      node->esquerda = inserirAVL(valor, node->esquerda);
      if (altura(node->esquerda) - altura(node->direita) == 2) {
        return valor < node->esquerda->valor ? rotacaoSimplesDireita(node) : rotacaoDuplaDireita(node);
      }
    }
    else {
      node->direita = inserirAVL(valor, node->direita);
      if (altura(node->esquerda) - altura(node->direita) == -2) {
        return valor > node->direita->valor ? rotacaoSimplesEsquerda(node) : rotacaoDuplaEsquerda(node);
      }
    }
    atualizaAltura(node);
  }
  return node;
}

struct no* rotacaoSimplesEsquerda(struct no* node) {
  struct no* auxiliar = (struct no*) malloc(sizeof(struct no));
  auxiliar = node->direita;
  node->direita = auxiliar->esquerda;
  auxiliar->esquerda = node;
  atualizaAltura(node);
  atualizaAltura(auxiliar);
  return auxiliar;
}

struct no* rotacaoSimplesDireita(struct no* node) {
  struct no* auxiliar = (struct no*) malloc(sizeof(struct no));
  auxiliar = node->esquerda;
  node->esquerda = auxiliar->direita;
  auxiliar->direita = node;
  atualizaAltura(node);
  atualizaAltura(auxiliar);
  return auxiliar;
}

struct no* rotacaoDuplaEsquerda(struct no* node) {
  struct no* resultado = (struct no*) malloc(sizeof(struct no));
  node->direita = rotacaoSimplesDireita(node->direita);
  resultado = rotacaoSimplesEsquerda(node);
  return resultado;
}

struct no* rotacaoDuplaDireita(struct no* node) {
  struct no* resultado = (struct no*) malloc(sizeof(struct no));
  node->esquerda = rotacaoSimplesEsquerda(node->esquerda);
  resultado = rotacaoSimplesDireita(node);
  return resultado;
}

struct no* arvore_in_ordem2(struct no* node, int valor, int valor_desejado) {
  if (!node) {
    return NULL;
  }
    arvore_in_ordem2(node->esquerda, valor, valor_desejado);
    myindex = myindex + 1;
    if (node->valor == valor) {
      flag = 1;
      printf("%d\n",myindex);
    }
    arvore_in_ordem2(node->direita, valor, valor_desejado);
    return NULL;
}

void descreveFolha(struct no* node, int index, int valor_desejado) {
  printf("Index: %d - Valor %d - A esquerda: ",index,node->valor);
  if (node->esquerda) {
    printf("%d - A direita: ",node->esquerda->valor);
  }
  else {
    printf("NULL - A direita: ");
  }
  if (node->direita) {
    printf("%d",node->direita->valor);
  }
  else {
    printf("NULL");
  }
  printf(" - Altura = %d\n",node->altura);
  if (valor_desejado == node->valor) {
    printf("ENCONTRADOOOOOO INDEX = %d\n",index);
  }
}