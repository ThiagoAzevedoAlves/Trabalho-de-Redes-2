#include <iostream>
#include <fstream>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <cstdlib>
#include "funcsServ.h"

using namespace std;

int main(){
	ifstream hash;
	ifstream arquivo;
	int sock, novo_sock, tamanho; //sockets;
	struct sockaddr_in serv_addr, cl_addr; //estrutura para configuração do socket destino e origem;
	if ((sock = socket(PF_INET, SOCK_STREAM, 0)) < 0){ //declara o novo socket testando se não houve erro;
		cout << "ERRO DE SOCKET No " 
			 << -sock //número do erro;
			 << endl;
		exit(1);
	}
	serv_addr.sin_family = AF_INET; //protocolo internet arpa
	serv_addr.sin_addr.s_addr = INADDR_ANY; //procura endereço disponivel de internet
	serv_addr.sin_port = htons(5555); //número da porta
	
	if (bind(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){ //realiza o bind testando erros;
		perror("bind");
		cout << "ERRO AO REALIZAR O BIND()" 
			 << endl;
		exit(1);
	} 
	listen(sock,1); //tamanho da fila de sockets para o listen = 5;
	
	cout << "Esperando conexões: "
		 << endl;
	
	while(1){
		tamanho = sizeof(struct sockaddr_in);
		if ((novo_sock = accept(sock, (struct sockaddr*) &cl_addr, (socklen_t *)&tamanho)) < 0){
			cout << "ERRO AO REALIZAR ACCEPT()" 
				 << endl;
			exit(1);
		}
		cout << "Servidor recebeu pedido de conexão de: "
			 << inet_ntoa(cl_addr.sin_addr)
			 << endl;
		if (fork()==0){ // se for filho 
			if (send(novo_sock, "Hello, world!\n", 14, 0) == -1){
				perror("send");
				cout << "Erro ao enviar mensagem!"
					 << endl;
				close(novo_sock);
				exit(0);			
			}
			close(novo_sock);
		}
 
	}
	return 0;
	
}
