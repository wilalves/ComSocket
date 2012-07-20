/** 
* \file ClienteA.c
* \brief Socket Cliente Produtor utilizando Gerenciamentos de Threads
* \author Willian Alves
* \version 0001
* \date Criação: Jul/2012
* envia buffer para serem INSERIDOS na FILA*/

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
#include <itoa.h>
    /** Toda configuracao do Cliente Produtor e criaçao do socket */	 
	int configuracaoClienteA()
	{
	    /* Descritor do cliente */
	    int clienteSockfdA;
        /* Estrutura do Cliente */
	    struct sockaddr_in serv_addrA;
	    /* Tamanho da estrutura */
	    socklen_t addrlen = sizeof (serv_addrA);
	    /* Socket familia TCP declarado SOCK_STREAM e criado */
	    clienteSockfdA = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	    /** Verifica a Conexão */
        if (clienteSockfdA < 0)
	    {
	        printf("Erro no Socket\n");
	        exit(1);
	    }
	    /* Zera a estrutura */
	    bzero((char *) & serv_addrA, sizeof (serv_addrA));
	    /* Seta a familia */
	    serv_addrA.sin_family = AF_INET;
	    /* Define o IP nesse caso o localhost */
	    serv_addrA.sin_addr.s_addr = inet_addr("10.60.69.140");
	    /* Define a porta de conexao */
	    serv_addrA.sin_port = htons(6666);
	    /* Verifica e Faz a conexao com o servidor */
	    if(connect(clienteSockfdA,(struct sockaddr *)&serv_addrA, addrlen) < 0)
	     {
	            printf("Erro no Socket\n");
	            exit(1);
	     }
	        return clienteSockfdA;
		 }
         /** Troca mensagens com o Servidor */
		 void ClienteA(int clienteSockfdA)
		 {
	      /** Buffer de dados a ser mandado para o servidor e a RECEBER */
	      char buffer_para_servidor[256] = "";
          char buffer_do_clienteA[256] = "";
          int A=0;
          /**Loop infinito*/
	       while(1) {
                    /* Pedi que o usuario digite a mensagem para o servidor
	                * printf("Digite uma mensagem para o servidor: ");
                    * Lê a mensagem ESCRITA
	                * scanf("%s",buffer_para_servidor);
                    * Limpa uma possivel leitura de um ENTER ou alguma coisa que sobrara no buffer */
                    f_itoa( buffer_para_servidor, A );
                    /* printf("%d convertido = %s\n", A, buffer_para_servidor);
                    *   sleep(10); */
                    fflush(stdin);
                    /* Verifica se a mensagem é igual a condição de retirada cujo cliente A que so INSERI não tem PRIVILEGIO */
                    if(strcmp(buffer_para_servidor,"r")==0){
                        /* Imprimi a mensagem de negação ou ERRO */
                        printf("vs não tem esse PRIVILEGIO!\n");
                        /* Apaga essa mensagem zera a mensagem para voltar ao loop */
                        memset(buffer_para_servidor,0,sizeof(buffer_para_servidor));
                        }
                        /* se não é R faz o seguinte */
                    else{
                        /* manda a mensagem DIGITADA para o SERVIDOR */
	                    write(clienteSockfdA, buffer_para_servidor, sizeof(buffer_para_servidor));
                        /* Faz a LEITURA que foi RETORNADO do Servidor para Cliente A */
	                    read(clienteSockfdA, buffer_do_clienteA, sizeof(buffer_do_clienteA));
                        /* Imprimi a MENSAGEM do Servidor no Cliente A */
                        printf("mensagem do Servidor: %s\n",buffer_do_clienteA);
                        if(strcmp(buffer_do_clienteA,"Cheio")==0){
                            read(clienteSockfdA, buffer_do_clienteA, sizeof(buffer_do_clienteA));
                            printf("mensagem do Servidor: %s\n", buffer_do_clienteA);
                            /* break; */
                                    }
                                else{/*  Se não NADA faz.
	                                  *  fflush(stdin);
                                      *  if(strcmp(buffer_para_servidor,"r")==0){
                                      *  memset(buffer_para_servidor,0,sizeof(buffer_para_servidor));
                                      *  }
	                                  *  Escreve para o servidor
                                      *  printf("Fila Totalmente Cheia\n");
	                                  *   Mensagem para sair */
	                                }   
                }A++;
                sleep(1);
          }
              /*  printf("Fila Totalmente Cheia\n");
              *    Encerra o descritor */
	    close(clienteSockfdA);
	 }


     /** Função PRINCIPAL Faz as Conexoes com o Servidor */
	int main()
	{
	    system("clear");
	    /* Estrutura cliente */
	    struct sockaddr_in serv_addrA;
	    /* Tamanho da estrutura */
	    socklen_t addrlen = sizeof (serv_addrA);
	    /* Define o descritor cliente */
	    int descritorClienteA;
	    descritorClienteA = configuracaoClienteA();
	    /* chama funcao do cliente passando o descritor */
	    ClienteA(descritorClienteA);
	    exit(0);
	}
