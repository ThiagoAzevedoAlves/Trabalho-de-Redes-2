#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <cstdlib>

using namespace std;

int main(int argc, char *argv[]){
	//*argv[0] == nome do programa;
	//*argv[1] == arquivo contendo chave pública do cliente;
	
	if (argc != 2){
		cout << "Parâmetros inválidos!"
			 << "./servidor <chave publica do cliente>"
			 << endl;
		exit(1);
	}
	
	
	ifstream arquivo;
	int sock, novo_sock, tamanho; //sockets;
	struct sockaddr_in serv_addr, cl_addr; //estrutura para configuração do socket destino e origem;
	char buffer[4000];
	char string[256];
	
	
	if ((sock = socket(PF_INET, SOCK_STREAM, 0)) < 0){ //declara o novo socket testando se não houve erro;
		cout << "ERRO DE SOCKET No " 
			 << -sock //número do erro;
			 << endl;
		exit(1);
	}
	serv_addr.sin_family = AF_INET; //protocolo internet arpa
	serv_addr.sin_addr.s_addr = INADDR_ANY; //procura endereço disponivel de internet
	serv_addr.sin_port = htons(5678); //número da porta
	
	if (bind(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){ //realiza o bind testando erros;
		perror("bind");
		cout << "ERRO AO REALIZAR O BIND()" 
			 << endl;
		exit(1);
	} 
	listen(sock,1); //tamanho da fila de sockets para o listen = 5;
	
	cout << "Esperando conexões: "
		 << endl;
	
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
	
	//agora envia um arquivo hash.txt do arquivo que vai ser enviado para garantir a integridade ---------------------------------------//
		
		system("openssl dgst -md5 arquivo.txt > hash.txt");
		
		arquivo.open("hash.txt");
		arquivo.getline(buffer, 3999);
		if (send(novo_sock, buffer, (int)strlen(buffer), 0) == -1){
			perror("send");
			cout << "Erro ao enviar hash!"
				 << endl;
			close(novo_sock);
			exit(0);			
		}
		arquivo.close();
		
		//criptografa a hash com a chave privada do servidor e reenvia, para garantir a autenticidade --------------------------------------//
		
		system("openssl dgst -md5 -sign chave_privada_servidor.key -out signed_digest_file hash.txt");
		
		arquivo.open("signed_digest_file");
		arquivo.getline(buffer, 3999);
		if (send(novo_sock, buffer, (int)strlen(buffer), 0) < 0){
			perror("send");
			cout << "Erro ao enviar digest!"
				 << endl;
			close(novo_sock);
			exit(0);		
		}
		arquivo.close();
		
		//agora envia o arquivo criptografado com a chave publica do cliente;
		string[0] = '\0';
		strcat(string, "openssl rsautl -in arquivo.txt -out texto_cifrado.bin -pubin -inkey ");
		strcat(string, argv[1]);
		strcat(string, " -encrypt");
		system(string);
		
		arquivo.open("texto_cifrado.bin");
		arquivo.getline(buffer, 3999);
		if (send(novo_sock, buffer, (int)strlen(buffer), 0) < 0){
			perror("send");
			cout << "Erro ao enviar digest!"
				 << endl;
			close(novo_sock);
			exit(0);		
		}
		arquivo.close();
		
		close(novo_sock);
	}
	return 0;
}
