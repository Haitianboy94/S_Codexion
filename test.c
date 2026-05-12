#include "codexion.h"

void *funct1(void *arg)
{
	int i;

	i = 0;
	while (true)
	{
		printf("\033[91mthread[%d]\033[0m\n", i);
		i++;
		sleep(1);
		if (i == 30)
		{
			printf("thread stop at [%d]\n", i);
			return NULL;
		}
	}
	return NULL;
}

void *funct2(void *arg)
{
	int i;

	i = 0;
	while (true)
	{
		printf("\033[95mcoder[%d]\033[0mis compiling\n", i);
		i++;
		sleep(1);
		if (i == 30)
		{
			printf("Coder[%d] has a burn out\n", i);
			return NULL;
		}
	}
	return NULL;
}

int main()
{
	pthread_t t1;
	pthread_t t2;

	pthread_create(&t1, NULL, funct1, NULL);
	pthread_create(&t2, NULL, funct2, NULL);
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
}