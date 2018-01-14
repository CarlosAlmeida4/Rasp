/*
 * Rasp_TCP.cpp
 * 
 * Copyright 2018 root <root@kali>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */

#include <iostream> // cout
#include <sstream> // cout
#include <stdlib.h>  // exit
#include <string.h> // bzero
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>//threads
#include <set>//Set para os clientes

using namespace std;

set<int> clients;//set usado para os clientes 


/* Envia uma string para um socket */ //funciona corretamente
void writeline(int socketfd, string line) {
	string tosend = line + "\n";
	write(socketfd, tosend.c_str(), tosend.length());
}


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

/* Envia uma mensagem para todos os clientes ligados exceto o que comunica */ // funciona corretamente
void broadcast (int origin, string text) {
   /* Usamos um ostringstream para construir uma string
      Funciona como um cout mas em vez de imprimir no ecrã
      imprime numa string */
   ostringstream message;
   message << text;

   // Iterador para sets de inteiros 
   set<int>::iterator it;
   //cout << "O utilizador" << origin << "disse: " << text << endl;	
	for (it = clients.begin(); it != clients.end(); it++)
	{
		if (*it != origin)
			{
				writeline(*it, message.str());
			}
	}
}


void* client(void* args){
	
	int origin = *(int*)args;
	string line;
	string aux;
	
	
	clients.insert(origin);
		
	writeline(origin,"welcome, insira \\help para uma lista de comandos");
	  		
	pthread_mutex_t mutex;
	pthread_mutex_init(&mutex,NULL);	
		
	cout << "Client connected: " << origin << endl;
	 
	while (readline(origin, line)) {	
	    
	    cout << "Socket " << origin << " said: "<< line << " (função cliente)\n"<< endl;

	}
	
	 cout << "Client disconnected: " << origin << endl;
		
	pthread_mutex_lock(&mutex);
	//apaga da lista de clientes
	pthread_mutex_unlock(&mutex);
	pthread_mutex_destroy(&mutex);
}


int main(int argc, char *argv[]){
	  
	  /* Estruturas de dados */
	  int sockfd, newsockfd, port = 8001;
	  socklen_t client_addr_length;
	  struct sockaddr_in serv_addr, cli_addr;
	  srand (time(NULL));
	  /* Inicializar o socket
	     AF_INET - para indicar que queremos usar IP
	     SOCK_STREAM - para indicar que queremos usar TCP
	     sockfd - id do socket principal do servidor
	     Se retornar < 0 ocorreu um erro */
	  
	  sockfd = socket(AF_INET, SOCK_STREAM, 0);
	  
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
      
      cout << " O Meu IP é: " <<	serv_addr.sin_addr.s_addr << endl;

	  while(1){
	    
	    /* Aceitar uma nova ligação. O endereço do cliente fica guardado em 
	       cli_addr - endereço do cliente
	       newsockfd - id do socket que comunica com este cliente */
	    
	    client_addr_length = sizeof(cli_addr);
	    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &client_addr_length);
	
	    /* Criar uma thread para tratar dos pedidos do novo cliente */
	    pthread_t thread;
	    pthread_create(&thread, NULL, client, &newsockfd);
		
		
	  }
	
	 
	  return 0; 
}



