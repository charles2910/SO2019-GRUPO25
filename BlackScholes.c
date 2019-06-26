#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>


double maximo(double n1, double n2) {
    if(n1 > n2)
		return n1;
    else
		return n2;
}


int main(){
	// Inicialização segura de variáveis
	double 	S = 0.00000,
			E = 0.00000,
			r = 0.00000,
			sigma = 0.00000,
			T = 0.00000,
			t = 0.00000,
			*testes = NULL,
			media = 0.00000,
			var = 0.00000,
			stddev = 0.00000,
			intervalo = 0.00000,
			val_minimo = 0.00000,
			val_maximo = 0.00000;

			int 	M = 0,
					i = 0;
	// Setando seed para numeros randomicos
	srand(time(NULL));
	// Fazendo leitura dos valores iniciais
	scanf("%lf", &S);
	scanf("%lf", &E);
	scanf("%lf", &r);
	scanf("%lf", &sigma);
	scanf("%lf", &T);
	scanf("%d", &M);
	// Alocando memória para o vetor
	testes = malloc(sizeof(double)*M);

	for(;i < M; i++) {
		// Cálculo iterativo do valor da ação
    	t = S * exp((r - (sigma * sigma) / 2.000) * T + sigma * sqrt(T) * (rand() / (double) RAND_MAX));
    	testes[i] = exp((-r) * T) * maximo((t - E), 0.0000);
    	media += testes[i];
	}
	// Cáculo da média
	media = media / (double) M;
    printf("media = %lf\n", media);
	// Cálculo da variância
	for(i = 0 ; i < M; i++) {
    	var += (testes[i] - media) * (testes[i] - media);
	}
	var = var / (double) M - 1.000;
	// Cálculo do desvio padrão
	stddev = sqrt(var);
	// Cálculo do Z alfa/2 (intervalo de confiança)
	intervalo = (1.96 * stddev) / (double) sqrt(M);
	// Cálculo dos valores mínimos e máximos para o intervalo de confiança
	val_minimo = media - intervalo;
	val_maximo = media + intervalo;

	printf("S:        %.1lf\n", S);
	printf("E:        %.1lf\n", E);
	printf("r:        %.1lf\n", r);
	printf("sigma:    %.1lf\n", sigma);
	printf("T:        %.1lf\n", T);
	printf("M:        %d\n", M);


	printf("Confidence interval: (%lf, %lf)\n", val_minimo, val_maximo);
	free(testes);

	return 0;
}
