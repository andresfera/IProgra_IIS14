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

//estructura con la informacion para el envio de msj atraves del servidor
struct paquete{
	struct contacto destinatario;
	char msj[100];
	FILE archivo;
};
	
	
//VARIABLES GLOBALES
struct contacto agenda[100]; //struct de structs para guardar una lista de contactos
int ultimoCont= 0; //posicion del ultimo contacto introducido
char *ip, *puerto; //informacion del programa usuario
////////////////////////////////////////////////////////////////////////////////////

//"Enviar"
//envia el mensajes; funcion de CLIENTE dentro del usuario
int cliente(struct contacto destino){
	char *puerto, *ip;//variables para la dirrecion IP y # de puerto del SERVIDOR DEL PROGRAMA O SERVICIO
	puerto = agenda[0].puerto;
	ip = agenda[0].ip;
	//struct con la informacion del DESTINATARIO FINAL
	
	int socket_conex; // ID del socket de conexion
	int conexion; //boolean para verificar la conexion
	struct sockaddr_in receptorAddr;  // Estructura para la direccion del mensaje

//-----------------------------------------------------------------------------------
//Crea el socket
	socket_conex = socket(AF_INET, SOCK_STREAM, 0); //Pide el socket tcp/ip
	receptorAddr.sin_family = AF_INET; // Construccion de direccion del receptor
	
	//conversion de char a dirrecion de ip (bytes de red)
	inet_pton(AF_INET,ip,&(receptorAddr.sin_addr.s_addr));
	receptorAddr.sin_port= htons(atoi(puerto));

//-----------------------------------------------------------------------------------
//Conexion con el socket 
	conexion = connect(socket_conex, (struct sockaddr *) &receptorAddr, sizeof(receptorAddr));
	if(conexion <0){ 
		perror("Error de conexion" );
		exit(1);
	}

	conexion = send(socket_conex,destino.nombre,strlen(destino.nombre)+1,0); 
	//Verifica la conexion con el socket, para el envio de mensajes
	if(conexion <0){  
		perror("Falló la conexion. Lo sentimos msj no enviado"); 
		close(socket_conex);
		exit(1);
	}

	char msj[100]; //mensaje del usuario (emisor)
	printf("\033[01;33mMsj por enviar: ");//Solicita al usuario el mensaje que desea enviar
	scanf(" %[^\n]", 100);	//Lee el mensaje ingresado y lo guarda en la variable mensaje
	
	//Proceso para enviar el mensaje
	conexion = send(socket_conex, msj, strlen(msj)+1, 0);
	if(conexion <0){ 
		perror("\033[01;37mFalló la conexion. Lo sentimos msj no enviado"); 
		close(socket_conex);
		exit(1);
	}
	else{
		printf("\033[01;37m\nEnviado\n");
	}
} 

//"ESCUCHAR"
//recibe mensajes; funcion de SERVIDOR dentro del usuario
int servidor (){
	int socket_conex; //ID del socket de conexion
	//boolean para verificar la conexion, largo de la direccion IP
	//largo del mensaje
	int conexion, clienteAddrLen, msjLen; 
	struct sockaddr_in clienteAddr, servidorAddr;
	char msj[100]; //mensaje recivido
//-----------------------------------------------------------------------------------
//crea el socket
	socket_conex = socket(AF_INET, SOCK_STREAM, 0); //Pide el socket TCP/IP
	
	//conversion de char a dirrecion de ip (bytes de red)
	servidorAddr.sin_family = AF_INET; 
	servidorAddr.sin_addr.s_addr = htonl(INADDR_ANY); 
	servidorAddr.sin_port = htons(atoi(puerto)); 

	//Union del socket con esta direccion
	if(bind(socket_conex, (struct sockaddr *) &servidorAddr, sizeof(servidorAddr))<0){
		perror("Error. No se puede conectar al puerto");
		exit(1);
	}
//-----------------------------------------------------------------------------------
//Ciclo infinito donde el puerto se matiene escuchando
	while(1){
		listen(socket_conex,10); //Le indica al socket que espere conexiones
		clienteAddrLen= sizeof(clienteAddr);
		//verifica si la conexion con el socket es correcta
		conexion= accept(socket_conex, (struct sockaddr *) &clienteAddr, &clienteAddrLen);
		if(conexion <0){
			perror("Error. Conexion no aceptada");
			exit(1);
		}
		
		memset(msj,0x0,100); //inicia la linea de escucha

		msjLen = recv(conexion, msj, 100, 0); // Espera que lleguen todos los datos
		if(msjLen < 0){
			perror("Error en la recepción de datos");
			exit(1);
		}
		// Cuando ya tiene todos los datos recibidos, los muestra en pantalla
		printf("\033[01;32m\n\nNuevo msj de: %s",msj);
		// ----------------

		//Recibe los mensajes enviados
		msjLen = recv(conexion, msj, 100, 0); // Espera que lleguen todos los datos
		if(msjLen < 0){
			perror("Error en la recepcion de datos");
			exit(1);
		}
		printf("\n%s\n\033[01;37m", msj);
		
	}
	return 0;
}

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
				//servidor_recibe
				//escucha();
				servidor();
				}
		}
		else{
			while(0){
				//cliente_envia
				//menuPrincipal();
				int opcion;
				printf("\033[01;37m\n **** Menu principal ****\n\n");
				printf("1. Enviar mensaje\n");
				printf("2. Ver amigos\n");
				printf("4. Salir\n");
				printf("\n Ingrese el número de la opcion deseada: ");
				scanf("%d",&opcion); // lee la opcion que el usuario ingreso y lo guarda en la variable opcion
				//cliente();
				}
		}
	}
}


