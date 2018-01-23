#include <iostream> // cout
#include <sstream> // cout
#include <stdlib.h>  // exit
#include <string.h> // bzero
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include "libgpsmm.h"


//user created
#include "GPS.hpp"

using namespace std;


void writeline(int socketfd, string line) {
	string tosend = line + "\n";
	write(socketfd, tosend.c_str(), tosend.length());
}

/* Lê uma linha de um socket // funciona corretamente
   retorna false se o socket se tiver fechado */
bool readline(int socketfd, string &line) {
  int n;
  /* buffer de tamanho 1025 para ter espaço
     para o \0 que indica o fim de string*/
  char buffer[1025];

  /* inicializar a string */
  line = "";

  /* Enquanto não encontrarmos o fim de linha
     vamos lendo mais dados da stream */
  while (line.find('\n') == string::npos) {
    // leu n carateres. se for zero chegamos ao fim
    int n = read(socketfd, buffer, 1024); // ler do socket
    if (n == 0) return false; // nada para ser lido -> socket fechado
    buffer[n] = 0; // colocar o \0 no fim do buffer
    line += buffer; // acrescentar os dados lidos à string
  }

  // Retirar o \r\n (lemos uma linha mas não precisamos do \r\n)
  line.erase(line.end() - 1);
  line.erase(line.end() - 1);
  return true;
}

string doubleToString(double i) {
        ostringstream oss;
        oss << i;
        return oss.str();
}

//Client attending routine
void* client_handler(void* args){

		int client_socket = *(int*) args;
		string line;

		cout << "we have lift off" << endl;

		writeline(client_socket, "Welcome to the show");

		while(readline(client_socket,line)){

			cout << " The socket: " << client_socket << " said: " << line << endl;

			if(line.find("GPS")==0){

					writeline(client_socket, "Here´s the GPS info, lat equals: " + doubleToString(latitude()) + "  ");
					writeline(client_socket, "long equals: " +doubleToString(longitude()) + "  ");
					writeline(client_socket, "long equals: " +doubleToString(speed()) + "  ");

					cout << "give the GPS info to the client " << endl;
			}
		}


}
//initialize the server with the port chosen in main()
int tcp_init_server(int port){

		int sockfd;
		//socklen_t client_addr_length;
		struct sockaddr_in serv_addr;
		srand(time(NULL));

		/* Inicializar o socket
	     AF_INET - para indicar que queremos usar IP
	     SOCK_STREAM - para indicar que queremos usar TCP
	     sockfd - id do socket principal do servidor
	     Se retornar < 0 ocorreu um erro */

		sockfd= socket(AF_INET,SOCK_STREAM,0);

		if (sockfd < 0) {
	    cout << "Error creating socket" << endl;
	     exit(-1);
	  }

		/* Criar a estrutura que guarda o endereço do servidor
	     bzero - apaga todos os dados da estrutura (coloca a 0's)
	     AF_INET - endereço IP
	     INADDR_ANY - aceitar pedidos para qualquer IP */

	  bzero((char *) &serv_addr, sizeof(serv_addr));
	  serv_addr.sin_family = AF_INET;
	  serv_addr.sin_addr.s_addr = INADDR_ANY;
	  serv_addr.sin_port = htons(port);

		/* Fazer bind do socket. Apenas nesta altura é que o socket fica ativo
	     mas ainda não estamos a tentar receber ligações.
	     Se retornar < 0 ocorreu um erro */

	  int res = bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));

	  if (res < 0) {
	    cout << "Error binding to socket" << endl;
	    exit(-1);
	  }

		/* Indicar que queremos escutar no socket com um backlog de 5 (podem
 	     ficar até 5 ligações pendentes antes de fazermos accept */

 	  listen(sockfd, 5);

		return sockfd;
}
//input sockfd do servidor, output socketfd do cliente
int tcp_new_client(int server){

	/* Aceitar uma nova ligação. O endereço do cliente fica guardado em
		 cli_addr - endereço do cliente
		 sockfd - id do socket que comunica com este cliente */

		int sockfd;
		socklen_t client_addr_length;
		struct sockaddr_in cli_addr;

		client_addr_length = sizeof(cli_addr);
		sockfd = accept(server, (struct sockaddr *) &cli_addr, &client_addr_length);

		return sockfd;
}
//Recebe os clientes e cria os threads para os acompanhar
void* tcp_client_receiver(void* args){

		int server = *(int*)args;

		while(1){

				int client_socket = tcp_new_client(server);
				pthread_t thread;
				pthread_create(&thread, NULL, client_handler, &client_socket);

		}

}
