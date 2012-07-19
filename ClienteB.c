/** 
* \file ClienteB.c
* \brief Socket Cliente Consumidor utilizando Gerenciamentos de Threads
* \author Willian Alves
* \version 0001
* \date Criação: Jul/2012
* envia buffer para serem RETIRADOS da FILA */





#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <ctype.h>
#include <string.h>
	/** Toda configuracao do Cliente Consumidor e criaçao do socket */ 
	int configuracaoClienteB()
	{
	    /* Descritor do cliente */
	    int clienteSockfdB;
        /* Estrutura do Cliente */
	    struct sockaddr_in serv_addrB;
	    /* Tamanho da estrutura */
	    socklen_t addrlen = sizeof (serv_addrB);
	 
	    /* Socket familia TCP declarado SOCK_STREAM e criado */
	    clienteSockfdB = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	    if (clienteSockfdB < 0)
	    {
	        printf("Erro no Socket\n");
	        exit(1);
	    }
	    /* Zera a estrutura */
	    bzero((char *) & serv_addrB, sizeof (serv_addrB));
	    /* Seta a familia */
	    serv_addrB.sin_family = AF_INET;
	    /* Define o IP nesse caso o localhost */
	    serv_addrB.sin_addr.s_addr = inet_addr("10.60.69.140");
	    /* Define a porta de conexao */
	    serv_addrB.sin_port = htons(6666);
	    /* Verifica e Faz a conexao com o servidor */
	    if(connect(clienteSockfdB,(struct sockaddr *)&serv_addrB, addrlen) < 0)
	     {
	           
	            printf("Erro no Socket talvez: Servidor não esta em COMUNIÇÃO\n");
	            exit(1);
	     }
	        return clienteSockfdB;
		 }
	    /** Envia e Recebe todas as mensagem */
		void ClienteB(int clienteSockfdB)
		{
	    /*  Buffer de dados a ser mandado para o servidor e a RECEBER */
	      char buffer_para_servidor;
          char buffer_do_clienteB[256]= "";
	      while(1) {
	              /*  printf("Digite 'r' para RETIRAR o PRIMEIRO DA FILA: ");
	               * scanf("%s",buffer_para_servidor); */
	               buffer_para_servidor = 'r';
                   fflush(stdin);
                   /* if(strcmp(buffer_para_servidor,"r")==0){
	               *   Escreve para o servidor */
	                write(clienteSockfdB, &buffer_para_servidor, sizeof(buffer_para_servidor));
                    memset(buffer_do_clienteB, 0, sizeof(buffer_do_clienteB));
	                read(clienteSockfdB, buffer_do_clienteB, sizeof(buffer_do_clienteB));
                    printf("mensagem do Servidor: %s\n",buffer_do_clienteB);
	            // }
                sleep(1);
            }
	    close(clienteSockfdB);
	    }
    /** Função PRINCIPAL Faz as Conexoes com o Servidor */
	int main()
	{
	    system("clear");
	    /* Estrutura cliente */
	    struct sockaddr_in serv_addrB;
	    /* Tamanho da estrutura */
	    socklen_t addrlen = sizeof (serv_addrB);
	    /* Define o descritor cliente */
	    int descritorClienteB;
	    descritorClienteB = configuracaoClienteB();
	    /* Chama funcao do cliente passando o descritor*/
	    ClienteB(descritorClienteB);
	    exit(0);
	}
