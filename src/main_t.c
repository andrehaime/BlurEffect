#include "imageprocessing.h"
#include "trab2library.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <math.h> 
#include <pthread.h>

#define n_threads 4

typedef struct{
  int idz, blur;
  imagem* img_pointer;
} parametros_thread;

void* funcao_thread(void *arg);
pthread_mutex_t trava;

int main(int argc, char *argv[]) {
  // limites
  int lim_inf_i, lim_sup_i, lim_inf_j, lim_sup_j, I_max =0, I_min =0;
  //indices
  int k=0,l=0, z=0, idx =0, i=0;
  /*variaveis auxiliares */
  int acumulador_r =0, acumulador_g =0, acumulador_b =0;
  int area_janela =0;
  /* parametros */
  int N = 5, W_max = 100;
  float somador = 0, tempo_total =0, desvio_padrao =0, tempo_medio =0;
  /* variaveis para tempo*/
  long seconds =0 ,useconds =0;
  double time_total;
  float *standard_deviation;
  // imagem 
  imagem img;
  /* Declara um vetor de threads*/
  pthread_t threads[n_threads];
  // cria uma struct do tipo parametros_thread
  // para ser entrada na funcao thread
  parametros_thread parametros;
  /* Tempo baseada em tempo real */
  struct timeval rt0, rt1, drt; 

  // tira os dados de N e W_max da entrada
  if (argc > 0){
    N = atoi(argv[1]);
    W_max = atoi(argv[2]);
  }

  // adiciona o parametro N na struct
  parametros.blur = N;

   // faz o vetor pra armazenar o desvio padrao
  standard_deviation = malloc(W_max * sizeof(float)); 

  // laço principal
  for (int w =0; w<W_max; w++){
    // abre a imagem
    img = abrir_imagem("data/cachorro.jpg");
    //armazena um ponteiro pra imagem como parametro
    parametros.img_pointer = &img;
    // captura o tempo atual
    gettimeofday(&rt0, NULL);
    //define z =0
    z = 0;
    // criei a thread em while e não em for como das outras
    // vezes, pois foi o jeito que encontrei para não ocorrer
    // compartilhamento indevido da memória
    while(1){
      // Cria thread
      pthread_mutex_lock(&trava);
      // insere o parametro z na struct
      parametros.idz = z;
      // a ideia aqui é criar um numero n de threads e passar pra cada uma
      // o parametro z que indica a partir de qual "zona" aquela thread vai
      // trabalhar
      pthread_create(&(threads[z]), NULL, funcao_thread, &(parametros));
      if (z == n_threads -1) break;
      z++;
    } 

    // espera sair da thread
    for (k=0; k<n_threads; k++) pthread_join(threads[k], NULL);
    // dá free na imagem
    if (w!=W_max-1) liberar_imagem(&img);
    //printf("Progresso %d % blur_value %d \n", 100*(w+1)/W_max, N);
    gettimeofday(&rt1, NULL);
    timersub(&rt1, &rt0, &drt);
    // calcula o tempo
    seconds = drt.tv_sec;
    useconds = drt.tv_usec;
    time_total = seconds + useconds*1e-6;
    standard_deviation[w] = time_total;

  }
  //calcula a média
  for(i=0; i<W_max; i++){
    somador+= standard_deviation[i];
  }
  tempo_medio = somador/W_max;
  somador =0;

  //calcula o desvio padrão
  for (i=0; i<W_max; i++){
    somador += (standard_deviation[i] - tempo_medio)*(standard_deviation[i] - tempo_medio);
  }
  desvio_padrao = sqrt(somador/W_max);

  // salva a imagem
  salvar_imagem("cachorro-out.jpg", &img);
  // dá free na imagem
  liberar_imagem(&img);
  // coloca na saída padrão em formato csv
  printf("%d, %f, %f \n",N , tempo_medio, desvio_padrao);

  return 0;
}

void* funcao_thread(void *arg){
  // recebe os parametros e transforma de novo pra uma
  // struct mas aqui com o escopo interno
  parametros_thread* aux =  arg;
  parametros_thread parametros = (*aux);

  int I_max, I_min, lim_inf_i, lim_sup_i;
  int idz =0, k=0, l=0;
  int idx, lim_inf_j, lim_sup_j;
  int acumulador_r, acumulador_g, acumulador_b;
  int N = parametros.blur;
  long area_janela=0;
  imagem img = *parametros.img_pointer;
  idz = parametros.idz;
  // libera o mutex após z ser passado pra dentro da funcao
  pthread_mutex_unlock(&trava);

  // calcula os limites maximos e minimos para execucao
  // da funcao 
  I_max = (idz+1)*(img.width)/n_threads;
  I_min = idz*(img.width)/n_threads;
  for (int i=I_min; i<I_max+1; i++) {
    /*-------- Ajusta limites ----------*/
    //printf("Imin %d \n",i);
    ajusta_limites(i, N, img.width, &lim_inf_i, &lim_sup_i);

    for (int j=0; j<(img.height); j++) {
      acumulador_r = 0;
      acumulador_g = 0;
      acumulador_b = 0;

      // /*-------- Ajusta limites ----------*/
      ajusta_limites(j, N, img.height, &lim_inf_j, &lim_sup_j);
      // calcula a area da janela de dados
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

    }
  }
  return NULL;
}
