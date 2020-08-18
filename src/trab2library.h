void ajusta_limites(int indice, int tamanho, int tamanho_max, int* limite_inf, int* limite_sup);
int naborda(int indice, int limite, int tamanho);
int verifica_bordas(int i, int j, int largura, int altura, int tam_janela);
int calcula_valor(int soma, int elementos);
int get_value_r(imagem* data, int i, int j);
int get_value_g(imagem* data, int i, int j);
int get_value_b(imagem* data, int i, int j);