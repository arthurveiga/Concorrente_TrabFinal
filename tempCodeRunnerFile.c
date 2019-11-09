
        pthread_mutex_lock(&mutx_entrada_no_banheiro);
        printf("%d - entrei no banheiro!!\n", posicao_fila);
        num_pessoas_no_banheiro++;
        sleep(1 + rand() % (3));
        pthread_mutex_unlock(&mutx_entrada_no_banheiro);