#include "codexion.h"

void *funct1(void *arg)
{
	int i;
	for(i = 0; i <= 10; i++)
	{
		printf("\033[91mthread[%d]\033[0m\n", i);
		sleep(1);
	}

	return NULL;
}

void *funct2(void *arg)
{
	int i;
	for(i = 0; i <= 10; i++)
	{
		printf("\033[95mCoder[%d]\033[0m\n", i);
		sleep(1);
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