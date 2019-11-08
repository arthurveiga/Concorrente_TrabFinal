// autor: Arthur da Veiga Feitoza Borges
//        Leticia de Souza Soares
// arquivo: banheiro_unissex.c
// Trabalho de Prog. Concorrente 2019-2

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <time.h> //para srand

int num_banheiros = 1, num_compartimentos = 1, num_generos = 1, num_pessoas = 100;

void input_test()
{
    printf("Digite a quantidade de banheiros: ");
    scanf("%d", &num_banheiros);
    printf("Digite a quantidade de compartimentos: ");
    scanf("%d", &num_compartimentos);
    printf("Digite a quantidade de generos: ");
    scanf("%d", &num_generos);
    printf("Digite a quantidade de pessoas: ");
    scanf("%d", &num_pessoas);
};

void *pessoa(void *id)
{
    
};

int main()
{
    /*Como gerar valores aleatoriamente: exemplo: 1 + rand() % (100); 1 ate 100 ou rand() % (100); 0 ate 99 */
    srand(time(NULL)); //sempre que executar vai mudar os valores gerados aleatoriamente
    
    //input_test(); //alterando manualmente, quando terminar utiliza
    
    pthread_t pessoas[num_pessoas];
    
    for (long int i = 0; i < num_pessoas; i++)
        pthread_create(&pessoas[i], NULL, pessoa, (void *) i);

    for (int i = 0; i < num_pessoas; i++)
        pthread_join(pessoas[i], NULL);

    return 0;
};

