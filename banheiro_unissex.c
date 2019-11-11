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

#define HOMEM 0  //homem utilizando
#define MULHER 1 //mulher utilizando
#define VAZIO -1 // banheiro vazio
int gen_pessoa; //gênero da pessoa

int num_banheiros, num_compartimentos, num_generos, num_pessoas;
int genero_no_banheiro = 0;               //essa variável que vai mostrar qual é o gênero que deve entrar. Definimos como -1, que representa que o banheiro está vazio. provavelmente essa variável deverá ser um array de tamanho num_compartimentos
int num_pessoas_no_banheiro = 0;          //essa variável mostra quantos estão no banheiro
sem_t sem_compartimentos;                 //o semáforo representa o banheiro. O num_compartimentos é o tamanho do banheiro.
pthread_mutex_t mutx_entrada_no_banheiro; //essa é a variável condicional que faz com que cada pessoa (thread) entre um por um no banheiro (o banheiro tem uma porta de entrada só)
pthread_mutex_t mutx_genero_no_banheiro;  // controla genero no banheiro


// imprimi na tela o genero
void print_genero(int genero, int posicao_fila)
{
    if (genero != VAZIO)
    {
        printf("Banheiro ocupado:\n");
        if (genero == HOMEM)
            printf("    %d - Homem no banheiro\n\n", posicao_fila);
        else if (genero == MULHER)
            printf("    %d - Mulher no banheiro\n\n", posicao_fila);
    }
    else if (genero == VAZIO)
        printf("Banheiro livre\n\n");
};
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

int time_banheiro(int genero, int posicao_fila, int alivio)
{

    int tempo = 1 + rand() % 10; // pra testes um valor pequeno
    sleep(tempo);
    printf("------------ Tempo------------------\n");
    print_genero(genero, posicao_fila);
    printf("Utilizando banheiro por %d segundos\n", tempo);
    printf("------------------------------------\n\n");
    alivio = 1;
    return alivio;
};

void banheiro_ocupado_mulher(int genero_pessoa, int posicao_fila)
{
    printf("%d - Mulher tentando entrar\n", posicao_fila);
    if (num_pessoas_no_banheiro == num_compartimentos) // se atingiu capacidade maxima do banheiro muda o genero
    {
        pthread_mutex_lock(&mutx_genero_no_banheiro);
        genero_no_banheiro = genero_pessoa;
        print_genero(genero_no_banheiro, posicao_fila);
        pthread_mutex_unlock(&mutx_genero_no_banheiro);

        pthread_mutex_lock(&mutx_entrada_no_banheiro);
        sem_wait(&sem_compartimentos);                               //decrementa compartimentos, aguarda ou bloqueia
        sem_getvalue(&sem_compartimentos, &num_pessoas_no_banheiro); // pega numero de passoas
        printf("\nCompartimento ocupado pela mulher: %d\nCompartimentos disponíveis = %d\n\n",posicao_fila, num_pessoas_no_banheiro);
        pthread_mutex_unlock(&mutx_entrada_no_banheiro);
    }
    else
    { // se ainda nao atingiu capacidade do banheiro maxima
        if (genero_no_banheiro == MULHER)
        {
            pthread_mutex_lock(&mutx_entrada_no_banheiro);
            sem_wait(&sem_compartimentos);
            sem_getvalue(&sem_compartimentos, &num_pessoas_no_banheiro);
            printf("\nCompartimento ocupado pela mulher: %d\nCompartimentos disponíveis = %d\n\n", posicao_fila, num_pessoas_no_banheiro);
            pthread_mutex_unlock(&mutx_entrada_no_banheiro);
        }
        else
        {
            while (genero_no_banheiro == HOMEM)
            {  // faz nada espera genero ser MULHER
                if(genero_no_banheiro == VAZIO){
                    pthread_mutex_lock(&mutx_genero_no_banheiro);
                    genero_no_banheiro = MULHER;
                    pthread_mutex_unlock(&mutx_genero_no_banheiro);
                }
            }
            banheiro_ocupado_mulher(genero_pessoa, posicao_fila);
        }
    }
}
void banheiro_ocupado_homem(int genero_pessoa, int posicao_fila)
{   
    printf("%d - Homem tentando entrar\n", posicao_fila);
    if (num_pessoas_no_banheiro == num_compartimentos) // se atingiu capacidade maxima do banheiro muda o genero
    {
        pthread_mutex_lock(&mutx_genero_no_banheiro);
        genero_no_banheiro = genero_pessoa;
        print_genero(genero_no_banheiro, posicao_fila);
        pthread_mutex_unlock(&mutx_genero_no_banheiro);

        pthread_mutex_lock(&mutx_entrada_no_banheiro);
        sem_wait(&sem_compartimentos);                               //decrementa compartimentos, aguarda ou bloqueia
        sem_getvalue(&sem_compartimentos, &num_pessoas_no_banheiro); // pega numero de passoas
        printf("\nCompartimento ocupado pelo homem: %d\nCompartimentos disponíveis = %d\n\n", posicao_fila, num_pessoas_no_banheiro);
        pthread_mutex_unlock(&mutx_entrada_no_banheiro);
    }
    else
    { // se ainda nao atingiu capacidade do banheiro maxima
        if (genero_no_banheiro == HOMEM)
        {
            pthread_mutex_lock(&mutx_entrada_no_banheiro);
            sem_wait(&sem_compartimentos);
            sem_getvalue(&sem_compartimentos, &num_pessoas_no_banheiro);
            printf("\nCompartimento ocupado pelo homem: %d\nCompartimentos disponíveis = %d\n\n", posicao_fila, num_pessoas_no_banheiro);
            pthread_mutex_unlock(&mutx_entrada_no_banheiro);
        }
        else
        {
            while (genero_no_banheiro == MULHER)
            { // faz nada espera genero ser homem
                if (genero_no_banheiro == VAZIO)
                {
                    pthread_mutex_lock(&mutx_genero_no_banheiro);
                    genero_no_banheiro = HOMEM;
                    pthread_mutex_unlock(&mutx_genero_no_banheiro);
                }
            }
            banheiro_ocupado_homem(genero_pessoa, posicao_fila);
        }
    }
}
void banheiro_livre(int genero_pessoa, int posicao_fila)
{
    sem_post(&sem_compartimentos);                               // libera compartimento
    sem_getvalue(&sem_compartimentos, &num_pessoas_no_banheiro); // pega numero de pessoas

    if(genero_pessoa == HOMEM)
        printf("Homem %d saiu do banheiro\nCompartimentos = %d\n", posicao_fila, num_pessoas_no_banheiro);
    else if (genero_pessoa == MULHER)
        printf("Mulher %d saiu do banheiro\nCompartimentos = %d\n", posicao_fila, num_pessoas_no_banheiro);

    if (num_pessoas_no_banheiro == num_compartimentos) // se atingiu capacidade maxima muda o genero
    {
        pthread_mutex_lock(&mutx_genero_no_banheiro);
        genero_no_banheiro = VAZIO;
        pthread_mutex_unlock(&mutx_genero_no_banheiro);
    }
}


