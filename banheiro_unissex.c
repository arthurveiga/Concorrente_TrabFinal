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

const int num_banheiros = 1, num_compartimentos = 1, num_generos = 1, num_pessoas = 100;
sem_t banheiro; //o semáforo representa o banheiro. O num_compartimentos é o tamanho do banheiro.
int genero_no_banheiro = 0; //essa variável que vai mostrar qual é o gênero que deve entrar. Definimos como -1, que representa que o banheiro está vazio. provavelmente essa variável deverá ser um array de tamanho num_compartimentos
pthread_cond_t entrada_no_banheiro; //essa é a variável condicional que faz com que cada pessoa (thread) entre um por um no banheiro (o banheiro tem uma porta de entrada só)

/**
 * Ou seja, a 1a pessoa entrou no banheiro (pegou a variável condicional), mudou a 
 * condicional genero_no_banheiro para o seu gênero definido na main (como id de thread) e 
 * entrou no banheiro e agora, todos os outros que são do mesmo gênero entram. Isso 
 * acontece até acabarem todos do mesmo gênero, ou se o banheiro esvaziar e uma pessoa de 
 * outro gênero entre no banheiro.
 * 
 */

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
    int alivio = 0; // variável local para saber se a pessoa fez o que tinha que fazer
    while (alivio == 0) {

    }
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

