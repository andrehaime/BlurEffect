#include "imageprocessing.h"

void ajusta_limites(int indice, int tamanho, int tamanho_max, int* limite_inf, int* limite_sup){
	/*-------- Ajusta limites ----------*/
    // verifica o limite inferior de i para o for do blur
    if (indice-tamanho < 0) *limite_inf = 0;
    else *limite_inf = indice-tamanho;
      
    // verifica o limite superior de i para o for do blur
    if (indice+tamanho >= tamanho_max) *limite_sup = tamanho_max;
    else *limite_sup = indice+tamanho;

    return;
}
int naborda(int indice, int limite, int tamanho){
  //funcao para retornar 1 caso o bloco em torno de indice esteja
  // numa borda delimitada por 0 e limite
  if (indice-tamanho < 0) return 1;
  else if(indice+tamanho>limite) return 1; 
  else return 0;
}

int verifica_bordas(int i, int j, int largura, int altura, int tam_janela){
  //funcao para retornar 1 se estiver em borda e 0 caso contrario
  return naborda(i, largura, tam_janela) ||  naborda(j, altura, tam_janela);
}

int calcula_valor(int soma, int elementos){
  soma = soma/elementos;  
  if (soma > 255) soma=255;
  return soma;
}

int get_value_r(imagem* data, int i, int j){
  //printf("%d\n", data->r[j*data->width + i]);
  return data->r[j*data->width + i];
}

int get_value_g(imagem* data, int i, int j){
  //printf("%d\n", data->r[j*data->width + i]);
  return data->g[j*data->width + i];
}

int get_value_b(imagem* data, int i, int j){
  //printf("%d\n", data->r[j*data->width + i]);
  return data->b[j*data->width + i];
}