void *pessoa_homem(void *pos_fila)
{

    int posicao_fila = (long int) pos_fila;

    srand((unsigned) time(NULL) * posicao_fila); //sempre que executar vai mudar os valores gerados aleatoriamente
    int alivio = 0; // variável local para saber se a pessoa fez o que tinha que fazer

    while (alivio != 1)
    {
        
        banheiro_ocupado_homem(HOMEM, posicao_fila);
        alivio = time_banheiro(HOMEM, posicao_fila, alivio);
        banheiro_livre(HOMEM, posicao_fila);
    }
};

void *pessoa_mulher(void *pos_fila)
{

    int posicao_fila = (long int) pos_fila;

    srand((unsigned) time(NULL) * posicao_fila); //sempre que executar vai mudar os valores gerados aleatoriamente
    int alivio = 0; // variável local para saber se a pessoa fez o que tinha que fazer

    while (alivio != 1)
    {
        
        banheiro_ocupado_mulher(MULHER, posicao_fila);
        alivio = time_banheiro(MULHER, posicao_fila, alivio);
        banheiro_livre(MULHER, posicao_fila);
    }
};

int main()
{
    /*Como gerar valores aleatoriamente: exemplo: 1 + rand() % (100); 1 ate 100 ou rand() % (100); 0 ate 99 */
    input_test(); //alterando manualmente, quando terminar utiliza

    pthread_t pessoas[num_pessoas];

    //inicializa semaforos e mutexes
    sem_init(&sem_compartimentos, 0, num_compartimentos);
    pthread_mutex_init(&mutx_entrada_no_banheiro, 0);
    pthread_mutex_init(&mutx_genero_no_banheiro, 0);

    for (long int i = 0; i < num_pessoas; i++)
    {
        gen_pessoa = rand() % num_generos;
        switch (gen_pessoa)
        {
        case HOMEM:        
            pthread_create(&pessoas[i], NULL, pessoa_homem, (void *) i+1);
            break;
        
        case MULHER:
            pthread_create(&pessoas[i], NULL, pessoa_mulher, (void *) i+1);
            break;

        default: 
            pthread_create(&pessoas[i], NULL, pessoa_mulher, (void *) i+1);
            break;
        }
    }

    for (int i = 0; i < num_pessoas; i++)
        pthread_join(pessoas[i], NULL);

    //destroi semaforos e mutexess
    sem_destroy(&sem_compartimentos);
    pthread_mutex_destroy(&mutx_genero_no_banheiro);
    pthread_mutex_destroy(&mutx_entrada_no_banheiro);
    return 0;
};
