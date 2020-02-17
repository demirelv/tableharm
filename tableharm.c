#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "prime.h"

static inline long long get_prime_num(unsigned int n) {

   static long long *array = NULL;
   static unsigned int count = 0;
   long long num;
   long long i;
   if (count == 0) {
      array = malloc(sizeof(prime_list));
      count = sizeof(prime_list) / sizeof(prime_list[0]);
      memcpy(array, prime_list, sizeof(prime_list));
   }
   if (count > n) {
      return array[n];
   }
   num = array[count -1] + 2;
   do {
         int not_prime = 1;
         for (i = 0; i < count; i++) {
            if (num % get_prime_num(i) == 0) {
               not_prime = 0;
               break;
            } else if(get_prime_num(i) > num / 2)
               break;
         }
         if (not_prime) {
            count++;
            array = realloc(array, sizeof(long long) * count);
            array[count - 1] = num;
            printf("c=%u,%llu\n", count, num);
         } else {
            num += 2;
         }

   } while(count <= n);

   return array[n];
}

static inline void factor(unsigned long long a, unsigned int *count, unsigned int *difcount)
{
   unsigned long long prime;
   unsigned int i = 0;
   unsigned long long r = 0;
   *count = 0;
   *difcount = 0;
   prime = get_prime_num(i);
   do {
      if (a % prime == 0 ) {
         a = a / prime;
         (*count)++;
         if (r != prime)
            (*difcount)++;
         r = prime;
      } else {
         i++;
         prime = get_prime_num(i);
      }
   } while (1 != a && prime <= a / 2);
}

static inline void obeb(long long *a, long long *b)
{
   unsigned long long *min = *a < *b ? a : b;
   unsigned long long *max = *a < *b ? b : a;
   unsigned long long prime;
   unsigned long long compare = *min / 2;
   unsigned int i = 0;

   do {
      prime = get_prime_num(i);
      if (*min % prime == 0 && *max % prime == 0) {
         *min = *min / prime;
         *max = *max / prime;
         compare = *min / 2;
      } else {
         i++;
      }
      if (i%1000000 == 0)
         printf("prime= %llu\n", prime);
   } while (prime <= compare);
}

static inline unsigned long long harm(unsigned int n, unsigned long long *a, unsigned long long *b)
{
   static unsigned long long al[100];
   static unsigned long long bl[100];
   unsigned long long new = n;
   static int count = 0;
   if (n < 2) {
      *a = 1;
      *b = 1;
      return 0;
   } else if(count > n) {
      *a = al[n -1];
      *b = bl[n -1];
      return *a - *b;
   }

   harm(n-1, a, b);
   obeb(b, &new);
   *a = *a * new + *b;
   *b = *b * n;
   obeb(a,b);
   al[n - 1] = *a;
   bl[n - 1] = *b;
   count = n;
   return *a - *b;
}

int main(int argc, const char *argv[])
{
   int n = atoi(argv[1]);
   unsigned long long a, b, r;
   unsigned int count = 0, difcount = 0;
   int i = 3;

   for (i = 3; i < n; i++) {
      r = harm(i, &a, &b);
      factor(r, &count, &difcount);
      printf("%u  %u %s --- %llu %llu %llu\n", i, difcount, count == difcount ? "OK" : "FAIL",
                                                a, b, r);
   }
   return 0;
}