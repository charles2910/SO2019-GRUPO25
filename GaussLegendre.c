#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Define número de iterações
#define N_ITER 16384

int main () {
	//inicialização de valores
	int i = 0;
	long double an, bn, tn, pn, an1, bn1, tn1, pn1, pi, pi1;
    pi1 = 0.000;
	an = 1.0000;
	bn = 1.0000 / sqrtl(2.0000);
	tn = 1.0000/ 4.0000;
	pn = 1.0000;

	while (i < N_ITER) {
		an1 =  (an + bn) / 2;
		bn1 = sqrtl(an * bn);
		tn1 = tn - pn * (an - an1) * (an - an1);
		pn1 = 2.0000 * pn;
		pi1 = ((an1 + bn1) * (an1 + bn1)) / (4.000 * tn1);

		an = an1;
		bn = bn1;
		pn = pn1;
		tn = tn1;
		pi = pi1;

		i++;
	}
	printf("%Lf\n", pi1);
	return 0;
}
