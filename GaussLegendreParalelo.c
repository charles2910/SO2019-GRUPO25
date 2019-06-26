#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

// Define número de iterações
#define N_ITER 16384

// Declaração de variáveis globais (threads, mutexes, variaveis de condição e contadores)
pthread_t threads[3];
volatile int     count_ab = 1, count_t = 0, count_ppi = 0;
pthread_mutex_t count_mutex_ab, count_mutex_t, count_mutex_ppi;
pthread_cond_t count_threshold_ab, count_threshold_t, count_threshold_ppi;

volatile long double an, an1, bn, bn1, tn, tn1, pn, pn1, pi, pi1;

void *thread1 (void *arg) {
	int ab = 0;
	// checa se thread precisa bloquear
	pthread_mutex_lock(&count_mutex_ab);
	while (!(ab < count_ab)) {
		pthread_cond_wait(&count_threshold_ab, &count_mutex_ab);
	}
	pthread_mutex_unlock(&count_mutex_ab);

	for (; ab < N_ITER ; ab++) {
		// lock em count_ab
		pthread_mutex_lock(&count_mutex_ab);
		// calcula próximos valores de an e bn
		an1 = (an + bn) / 2;
		bn1 = sqrt(an * bn);
		// lock em count_t
		pthread_mutex_lock(&count_mutex_t);
		// incrementa count_t
		count_t++;
		// sinaliza quem está esperando sinais na variavel de condição count_threshold_t
		pthread_cond_signal(&count_threshold_t);
		pthread_mutex_unlock(&count_mutex_t);
		// checa se precisa esperar
		while (!(ab < count_ab - 1)) {
			pthread_cond_wait(&count_threshold_ab, &count_mutex_ab);
		}
		pthread_mutex_unlock(&count_mutex_ab);
		// atualiza valores de an e bn
		an = an1;
		bn = bn1;
	}

	pthread_mutex_lock(&count_mutex_t);
	count_t++;
	pthread_cond_signal(&count_threshold_t);
	pthread_mutex_unlock(&count_mutex_t);
}

void *thread2 (void *arg) {
	int t = 0;
	pthread_mutex_lock(&count_mutex_t);
	while (!(t < count_t)) {
		pthread_cond_wait(&count_threshold_t, &count_mutex_t);
	}
	pthread_mutex_unlock(&count_mutex_t);

	for (; t < N_ITER; t++) {
		pthread_mutex_lock(&count_mutex_t);

		tn1 = tn - pn * (an - an1) * (an - an1);

		pthread_mutex_lock(&count_mutex_ppi);
		count_ppi++;

		pthread_cond_signal(&count_threshold_ppi);
		pthread_mutex_unlock(&count_mutex_ppi);

		while (!(t < count_t - 1)) {
			pthread_cond_wait(&count_threshold_t, &count_mutex_t);
		}
		pthread_mutex_unlock(&count_mutex_t);

		tn = tn1;
	}
	pthread_mutex_lock(&count_mutex_ppi);
	count_ppi++;
	pthread_cond_signal(&count_threshold_ppi);
	pthread_mutex_unlock(&count_mutex_ppi);
}

void *thread3 (void *arg) {
	int ppi = 0;
	pthread_mutex_lock(&count_mutex_ppi);
	while (!(ppi < count_ppi)) {
		pthread_cond_wait(&count_threshold_ppi, &count_mutex_ppi);
	}
	pthread_mutex_unlock(&count_mutex_ppi);

	for (; ppi < N_ITER; ppi++) {
		pthread_mutex_lock(&count_mutex_ppi);

		pn1 = 2.0000 * pn;
		pi1 = ((an1 + bn1) * (an1 + bn1)) / (4.000 * tn1);

		pthread_mutex_lock(&count_mutex_ab);
		count_ab++;

		pthread_cond_signal(&count_threshold_ab);
		pthread_mutex_unlock(&count_mutex_ab);

		while (!(ppi < count_ppi - 1)) {
			pthread_cond_wait(&count_threshold_ppi, &count_mutex_ppi);
		}
		pthread_mutex_unlock(&count_mutex_ppi);

		pn = pn1;
		pi = pi1;
	}
	pthread_mutex_lock(&count_mutex_ab);
	count_ab++;
	pthread_cond_signal(&count_threshold_ab);
	pthread_mutex_unlock(&count_mutex_ab);
}

int main () {
	// inicialização de variáveis
    pi1 = 0.000;
	an = 1.0000;
	bn = 1.0000 / sqrtl(2.0000);
	tn = 1.0000/ 4.0000;
	pn = 1.000000;
	pthread_attr_t attr;

	/* Inicialização do MUTEX e das variáveis de condição*/
	pthread_mutex_init(&count_mutex_ab, NULL);
	pthread_cond_init (&count_threshold_ab, NULL);
	pthread_mutex_init(&count_mutex_t, NULL);
	pthread_cond_init (&count_threshold_t, NULL);
	pthread_mutex_init(&count_mutex_ppi, NULL);
	pthread_cond_init (&count_threshold_ppi, NULL);

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
   	pthread_mutex_destroy(&count_mutex_ab);
	pthread_mutex_destroy(&count_mutex_t);
	pthread_mutex_destroy(&count_mutex_ppi);
   	pthread_cond_destroy(&count_threshold_ab);
	pthread_cond_destroy(&count_threshold_t);
	pthread_cond_destroy(&count_threshold_ppi);
	printf("%Lf\n", pi1);
   	pthread_exit(NULL);
	return 0;
}
