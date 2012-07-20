/** 
* \file Servidor.c
* \brief Socket Servidor utilizando Gerenciamentos de Threads
* \author Willian Alves
* \version 0001
* \date Criação: Jul/2012
* Fica a escuta de diversos Clientes tanto produtores quanto consumidores */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <pthread.h>
#include <itoa.h>
    
	/* Declaraçao da fila */
    int fila[999];
    /* Contador das POSIÇÕES da FILA */
    int contfila=0;
    /* Variavel que vai RECEBER o CONTEUDO da FILA na posição ZERO '0' */
    int recebefila;
    pthread_mutex_t mutex_A;
    /** Recebe e Envia mensagem para os Clientes Adc ou Remove da Fila, verifica Produto/Consumidor */
	void* ServidorA(void* arg)
	{
	    /* Buffer de entrada armazena buffer do cliente onde ja defini seu tamanho */
        char buffer_do_clienteA[256];
        /* buffer que Servidor enviar para Cliente tbm defini o seu tamanho maximo */
        char buffer_para_clienteA[256];
	    /* declaro que meu ponteiro é do TIPO inteiro e que quero o endereço */
	    int sockEntradaA = *(int *) arg;
	    /** Loop "infinito" */
	    while(1)
        {
        /** se contador de fila menor que qtde declarada do tamanho da fila for menos entra aqui
        *   Le o que vem do cliente */
                if((read(sockEntradaA, buffer_do_clienteA, sizeof(buffer_do_clienteA)))>0){
	                printf("Mensagem do Cliente: %s\n",buffer_do_clienteA);

                    if(contfila<999 && strcmp(buffer_do_clienteA,"r")!=0){
                        pthread_mutex_lock(&mutex_A);
                        /* Converte o conteudo da mensagem do cliente em INTEIRO e adc a fila */
                        fila[contfila]=atoi(buffer_do_clienteA);

                        /* Conta mais uma posiçao na fila */
                        contfila++;
                        pthread_mutex_unlock(&mutex_A);
                   
                        /* Imprimi aqui no servidor o CONTEUDO da mensagem na posiçao anterior */
                        printf("Ultimo Adc = %d \n", fila[contfila-1]);
                        /* Limpa a conteuso da variavel */
                        memset(buffer_para_clienteA, 0, sizeof(buffer_para_clienteA));
                        /* copia recebido para dentro da VARIAVEL */
                        strcpy(buffer_para_clienteA,"recebido");
                        /* Envia pela conexao a mensagem recebido para o CLIENTE */
                        write(sockEntradaA, buffer_para_clienteA, sizeof(buffer_para_clienteA));
                        }
                        /* Se a fila for maior que zero e buffer do cliente for igual a '111' retira um da FILA */
                    else if(contfila>0 && strcmp(buffer_do_clienteA,"r")==0){
                        /* Variavel que recebera o coteudo da fila no loop */
                        int menosfila = 0;
                        /* variavel que recebe a fila na PRIMEIRA posição */
                        recebefila = fila[menosfila];
                        pthread_mutex_lock(&mutex_A);
                        /** Laco que trocara as P&OSIÇOES DE todos da fila decrementando sempre se for menor que tamanho da fila faz */
                        while(menosfila<contfila){
                            fila[menosfila] = fila[menosfila+1];
                            /* aqui incrementa um contador para as posições da nova fila que esta sendo gerada */
                            menosfila++;
                            }
                            /* imprimi quem foi o primeiro a ser inserido na FILA */
                            printf("primeiro da Fila %d\n",recebefila);
                            /* Limpa a variavel para que nao reste nem um lixo */
                            memset(buffer_para_clienteA,0, sizeof(buffer_para_clienteA));
                            /* copia para dentro da variavel a mensagem entre "" */
                            strcpy(buffer_para_clienteA,"Retirado");
                            /* envia para cliente o valor dentro da VARIAVEL */
                            write(sockEntradaA, buffer_para_clienteA, sizeof(buffer_para_clienteA));
                            /* Imprimi o novo conteudo da POSIÇÃO ZERO '0' */
                            printf("primeiro da fila depois de RETIRADO UM DA FILA %d\n",fila[0]);
                            /* Decrementa o contador para que a fila reggresse */
                            contfila--;
                            pthread_mutex_unlock(&mutex_A);
                            }/** se a fila for menor que ZERO '0' e MENSAGEM FOR r "MANDA A MSGM de vazia" */
                        else if(contfila<=0 && strcmp(buffer_do_clienteA,"r")==0){
                            /* copia para dentro da variavel a MSGM */
                            strcpy(buffer_para_clienteA,"Fila Vazia");
                            /* Envia pela conexao a nova mensagem */
                            write(sockEntradaA, buffer_para_clienteA, sizeof(buffer_para_clienteA));
                            }
                        else {
                            // return "a"; TESTANDO
                            /*   Se nao imprimi no SERVIDOR que não existe mais posiçoes livres na FILA */
                            printf("Fila no limite totalmente CHEIA\n");
                            /* Limpa a variavel novamente */
                            memset(buffer_para_clienteA, 0, sizeof (buffer_para_clienteA));
                            /* Copia a mensagem para dentro da VARIAVEL */
                            strcpy(buffer_para_clienteA,"Cheio!");
                            /* IMPRIMI aqui no Servidor a mensagem de CHEIO!! */
                            printf("Msg: %s \n", buffer_para_clienteA);
                            /* Envia para o Cliente essa NOVA mensagem */
                            write(sockEntradaA, buffer_para_clienteA, sizeof(buffer_para_clienteA));
                            /* testes para PAUSAR por tempo determinado o que ta ACONTECENDO COM o SERVIDOR
                            *   sleep(2); TESTANDO
                            *   break; */
                            sleep(1);
                            }
                        }
              else{     
                    /** Encerra o DESCRITOR */
                    close(sockEntradaA);
                    }
             }
        }
	/** Configiurando o Servidor com todas as declarações necessarias de criação de um SOCKET */
	int configuracaoServidor()
	{
	    /* Cria o descritor */
	    int sockfd;
	    /** Declaracao da estrutura */
        struct sockaddr_in serverAddr;
        /* Cria o socket
        *   Imprimi uma mensagem se ele não for criado e o retorno for <0 ou seja menor que ZERO */
        if ((sockfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
            {
                printf("Erro no Socket\n");
                exit(1);
            }
	    /* Zera a estrutura */
	    memset(&serverAddr, 0, sizeof (serverAddr));
	    /* Seta a familia */
	    serverAddr.sin_family = AF_INET;
	    /* Seta os IPS (A constante INADDR_ANY e todos os ips ou qualquer ip) htonl -> conversao de host em ordem de bytes da rede */
	    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	    /* Define a porta */
	    serverAddr.sin_port = htons(6666);
	    /* Faz a bindagem (cola, gruda, conecta seja o que for) */
	    if (bind(sockfd, (struct sockaddr *) & serverAddr, sizeof(serverAddr)) < 0)
    	    {
	            printf("Erro no Socket\n");
     	        exit(1);
	        }
	    /* Fica na escuta dos clientes */
	    if (listen(sockfd, 5) < 0)
	        {
	            printf("Erro no Socket\n");
	            exit(1);
	        }
	    return sockfd;
	}
	 /** Função PRINCIPAL Faz as Conexoes com todos os Clientes*/
	int main()
	{
        /* Faz uma limpeza na TELA */
	    system("clear");
	    /* Declaracao da estrutura */
	    struct sockaddr_in serverAddr;
	    /* Retorna da funcao e o descritor */
	    int sockfdA = configuracaoServidor();
        int teste;
	 
        printf("***Servidor esta escutando***\n");
	    /** Loop "infinito" */
	    while (1)
	    {
           /* Variavel que recebera a Conexao com cliente */
	       int clienteSockfdA;
           /* Declaracao da ESTRUTURA de COMUNICACAO */
     	   struct sockaddr_in clienteAddrA;
	       /* tamanho da estrutura */
	       unsigned int clntLenA;
	       clntLenA = sizeof (clienteAddrA);
	       /* declara uma thread */
	       pthread_t threadA;
           pthread_mutex_init(&mutex_A,0);
	       /* Fica no aguardo da conexao do cliente se não existir conexao retorna uma MENSAGEM DE ERRO */
	       if ((clienteSockfdA = accept(sockfdA, (struct sockaddr *) & clienteAddrA, &clntLenA)) > 0)
	          {
                    printf("conectado\n");
	                printf("Aguardando as mensagens de novo cliente... \n");
	           }
               else{
	                printf("Erro no Socket\n");
	                exit(1);
               }
	       /* Inicializa a thread não criar a THREAD exibi uma mensagem de ERRO */
	       if (pthread_create(&threadA, NULL, ServidorA, &clienteSockfdA) == 0)
	         {
                    printf("Thread Criada...!\n");

             }
	         
             else {
	                printf("Erro na Thread\n");
	                exit(1);
                    }
            /** Quando um segmento termina automaticamente libera de volta para o sistema */
	        pthread_detach(threadA);
	    }
	    exit(0);
    }   
