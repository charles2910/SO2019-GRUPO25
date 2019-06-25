#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main()
{
  double x, y, t = 0.0000;
  long int i = 0, max = 16384;
  srandom(2);

  while (i < max) {
     x = (double) (random() % 2000001) / 1000000.0000;
     x -= 1.00000;
     y = (double) (random() % 2000001) / 1000000.0000;
     y -= 1.00000;
     if ((pow(x, 2.000) + pow(y, 2.000)) <= 1.0000)
       t += 1.0000;
     //printf("(%f, %f) => r = %f\n", x, y, (pow(x, 2.000) + pow(y, 2)));
     i++;
  }
  //printf ("t is %lf\n", t);
  double pi = 4 *(double) (t / max);
  printf("%lf\n", pi);
  return 0;
}
