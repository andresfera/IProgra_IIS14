#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>

//Estructura para guardar la info de contactos
struct contacto{
	char nombre[100];
	char ip[100];
	char puerto[100];
};
//VARIABLES GLOBALES
struct contacto agenda[100]; //struct de structs para guardar una lista de contactos
int ultimoCont= 0; //posicion del ultimo contacto introducido
char *ip, *puerto;
////////////////////////////////////////////////////////////////////////////////////


//se toma la informacion de IP y puerto
configurar(){
	//recupera la ip
	struct sockaddr_in host;
	char nombre[255] ;
	gethostname(nombre, 255);
	host.sin_addr = * (struct in_addr*) gethostbyname(nombre)->h_addr;
	ip = inet_ntoa(host.sin_addr);
	//recupera el puerto del archivo de configuracion
	FILE *archivo = fopen("config.txt","r");
	if(archivo != NULL){
		char leido[30];
		while(!feof(archivo)){
			fscanf(archivo,"%s",leido);
			puerto= leido;}
	}
	printf("la ip es %s , el puerto: %s", ip, puerto);
}


//se guarda la info del servidor como primer contacto
conectar(){
	FILE *archivo = fopen("serverinfo.txt","r");
	if(archivo != NULL){
		char leido[30];
		while(!feof(archivo)){
			fscanf(archivo,"%s",leido);
			strcpy(agenda[0].nombre,leido);
			fscanf(archivo,"%s",leido);
			strcpy(agenda[0].ip,leido);
			fscanf(archivo,"%s",leido);
			strcpy(agenda[0].puerto,leido);
		}
		ultimoCont = 1;
	}
}


/*Funció principal: llama a la funcion conectar; ademas crea los procesos padre e hijo con el fork() y los hace infinitos*/ 
main(){
	//se corrobora la info del servidor 
	conectar();
	//se corrobora la info del usuario
	configurar();
	
	//se crea el proceso hijo apartir de la funcion fork()
	int process = fork();
	
	if(process>=0){
		if(process==0){
			while(0){
				//escucha();
				}
		}
		else{
			while(0){
				//menuPrincipal();
				}
		}
	}
}


