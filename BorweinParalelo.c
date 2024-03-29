#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

// Define número de iterações
#define N_ITER 16384

// Declaração de variáveis globais (threads, mutexes, variaveis de condição e contadores)
pthread_t threads[3];
volatile int     count_a = 1, count_b = 0, count_p = 0;
pthread_mutex_t count_mutex_a, count_mutex_b, count_mutex_p;
pthread_cond_t count_threshold_a, count_threshold_b, count_threshold_p;

volatile long double an, an1, bn, bn1, pn, pn1;

void *thread1 (void *arg) {
	int a = 0;
	// checa se thread precisa bloquear
	pthread_mutex_lock(&count_mutex_a);
	while (!(a < count_a)) {
		pthread_cond_wait(&count_threshold_a, &count_mutex_a);
	}
	pthread_mutex_unlock(&count_mutex_a);

	for (; a < N_ITER ; a++) {
		// lock em count_a
		pthread_mutex_lock(&count_mutex_a);
		// calcula próximos valores de an
		an1 =  (sqrtl(an) + 1.000000 / sqrtl(an)) / 2.000000 ;
		// lock em count_b
		pthread_mutex_lock(&count_mutex_b);
		// incrementa count_b
		count_b++;
		// sinaliza quem está esperando sinais na variavel de condição count_threshold_b
		pthread_cond_signal(&count_threshold_b);
		pthread_mutex_unlock(&count_mutex_b);
		// checa se precisa esperar
		while (!(a < count_a - 1)) {
			pthread_cond_wait(&count_threshold_a, &count_mutex_a);
		}
		pthread_mutex_unlock(&count_mutex_a);
		// atualiza valores de an
		an = an1;
	}

	pthread_mutex_lock(&count_mutex_b);
	count_b++;
	pthread_cond_signal(&count_threshold_b);
	pthread_mutex_unlock(&count_mutex_b);
}

void *thread2 (void *arg) {
	int b = 0;
	pthread_mutex_lock(&count_mutex_b);
	while (!(b < count_b)) {
		pthread_cond_wait(&count_threshold_b, &count_mutex_b);
	}
	pthread_mutex_unlock(&count_mutex_b);

	for (; b < N_ITER; b++) {
		pthread_mutex_lock(&count_mutex_b);

		bn1 = ((1.00000000000000 + bn) * sqrtl(an)) / (an + bn);

		pthread_mutex_lock(&count_mutex_p);
		count_p++;

		pthread_cond_signal(&count_threshold_p);
		pthread_mutex_unlock(&count_mutex_p);

		while (!(b < count_b - 1)) {
			pthread_cond_wait(&count_threshold_b, &count_mutex_b);
		}
		pthread_mutex_unlock(&count_mutex_b);

		bn = bn1;
	}
	pthread_mutex_lock(&count_mutex_p);
	count_p++;
	pthread_cond_signal(&count_threshold_p);
	pthread_mutex_unlock(&count_mutex_p);
}

void *thread3 (void *arg) {
	int p = 0;
	pthread_mutex_lock(&count_mutex_p);
	while (!(p < count_p)) {
		pthread_cond_wait(&count_threshold_p, &count_mutex_p);
	}
	pthread_mutex_unlock(&count_mutex_p);

	for (; p < N_ITER; p++) {
		pthread_mutex_lock(&count_mutex_p);

		pn1 = ((1.0000000 + an1) * pn * bn1) / (1.0000000 + bn1);

		pthread_mutex_lock(&count_mutex_a);
		count_a++;

		pthread_cond_signal(&count_threshold_a);
		pthread_mutex_unlock(&count_mutex_a);

		while (!(p < count_p - 1)) {
			pthread_cond_wait(&count_threshold_p, &count_mutex_p);
		}
		pthread_mutex_unlock(&count_mutex_p);

		pn = pn1;
	}
	printf("%Lf\n", pn);
	pthread_mutex_lock(&count_mutex_a);
	count_a++;
	pthread_cond_signal(&count_threshold_a);
	pthread_mutex_unlock(&count_mutex_a);
}

int main () {
	an = sqrtl(2.0000000000);
	bn = 0.0000000000;
	pn = 2.000000 + an;

	pthread_attr_t attr;

	/* Inicialização do MUTEX e das variáveis de condição*/
	pthread_mutex_init(&count_mutex_a, NULL);
	pthread_cond_init (&count_threshold_a, NULL);
	pthread_mutex_init(&count_mutex_b, NULL);
	pthread_cond_init (&count_threshold_b, NULL);
	pthread_mutex_init(&count_mutex_p, NULL);
	pthread_cond_init (&count_threshold_p, NULL);

	/* Criação dos threads com JOINABLE setado */
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	pthread_create(&threads[2], &attr, thread3, NULL);
	pthread_create(&threads[1], &attr, thread2, NULL);
	pthread_create(&threads[0], &attr, thread1, NULL);

	/* Thread principal espera a conclusão dos outros threads */
   	for (int i = 0; i < 3; i++) {
     	pthread_join(threads[i], NULL);
   	}

   	/* Clean up and exit */
   	pthread_attr_destroy(&attr);
   	pthread_mutex_destroy(&count_mutex_a);
	pthread_mutex_destroy(&count_mutex_b);
	pthread_mutex_destroy(&count_mutex_p);
   	pthread_cond_destroy(&count_threshold_a);
	pthread_cond_destroy(&count_threshold_b);
	pthread_cond_destroy(&count_threshold_p);
   	pthread_exit(NULL);
	return 0;

}
