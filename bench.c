#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <time.h>

#define N 1000000

/* empeche le compilateur d'eliminer les comparaisons */
static volatile int sink;

static char *random_word(void)
{
	int len = 8 + rand() % 8;
	char *s = malloc(len + 1);
	for (int i = 0; i < len; i++)
		s[i] = 'a' + rand() % 26;
	s[len] = '\0';
	return s;
}

static char *random_case(const char *w)
{
	char *s = strdup(w);
	for (char *p = s; *p; p++)
		if (rand() % 2 == 0)
			*p -= 'a' - 'A';
	return s;
}

static double now_ms(void)
{
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	return ts.tv_sec * 1000.0 + ts.tv_nsec / 1e6;
}

static double bench_strcasecmp(char **as, char **bs)
{
	double start = now_ms();
	for (int i = 0; i < N; i++)
		sink = strcasecmp(as[i], bs[i]) == 0;
	return now_ms() - start;
}

/* comparaison ASCII insensible a la casse, octet par octet, sans SIMD.
 * En ASCII, une majuscule et sa minuscule ne different que par le bit 0x20.
 * La minuscule est calculee sans branchement : (c - 'A') < 26 vaut 0 ou 1,
 * decale de 5 bits il donne 0 ou 0x20. Aucun if ne depend de la casse,
 * donc aucune erreur de prediction de branchement. */
static inline int ascii_equal_fold(const char *a, const char *b)
{
	for (;; a++, b++) {
		unsigned char ca = *a, cb = *b;
		unsigned char fa = ca | (((unsigned)(ca - 'A') < 26u) << 5);
		unsigned char fb = cb | (((unsigned)(cb - 'A') < 26u) << 5);
		if (fa != fb)
			return 0;
		if (ca == '\0')
			return 1;
	}
}

static double bench_ascii(char **as, char **bs)
{
	double start = now_ms();
	for (int i = 0; i < N; i++)
		sink = ascii_equal_fold(as[i], bs[i]);
	return now_ms() - start;
}

static void show(const char *name, double ms)
{
	char buf[32];
	snprintf(buf, sizeof(buf), "%.2f", ms);
	char *dot = strchr(buf, '.');
	if (dot)
		*dot = ',';
	printf("%s : %s ms\n", name, buf);
}

int main(void)
{
	char **as = malloc(N * sizeof(char *));
	char **bs = malloc(N * sizeof(char *));

	srand(42);
	for (int i = 0; i < N; i++) {
		char *w = random_word();
		as[i] = random_case(w);
		if (rand() % 2 == 0) {
			bs[i] = random_case(w); /* meme mot, autre casse */
		} else {
			char *w2 = random_word();
			bs[i] = random_case(w2);
			free(w2);
		}
		free(w);
	}

	printf("C :\n");
	show("strcasecmp(a, b)", bench_strcasecmp(as, bs));
	show("ascii_equal_fold(a, b)", bench_ascii(as, bs));
	return 0;
}
