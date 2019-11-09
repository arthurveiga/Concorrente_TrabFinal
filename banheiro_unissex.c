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

#define HOMEM 0 //homem utilizando
#define MULHER 1 //mulher utilizando
#define VAZIO -1 // banheiro vazio

int num_banheiros = 1, num_compartimentos = 1, num_generos = 1, num_pessoas = 100;
int genero_no_banheiro = 0;               //essa variável que vai mostrar qual é o gênero que deve entrar. Definimos como -1, que representa que o banheiro está vazio. provavelmente essa variável deverá ser um array de tamanho num_compartimentos
int num_pessoas_no_banheiro = 0;          //essa variável mostra quantos estão no banheiro
sem_t sem_compartimentos;                 //o semáforo representa o banheiro. O num_compartimentos é o tamanho do banheiro.
pthread_mutex_t mutx_entrada_no_banheiro; //essa é a variável condicional que faz com que cada pessoa (thread) entre um por um no banheiro (o banheiro tem uma porta de entrada só)
pthread_mutex_t mutx_genero_no_banheiro;  // controla genero no banheiro

struct dados_pessoa
{
    int gen_pessoa;
    int pos_fila;
};

/**
 * Ou seja, a 1a pessoa entrou no banheiro (pegou a variável condicional), mudou a 
 * condicional genero_no_banheiro para o seu gênero definido na main (como id de thread) e 
 * entrou no banheiro e agora, todos os outros que são do mesmo gênero entram. Isso 
 * acontece até acabarem todos do mesmo gênero, ou se o banheiro esvaziar e uma pessoa de 
 * outro gênero entre no banheiro.
 * 
 */
void print_genero(int genero){
    if(genero != VAZIO){
        printf("Banheiro ocupado:\n");
        if(genero == HOMEM)
            printf("Homem no banheiro\n");
        else if(genero == MULHER)
            printf("Mulher no banheiro\n");
    }
    else if(genero == VAZIO)
        printf("\nBanheiro livre");
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

void banheiro_ocupado(int genero_pessoa, int posicao_fila)
{
    print_genero(genero_pessoa);
    if (num_pessoas_no_banheiro == num_compartimentos) // se atingiu capacidade maxima do banheiro muda o genero
    {
        pthread_mutex_lock(&mutx_genero_no_banheiro);
        genero_no_banheiro = genero_pessoa;
        pthread_mutex_unlock(&mutx_genero_no_banheiro);
    }
    else{ // se ainda nao atingiu capacidade do banheiro maxima

    }
}
void banheiro_livre(int genero_pessoa, int posicao_fila)
{
    sem_post(&sem_compartimentos); // libera compartimento
    sem_getvalue(&sem_compartimentos, &num_pessoas_no_banheiro); // pega numero de passoas
    printf("Compartimentos = %d", num_pessoas_no_banheiro);
    if (num_pessoas_no_banheiro == num_compartimentos) // se atingiu capacidade maxima muda o genero
    {
        pthread_mutex_lock(&mutx_genero_no_banheiro);
        genero_no_banheiro = genero_pessoa;
        pthread_mutex_unlock(&mutx_genero_no_banheiro);
    }
} 
void *pessoa(void *dados_pes)
{
    struct dados_pessoa *d_pes = dados_pes;

    int genero_pessoa = d_pes->gen_pessoa;
    int posicao_fila = d_pes->pos_fila;
    int alivio = 0; // variável local para saber se a pessoa fez o que tinha que fazer

    while (alivio != 1)
    {
        pthread_mutex_lock(&mutx_entrada_no_banheiro);
        printf("%d - entrei no banheiro!!\n", posicao_fila);
        num_pessoas_no_banheiro++;
        //sleep(1 + rand() % (3));
        pthread_mutex_unlock(&mutx_entrada_no_banheiro);

        if (genero_no_banheiro == HOMEM)
        {
            genero_no_banheiro = genero_pessoa;
            printf("%d - sou do gênero %d e tem ninguém! Ótimo.\n", posicao_fila, genero_pessoa);
        }
        if (genero_pessoa == genero_no_banheiro)
        {
            printf("%d - sou de gênero %d, verei se tem compartimento disponível. \n", posicao_fila, genero_pessoa);
            if (sem_trywait(&sem_compartimentos) == 0)
            {
                alivio = 1;
                sleep(1 + rand() % 3); // pra testes um valor pequeno
                sem_post(&sem_compartimentos);
            }
        }
        else
        {
            printf("%d - eita! Sou de gênero %d e já tem gente de gênero %d! Vou sair do banheiro aqui\n", posicao_fila, genero_pessoa, genero_no_banheiro);
        }
        num_pessoas_no_banheiro--;
    }
};

int main()
{
    /*Como gerar valores aleatoriamente: exemplo: 1 + rand() % (100); 1 ate 100 ou rand() % (100); 0 ate 99 */
    srand(time(NULL)); //sempre que executar vai mudar os valores gerados aleatoriamente

    //input_test(); //alterando manualmente, quando terminar utiliza

    struct dados_pessoa d_pes;
    pthread_t pessoas[num_pessoas];

    //inicializa semaforos e mutexes
    sem_init(&sem_compartimentos, 0, num_compartimentos);
    pthread_mutex_init(&mutx_entrada_no_banheiro, 0);
    pthread_mutex_init(&mutx_genero_no_banheiro, 0);

    for (long int i = 0; i < num_pessoas; i++)
    {
        d_pes.gen_pessoa = rand() % num_generos;
        d_pes.pos_fila = i + 1;
        pthread_create(&pessoas[i], NULL, pessoa, (void *)&d_pes);
    }

    for (int i = 0; i < num_pessoas; i++)
        pthread_join(pessoas[i], NULL);

    //destroi semaforos e mutexess
    sem_destroy(&sem_compartimentos);
    pthread_mutex_destroy(&mutx_genero_no_banheiro);
    pthread_mutex_destroy(&mutx_entrada_no_banheiro);
    return 0;
};
