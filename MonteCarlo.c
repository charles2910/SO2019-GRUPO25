#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
// Define número de iterações
#define N_ITER 16384

int main()
{
	// Declaração de variaveis
	double 	x = 0.0000,
  			y = 0.0000,
			t = 0.0000;

	long i = 0;
	// Inicialização "randomica" do seed da função random()
	srandom(time(NULL));

	while (i < N_ITER) {
		// Geração aleatoria de um ponto (x, y)
		x = (double) (random() % 2000001) / 1000000.0000;
		x -= 1.00000;
		y = (double) (random() % 2000001) / 1000000.0000;
    	y -= 1.00000;
		// Checa se o ponto está dentro da circunferencia de raio 1
    	if ((pow(x, 2.000) + pow(y, 2.000)) <= 1.0000)
    		t += 1.0000;
		i++;
  	}
	// Calcula o Pi
	double pi = 4 *(double) (t / N_ITER);
	printf("%lf\n", pi);
	return 0;
}
