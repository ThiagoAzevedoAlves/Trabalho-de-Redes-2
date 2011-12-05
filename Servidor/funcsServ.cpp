#include <cstdlib>
#include <string.h>
#include <iostream>
#include <fstream>
#include "funcsServ.h"

using namespace std;

int existe(char* pedido_char){
	ifstream lista; //arquivo que verifica se existe o arquivo na lista de arquivos para baixar;
	char arquivo[50]; //string que percorre a lista de arquivo guardando os nomes;
	system("cd ..");
	system("cd arquivos");
	system("ls > lista.txt");
	lista.open("lista.txt");
	while (lista.getline(arquivo, 50)){ //percorre a lista de arquivos
		if (strcmp(arquivo, pedido_char) == 0){
			return 0;
		}
	}
	return 1;
}
