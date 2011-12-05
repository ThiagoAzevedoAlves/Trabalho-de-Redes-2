#include <iostream>
#include <fstream>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstdlib>

using namespace std;

int main(int argc, char *argv[]){
	
	if (argc != 3){
		cout << "Argumentos inválidos: ./cliente <ip do servidor> <chave privada>" << endl;
		exit(1);
	}
	int sock, numbytes;
	struct sockaddr_in serv_addr;
	char buffer[4000];
	char string[256];
	ofstream arquivo;
	
	
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		cout << "Erro ao criar porta!"
			 << endl;
		exit(1);
	}
	
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(5678);
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	
	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) { //conecta com o servidor
			 cout << "Erro ao conectar com o servidor!" 
				  << endl;
			 exit(1);
	};
	
	//agora recebe a hash do arquivo ------------------------------------------------------------------------------------------------------ //
	
	arquivo.open("hash.txt");
	
	numbytes=recv(sock, buffer,3999, 0);
	buffer[numbytes] = '\0';
	arquivo << buffer;
	
	arquivo.close();
	//até aqui é recebendo a hash e salvando no arquivo hash.txt --------------------------------------------------------------------------- //
	
	//agora recebe a hash criptografada com a chave privada do servidor -------------------------------------------------------------------- //
	
	arquivo.open("signed_digest_file");
	
	numbytes=recv(sock, buffer,3999, 0);
	buffer[numbytes] = '\0';
	arquivo << buffer;
	arquivo.close();
	
	//agora recebe o arquivo criptografado com a chave publica do cliente e desencriptografa com a chave privada -----------------------------//
	
	arquivo.open("texto_cifrado.bin");
	
	numbytes=recv(sock, buffer,3999, 0);
	buffer[numbytes] = '\0';
	arquivo << buffer;
	arquivo.close();
	
	close(sock);
	
	string[0] = '\0';
	strcat(string, "openssl rsautl -in texto_cifrado.bin -out arquivo.txt  -inkey ");
	strcat(string, argv[2]);
	strcat(string, " -decrypt");
	system(string);
	
	// ok!
	return 0;
}
