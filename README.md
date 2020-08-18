# Trabalho 2
## Autor: André Luis Haime Coutinho
## RA: 166329

O objetivo deste trabalho foi contruir um filtro de desfoque, com diferentes implementações: 
* Linha de execução única
* Processos
* Threads

## Como testar o programa:
Esse programa contem diferentes instruções que podem ser usadas para facilidade:

* $ make:
	Executa todos os testes e todas as funções e gera o arquivo do doguinho na pasta principal e o arquivo PNG correspondente ao gráfico comparando as 3 implementações
* $ make main_t
	Compila a versão em thread
* $ make main_p
	Compila a versão em processo
* $ make main_s
	Compila a versão em linha única

A partir da execução de um desses makes, o programa poderá ser executado como:
$ ./main_t N W
Onde N é o tamanho do filtro blur e W é o número de repetições que o programa deverá fazer.

## Comandos úteis:

* $ make clean:
Remove os arquivos temporários e mantém os essenciais.
* $ make test_t ou test_p ou test_s:
Cada um dos test_X executa 100 repetições do programa de N = 0 até N = 100 e salva um arquivo em CSV (out_X.o) com os dados dos tempos de execução na pasta /doc. Esse arquivo é aberto por um script em python que cria um gráfico para cada um dos testes. Para testes rápidos recomenda-se alterar o valor de W (que é o valor que fica na posicao do 100 no exemplo abaixo) para algo entre 2 ~ 5 no código test.sh:

	for i in $(seq 1 10)
	do 
  		$1 $i 100
	done 




