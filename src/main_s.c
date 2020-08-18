#include <time.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include "imageprocessing.h"
#include "trab2library.h"
#include <math.h>


int main(int argc, char *argv[]) {
  //limites
  int lim_inf_i, lim_sup_i, lim_inf_j, lim_sup_j;
  //indices
  int k=0,l=0, idx =0, i=0;
  // variaveis auxiliares 
  int acumulador_r =0, acumulador_g =0, acumulador_b =0, area_janela =0;
  /* parametros */
  int N = 5 , W_max = 100;
  imagem img; 
  struct timeval rt0, rt1, drt; /* Tempo baseada em tempo real */
  float somador = 0, tempo_total =0, desvio_padrao =0, tempo_medio =0;
  float *standard_deviation; 
  long seconds =0 ,useconds =0;
  double time_total;

  // tira os dados de N e W_max da entrada
  if (argc > 0){
    N = atoi(argv[1]);
    W_max = atoi(argv[2]);
  }
  // faz o vetor pra armazenar o desvio padrao
  standard_deviation = malloc(W_max * sizeof(float));

  for (int w =0; w<W_max; w++){

    img = abrir_imagem("data/cachorro.jpg");

    // Pega o primeiro tempo
    gettimeofday(&rt0, NULL); 
    for (int i=0; i<(img.width); i++) {
      /*-------- Ajusta limites ----------*/
      ajusta_limites(i, N, img.width, &lim_inf_i, &lim_sup_i);

      for (int j=0; j<(img.height); j++) {

        // /*-------- Ajusta limites ----------*/
        ajusta_limites(j, N, img.height, &lim_inf_j, &lim_sup_j);

        // Calcula o tamanho da janela
        area_janela = (lim_sup_i -lim_inf_i) * (lim_sup_j - lim_inf_j);
        // Calcula o indice do vetor correspondente ao elemento ij
        idx = j*img.width + i;
        //---------------------------------------
        // soma os valores em torno de i e j em r g b separadamente
        for (k=lim_inf_i; k<lim_sup_i; k++){ 
          for (l=lim_inf_j; l<lim_sup_j; l++){
            acumulador_r += get_value_r(&img, k , l);
            acumulador_g += get_value_g(&img, k , l);
            acumulador_b += get_value_b(&img, k , l);
          }
        }
        // Divide o numero em acumulador pelo numero total de itens em torno do
        // item de interesse
        img.r[idx] = calcula_valor(acumulador_r, area_janela);
        img.g[idx] = calcula_valor(acumulador_g, area_janela);
        img.b[idx] = calcula_valor(acumulador_b, area_janela);
        acumulador_r = 0;
        acumulador_g = 0;
        acumulador_b = 0;
      }
    }
    // libera a imagem exceto a ultima que vai ser usada para
    // salvar a imagem.
    if (w!=W_max-1) liberar_imagem(&img);
    //printf("Progresso %d % blur_value %d \n", 100*(w+1)/W_max, N);
    gettimeofday(&rt1, NULL);
    timersub(&rt1, &rt0, &drt);
    // calculo do tempo do laço
    seconds = drt.tv_sec;
    useconds = drt.tv_usec;
    time_total = seconds + useconds*1e-6;
    standard_deviation[w] = time_total;
  }
  // calcula a média dos tempos;
  for(i=0; i<W_max; i++){
    somador+= standard_deviation[i];
  }
  tempo_medio = somador/W_max;
  somador =0;

  // calcula o desvio padrao
  for (i=0; i<W_max; i++){
    somador += (standard_deviation[i] - tempo_medio)*(standard_deviation[i] - tempo_medio);
  }
  desvio_padrao = sqrt(somador/W_max);

  // salva a imagem
  salvar_imagem("cachorro-out.jpg", &img);
  liberar_imagem(&img);
  free(standard_deviation);

  // coloca na saída padrão em formato csv
  printf("%d, %f, %f \n",N , tempo_medio, desvio_padrao);
  return 0;
}


