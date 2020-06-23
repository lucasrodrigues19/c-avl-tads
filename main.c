/*
 * main.c
 *
 *  Created on: 21 de jun de 2020
 *      Author: lucas.rodrigues
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct AVL {
	struct AVL *dir;
	char ele;
	struct AVL *esq;
	int balan;
} AVL;

void createAVL(AVL **avl) {

	*avl = NULL;
}
void setNewAVL(AVL **avl, char elemento) {

	(*avl)->ele = elemento;
	(*avl)->dir = NULL;
	(*avl)->esq = NULL;
	(*avl)->balan = 0;
}
int isEmptyAVL(AVL **avl) {
	return (avl == NULL);
}
void imprimirPreOrdemAVL(AVL **avl) {
	if (*avl != NULL) {
		printf(" %c ", (*avl)->ele);
		if ((*avl)->esq != NULL)
			imprimirPreOrdemAVL(&(*avl)->esq);
		if ((*avl)->dir != NULL)
			imprimirPreOrdemAVL(&(*avl)->dir);
	}
}
void imprimirInOrdemAVL(AVL **avl) {
	if (*avl != NULL) {

		if ((*avl)->esq != NULL)
			imprimirInOrdemAVL(&(*avl)->esq);

		printf(" %c ", (*avl)->ele);

		if ((*avl)->dir != NULL)
			imprimirInOrdemAVL(&(*avl)->dir);
	}
}
void imprimirPosOrdemAVL(AVL **avl) {
	if (*avl != NULL) {
		if ((*avl)->dir != NULL)
			imprimirPosOrdemAVL(&(*avl)->dir);
		if ((*avl)->esq != NULL)
			imprimirPosOrdemAVL(&(*avl)->esq);

		printf(" %c ", (*avl)->ele);
	}
}
AVL* inserirAVL(AVL **avl, char elemento) {
	if (*avl == NULL) {
		*avl = (AVL*) malloc(sizeof(AVL));
		setNewAVL(&(*avl), elemento);
	} else {
		AVL *avlTmp = (AVL*) malloc(sizeof(AVL));
		setNewAVL(&avlTmp, elemento);
		if (elemento < (*avl)->ele) {
			if ((*avl)->esq == NULL) { //se for uma folha
				(*avl)->esq = avlTmp;
			} else {
				(*avl)->esq = inserirAVL(&(*avl)->esq, elemento); //repasso a responsa para o no a esquerda
			}
		} else if (elemento > (*avl)->ele) {
			if ((*avl)->dir == NULL) {
				(*avl)->dir = avlTmp;
			} else {
				(*avl)->dir = inserirAVL(&(*avl)->dir, elemento);
			}
		}
	}
	return *avl;

}
AVL* removerElementoAVL(AVL **avl, char elemento) {
	// caso 1 - achei o elemento e ele nao contem sub-arvore
	if ((*avl)->ele == elemento) {
		if ((*avl)->dir == NULL && (*avl)->esq == NULL)
			return NULL;
		else {
			if ((*avl)->dir == NULL && (*avl)->esq != NULL) {
				return (*avl)->esq;
			} else if ((*avl)->dir != NULL && (*avl)->esq == NULL) {
				return (*avl)->dir;
			} else {
				//pego maior dentre os menores ou vice-versa
				AVL *tmp = (*avl)->dir;
				while (tmp->esq != NULL) {
					tmp = tmp->esq;
				}
				(*avl)->ele = tmp->ele;
				tmp->ele = elemento;
				(*avl)->dir = removerElementoAVL(&(*avl)->dir, elemento);
				free(tmp);
			}
		}
	} else {
		if ((*avl)->ele > elemento) { //caso 2 - o elemento é menor entao removo na esquerda
			if ((*avl)->esq != NULL)
				(*avl)->esq = removerElementoAVL(&(*avl)->esq, elemento);
		} else if ((*avl)->ele < elemento) { // caso 3  - o elemento é maior entoa removo na direita
			if ((*avl)->dir != NULL)
				(*avl)->dir = removerElementoAVL(&(*avl)->dir, elemento);
		}
	}
	return *avl;
}
int buscarElementoAVL(AVL **avl, char elemento) {
	if (*avl == NULL) {
		return 0;
	}
	if ((*avl)->ele == elemento) {
		return 1;
	} else {
		if ((*avl)->ele > elemento)
			return buscarElementoAVL(&(*avl)->esq, elemento);
		else if ((*avl)->ele < elemento)
			return buscarElementoAVL(&(*avl)->dir, elemento);

		return 0;
	}
}

int calcularAltura(AVL **avl) {
	if ((*avl)->dir == NULL && (*avl)->esq == NULL) { //Não tenho filho
		return 1;
	} else if ((*avl)->dir != NULL && (*avl)->esq == NULL) {
		return 1 + calcularAltura(&(*avl)->dir);
	} else if ((*avl)->dir == NULL && (*avl)->esq != NULL) {
		return 1 + calcularAltura(&(*avl)->esq);;
	} else if ((*avl)->dir != NULL && (*avl)->esq != NULL) {
		if (calcularAltura(&(*avl)->dir) > calcularAltura(&(*avl)->esq)) {
			return 1 + calcularAltura(&(*avl)->dir);
		} else {
			return 1 + calcularAltura(&(*avl)->esq);
		}
	}
}

void calcularBalanceamento(AVL **avl) {
	if ((*avl)->dir == NULL && (*avl)->esq == NULL) { //Não tenho filho
		(*avl)->balan = 0;
	} else if ((*avl)->dir != NULL && (*avl)->esq == NULL) {
		(*avl)->balan = calcularAltura(&(*avl)->dir) - 0;
	} else if ((*avl)->dir == NULL && (*avl)->esq != NULL) {
		(*avl)->balan = 0 - calcularAltura(&(*avl)->esq);
	} else {
		(*avl)->balan = calcularAltura(&(*avl)->dir)
				- calcularAltura(&(*avl)->esq);
	}
	if ((*avl)->dir != NULL)
		calcularBalanceamento(&(*avl)->dir);
	if ((*avl)->esq != NULL)
		calcularBalanceamento(&(*avl)->esq);

}

//rotacao simples
AVL * rotacaoSimplesDireita(AVL ** avl){
	AVL * filhoDir;
	AVL * filhoDoFilho = NULL;
	filhoDir = (*avl)->dir;
	if((*avl)->dir != NULL){//se meu filho a direita tem filhos a esquerda
		if((*avl)->dir->esq != NULL){
			filhoDoFilho = filhoDir->esq;
		}
	}
	filhoDir->esq = *avl;
	(*avl)->dir = filhoDoFilho;

	return filhoDir;
}
AVL * rotacaoSimplesEsquerda(AVL ** avl){
	AVL * filhoEsq;
		AVL * filhoDoFilho = NULL;
		filhoEsq = (*avl)->esq;
		if((*avl)->esq != NULL){//se meu filho a direita tem filhos a esquerda
			if((*avl)->esq->dir != NULL){
				filhoDoFilho = filhoEsq->dir;
			}
		}
		filhoEsq->dir = *avl;
		(*avl)->esq= filhoDoFilho;

		return filhoEsq;
}
//rotacao dupla
AVL * rotacaoDuplaDireita(AVL ** avl){
	AVL * tmpAvl = (*avl);
	AVL * filhoDir = (*avl)->dir;
	AVL * filhoDoFilho = filhoDir->esq;
	AVL * noInserido = filhoDoFilho->dir;
	//parte 1 - alinhar os caras
	filhoDir->esq = noInserido;
	filhoDoFilho->dir = filhoDir;
	(*avl)->dir = filhoDoFilho;
	//parte 2 -  tornar o filho a direita uma nova raiz
	AVL * novoFilhoDireita = (*avl)->dir;
	tmpAvl->dir = NULL;
	novoFilhoDireita->esq = tmpAvl;


	return novoFilhoDireita;

}
AVL * rotacaoDuplaEsquerda(AVL ** avl){
	AVL * tmpAvl = (*avl);
		AVL * filhoEsq = (*avl)->esq;
		AVL * filhoDoFilho = filhoEsq->dir;
		AVL * noInserido = filhoDoFilho->esq;
		//parte 1 - alinhar os caras
		filhoEsq->dir = noInserido;
		filhoDoFilho->esq = filhoEsq;
		(*avl)->esq = filhoDoFilho;
		//parte 2 -  tornar o filho a esquerda uma nova raiz
		AVL * novoFilhoEsquerda = (*avl)->esq;
		tmpAvl->esq = NULL;
		novoFilhoEsquerda->dir = tmpAvl;


		return novoFilhoEsquerda;
}



//verificaçao e rotacao


AVL* verificaBalanceamento(AVL **avl) {
	if ((*avl)->balan >= 2 || (*avl)->balan <= -2) { //se for (1) ou  (-1) nao acontece nada
		if ((*avl)->balan >= 2) { //vou mecher no lado direito
			if (((*avl)->balan * (*avl)->dir->balan) > 0) { //se for positivo vou tratar as coisas da direita
				return rotacaoSimplesDireita(&(*avl)); //inseri um novo no a direita na subarvore a direita
			} else { //trato as
				return rotacaoDuplaDireita(&(*avl)); //inseri um novo no a esquerda na subarvore a direita

			}
		} else if ((*avl)->balan <= -2) {
			if (((*avl)->balan * (*avl)->esq->balan) > 0) {
				return rotacaoSimplesEsquerda(&(*avl));
			} else {
				return rotacaoDuplaEsquerda(&(*avl));

			}
		}
	}
	calcularBalanceamento(&(*avl));
	if ((*avl)->esq != NULL)
		(*avl)->esq = verificaBalanceamento(&(*avl)->esq);
	if ((*avl)->dir != NULL)
		(*avl)->dir = verificaBalanceamento(&(*avl)->dir);
	return *avl;
}

main() {
	AVL *avl;
	int rsp;
	char str;
	createAVL(&avl);
	do{
		fflush(stdin);
		str = '\0';
		printf("\n-> ");
		scanf("%c",&str);
		if(str == '\0' || str == '\n')
			scanf("%c",&str);

		avl = inserirAVL(&avl,str);
		calcularBalanceamento(&avl);
		avl = verificaBalanceamento(&avl);
		calcularBalanceamento(&avl);
		printf("\nContinuar? [1 - sim | 0 - nao]: ");
		scanf("%d",&rsp);
	}while(rsp != 0);

	imprimirInOrdemAVL(&avl);
	printf("\n***Fim do programa");
	getchar();
}
