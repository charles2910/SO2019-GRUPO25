#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <pthread.h>

#define N_ITER 16384
#define N_THREADS 8

volatile double dentro;

pthread_mutex_t dentro_mutex;

void *montecarlo (void *k) {
	// Declaração de variaveis
	double 	x = 0.0000,
			y = 0.0000,
			t = 0.0000;

    long 	i = 0;

    while (i < N_ITER / N_THREADS) {
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
	// Atualiza a variável global que contem o num. de pontos dentro da circ.
	pthread_mutex_lock(&dentro_mutex);
	dentro += t;
	pthread_mutex_unlock(&dentro_mutex);
	return NULL;
}

int main()
{
	// Inicialização "randomica" do seed da função random()
	srandom(time(NULL));

	pthread_t threads[N_THREADS];
	pthread_attr_t attr;

	/* Inicialização do MUTEX */
	pthread_mutex_init(&dentro_mutex, NULL);


	/* Criação dos threads com JOINABLE setado */
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	for (int i=0; i < N_THREADS; i++) {
	  pthread_create(&threads[i], &attr, montecarlo, NULL);
	}

	/* Thread principal espera a conclusão dos outros threads */
	for (int i=0; i < N_THREADS; i++) {
	  pthread_join(threads[i], NULL);
	}

	/* Clean up and exit */
	pthread_attr_destroy(&attr);
	pthread_mutex_destroy(&dentro_mutex);
	// Calcula o Pi a partir da variável global que contem o num. de pontos
	// dentro da circ.
	double pi = 4.00 *(double) (dentro / N_ITER);
	printf("%lf\n", pi);
 	return 0;
}
