#include <iostream>
#include <fstream>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <cstdlib>
#include "funcsServ.h"

using namespace std;

int main(){
	int sock, novo_sock; //sockets;
	struct sockaddr_in serv_addr, cl_addr; //estrutura para configuração do socket destino e origem;
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){ //declara o novo socket testando se não houve erro;
		cout << "ERRO DE SOCKET No " << -sock << endl;
		exit(1);
	}
	serv_addr.sin_family = AF_INET; //protocolo internet arpa
	serv_addr.sin_addr.s_addr = INADDR_ANY; //procura endereço disponivel de internet
	serv_addr.sin_port = htons(80); //número da porta
	if (bind(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){ //realiza o bind testando erros;
		cout << "ERRO AO REALIZAR O BIND()" << endl;
		exit(1);
	} 
	listen(sock,5); //tamanho da fila de sockets para o listen = 5;
	if ((novo_sock = accept(sock, (struct sockaddr*) &serv_addr, (socklen_t *)sizeof(cl_addr))) < 0){
		cout << "ERRO AO REALIZAR ACCEPT()" << endl;
		exit(1);
	}
	
	
	
}
