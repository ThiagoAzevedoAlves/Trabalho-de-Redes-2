#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstdlib>

using namespace std;

int main(int argc, char *argv[]){
	
	if (argc != 2){
		cout << "Argumentos inválidos: ./cliente <ip>" << endl;
		exit(1);
	}
	int sock, numbytes;
	struct sockaddr_in serv_addr;
	char buffer[255];
	
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		//perror("socket");
		cout << "Erro ao criar porta!"
			 << endl;
		exit(1);
	}
	
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(5555);
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
			 //perror("connect");
			 cout << "Erro ao conectar com o servidor!" 
				  << endl;
			 exit(1);
	};
	if ((numbytes=recv(sock, buffer,254, 0)) < 0) {
		 //perror("recv");
		 cout << "Erro ao receber arquivo do servidor!"
			  << endl;
		 exit(1);
	}
	buffer[numbytes] = '\0';
	cout << "recebeu do servidor: "
		 << buffer
		 << endl;

	close(sock);
	
	return 0;
}
