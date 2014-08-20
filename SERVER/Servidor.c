#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#define len 100

//Struct que guardara los datos de los contactos
struct agen{
	char nombre[100];
	char ip[100];
	char puerto[100];
};

struct agen agend[len];
int indice = 0; //Determina el tamaño del struct

/*Al iniciar el programa revisa el archivo contactos.txt
si este tiene información la guarda en el struct agend, que
representa la agenda de contactos del usuario*/
void agenda(){
	FILE *archivo = fopen("contactos.txt","r");
	if(archivo != NULL){
		int i = 0;
		char leido[100];
		while(!feof(archivo)){
			fscanf(archivo,"%s",leido);
			strcpy(agend[i].nombre,leido);
			fscanf(archivo,"%s",leido);
			strcpy(agend[i].ip,leido);
			fscanf(archivo,"%s",leido);
			strcpy(agend[i].puerto,leido);
			i++;
		}
		indice = i;
	}
}
/*Configura el servidor para empezar a escuchar ,*/
void DatosUsuario(){/*
	char dato[len];
	FILE *archivo = fopen("informacion.txt","a");

	printf("\n **** Menú: Configuración **** \n \n");
	printf("a. Indique su nombre de usuario: ");
	scanf("%s",dato);
	fprintf(archivo, "%s",dato);
	printf("\n \033[1;31mImportante!!\nEl siguiente valor lo puede verificar en la información de su conexión\n\n");
	printf("\033[01;37mb. Indique su dirección IP: ");
	scanf("%s",dato);
	fprintf(archivo, " %s ",dato);
	printf("c. Indique su puerto: ");
	scanf("%s",dato);
	fprintf(archivo, "%s\n",dato);
	printf("\n \033[1;31m Configuración exitosa \n\033[01;37m");
	fclose(archivo);*/
	
}

/*Revisa el archivo infoserv.txt para determinar si 
el servidor ya cuenta con la configuración empezar a recivir y redirecionar msj*/
void verificaFile(){
	FILE *archivo=NULL;
	char* nombre = "infoserver.txt";
	char lectura[len];
	int tamanno;
	
	archivo = fopen(nombre, "r"); //abre el archivo infoserver.txt
	
	while(!feof(archivo)){
		fscanf(archivo,"%s",lectura); // lee cada linea del archivo
		tamanno = ftell(archivo); // determina el tamanno del archivo
	}

	// Si el tamnanno es igual a 0
	// solicita al usuario los datos requeridos
	if(tamanno == 0){
		printf("configuracion del servidor no disponible");
		DatosUsuario();
	}
	fclose(archivo);
}

/*Recupera la informacion del usuario, brindada en la configuración iniciar
para habilitar el puerto y ponerlo a escuchar; esta llama a servdor()*/
void recibirmjs(){
	char leido[len],dato[len];
	char *puerto[1];
	
	//recorre el archivo
	FILE *doc = fopen("infoserver.txt", "r");
	while(!feof(doc)){
		fscanf(doc,"%s",leido);
		strcpy(dato,leido);
		fscanf(doc,"%s",leido);
		strcpy(dato,leido);
		fscanf(doc,"%s",leido);
		strcpy(dato,leido);
	}
	
	puerto[0] = dato;
	//servidor(puerto);
	fclose(doc);
}

// se mantiene escuchando
/*void *client(void *var){
	int enviar = 0;
	while(enviar ==0)
	menuPrincipal();
		recibirmjs();
}*/
/ para enviar
void *serv(void *var){
	int recibir = 0;
	while(recibir ==0)
		
}

/*Funcion principal "main"
se usa for() para mantener procesos de escucha y envio de msj */

int main(){
	printf(" ************* SERVIDOR DE MENSAJERIA ************* \n\n");
	//verifica datos propios del archivo
	verificaFile();
	//guarda los contactos 
	agenda();
	//inicia el fork
	int band = fork();
	
	if(band>=0){
		if(band==0){
			//espera para enviar
			pthread_t servidor;
			char *msj = "servidor";
			pthread_create(&servidor,NULL,serv,(void*)msj);
			pthread_join(servidor,NULL);
		}
		//
		else{
			//canal de escuchar
			pthread_t cliente;
			char *msj1 = "cliente";
			pthread_create(&cliente,NULL,client,(void*)msj1);
			pthread_join(cliente,NULL);
		}
	}
	return 0;
}

