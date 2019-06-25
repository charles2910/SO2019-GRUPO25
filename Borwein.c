#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main () {
	//inicialização de valores
	int n = 16384, i = 0;
	long double an, bn, pn, an1, bn1, pn1;

	an = sqrtl(2.0000000000);
	bn = 0.0000000000;
	pn = 2.000000 + an;

	while (i < n) {
		an1 =  (sqrtl(an) + 1.000000 / sqrtl(an)) / 2.000000 ;
		bn1 = ((1.00000000000000 + bn) * sqrtl(an)) / (an + bn);
		pn1 = ((1.0000000 + an1) * pn * bn1) / (1.0000000 + bn1);

		an = an1;
		bn = bn1;
		pn = pn1;

		i++;

	}
	//printf("Iterações: %d\nan: %Lf\nbn:%Lf\npn:%Lf\n",i, an, bn, pn);
	printf("%Lf\n", i, pn1);
	return 0;

}
