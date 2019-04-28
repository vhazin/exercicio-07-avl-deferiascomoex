#include <stdio.h>
#include <stdlib.h>

struct no {
  int valor;
  struct no* esquerda;
  struct no* direita;
  int altura;
  int filhos_a_esquerda;
  int filhos_a_direita;
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

/*
//Descrever
struct no* arvore_in_ordem2(struct no* node, int valor, int valor_desejado); // nao mais usado
void descreveFolha(struct no* node, int index, int valor_desejado); // usado pra debugar
//
*/

//Procurar
int procurar(struct no* node, int valor);
//

unsigned short int flag = 0; // Variavel global responsavel por indicar existencia de um valor na arvore

int main() {
  int instrucoes;
  int query;
  int valor_passado;
  struct no* raiz = (struct no*) malloc (sizeof(struct no));
  if (!raiz) {
    printf("Error while allocating node \"raiz\"\n");
    exit(-1);
  }
  raiz = NULL;
  scanf("%d",&instrucoes);
  scanf("%d %d",&query,&valor_passado);
  raiz = criarFolha(valor_passado);
  instrucoes--;
  for (int i=0; i<instrucoes; i++) {
    flag = 0;
    scanf("%d %d",&query,&valor_passado);
    if (query == 1) { 
      //codigo para inserir na arvore
      raiz = inserirAVL(valor_passado, raiz);
    }
    else {
      //codigo para procurar na arvore
      int index = procurar(raiz, valor_passado);
      if (flag) {
        printf("Data tidak ada\n"); //flag significa que o valor nao foi encontrado
      }
      else {
        printf("%d\n",index + 1); // based-1 index
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
  node->altura = 1;
  // uma nova folha ainda nao tem filhos nem a esquerda nem a direita
  node->esquerda = NULL;
  node->filhos_a_esquerda = 0;
  node->direita = NULL;
  node->filhos_a_direita = 0; 
  return node;
}

void atualizaAltura(struct no* node) {
  int altura_direita;
  int altura_esquerda;
  if (node) {
    altura_direita = altura(node->direita);
    altura_esquerda = altura(node->esquerda);
    node->altura = maximo(altura_direita, altura_esquerda) + 1; // a altura de um no é igual ao maior caminho dele até uma folha
  }
}

struct no* inserirAVL(int valor, struct no* node) {
  if (!node) {
    return criarFolha(valor); // insere folha
  }
  else {
    if (valor < node->valor) {
      node->filhos_a_esquerda++; // no atual ganhou mais um filho a sua esquerda
      node->esquerda = inserirAVL(valor, node->esquerda);
      if (altura(node->esquerda) - altura(node->direita) == 2) {
        return valor < node->esquerda->valor ? rotacaoSimplesDireita(node) : rotacaoDuplaDireita(node);
      }
    }
    else {
      node->filhos_a_direita++; // no atual ganhou mais um filho a sua direita
      node->direita = inserirAVL(valor, node->direita);
      if (altura(node->esquerda) - altura(node->direita) == -2) {
        return valor > node->direita->valor ? rotacaoSimplesEsquerda(node) : rotacaoDuplaEsquerda(node);
      }
    }
    atualizaAltura(node); // no adicionado -> precisa-se atualizar valor de altura
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
  // o numero de filhos que um no "Node" tem a direita é igual a:
  // o numero de filhos que o no a direita tem a sua esquerda
  // + o numero de filhos que o no a direita tem a sua direita
  // + o proprio no diretamente a direita de "Node"
  // Se "Node" não tem ninguem a sua direita, o numero de filhos que ele tem a direita é 0
  node->filhos_a_direita = node->direita ? node->direita->filhos_a_esquerda + node->direita->filhos_a_direita + 1 : 0;
  // É garantido que auxiliar tem filhos a esquerda, logo, a verificação não é necessária
  auxiliar->filhos_a_esquerda = node->filhos_a_esquerda + node->filhos_a_direita + 1;
  return auxiliar;
}

struct no* rotacaoSimplesDireita(struct no* node) {
  struct no* auxiliar = (struct no*) malloc(sizeof(struct no));
  auxiliar = node->esquerda; // x
  node->esquerda = auxiliar->direita; // T2
  auxiliar->direita = node;
  atualizaAltura(node);
  atualizaAltura(auxiliar);
  // Explicacao analoga a rotacaoSimplesEsquerda
  node->filhos_a_esquerda = node->esquerda ? node->esquerda->filhos_a_esquerda + node->esquerda->filhos_a_direita + 1 : 0;
  auxiliar->filhos_a_direita = node->filhos_a_esquerda + node->filhos_a_direita + 1;
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

/*
struct no* arvore_in_ordem2(struct no* node, int valor, int valor_desejado) {
  if (!node) {
    return NULL;
  }
    arvore_in_ordem2(node->esquerda, valor, valor_desejado);
    myindex = myindex + 1;
    if (node->valor == valor) {
      flag = 1;
      printf("%d\n",myindex);
      return NULL;
    }
    arvore_in_ordem2(node->direita, valor, valor_desejado);
    return NULL;
}
*/

int procurar (struct no* node, int valor_desejado) {
	if (node) {
		if (node->valor < valor_desejado) {
      // se o no atual tem valor menor que o valor desejado, significa que o valor so pode estar a sua direita
      // logo, o index (quantidade de filhos) é igual a quantidade total de filhos a sua esquerda + ele mesmo + (procurar a sua direita)
      return node->filhos_a_esquerda + 1 + procurar(node->direita, valor_desejado);
    }
		else if (node->valor > valor_desejado) {
      // se o no atual tem valor maior que o valor desejado, precisa-se procurar apenas a sua esquerda
      return procurar(node->esquerda, valor_desejado);
    } 
		else {
      // se o no atual tem o valor igual ao valor desejado, para saber seu index basta que encontre-se os valores menor que ele mesmo
      return node -> filhos_a_esquerda;
    }
	} 
  else {
    // se chegou-se ao fim e nao encontrou-se mais nenhuma folha, significa que o valor desejado nao encontra-se na arvore
    // logo, a flag é levantada
		flag = 1;
		return 0;
	}
}