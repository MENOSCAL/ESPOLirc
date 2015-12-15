#include "include/connect_user.h"int main(int argc, char **argv){	//runtime 	runtimeDate  = malloc(70);	time_t rawtime;	struct tm * timeinfo;	time ( &rawtime );	timeinfo = localtime ( &rawtime );	sprintf(runtimeDate, "\nFecha de ejecucion del servidor : %s\n", asctime (timeinfo));		const int PORT = 7200;	int user_socket;		int listener = open_socket();	if(listener == -1){		printf("Error en listener \n");		return 1;	}	//puts("Socket created");	if(bind_to_port(listener, PORT)==-1)		return 1;		if(listen(listener,LISTENQ) == -1){		printf("No se puede conectar nadie mas \n");		return 1;	}	printf("Enlazando al puerto o [Presione CTRL+C para salir.]\n");	printf("Esperando al cliente\n");		struct sockaddr_in client;	socklen_t addres_size = sizeof(client);		user_list = empty_user_list();    pthread_mutex_init(&user_list_mutex, NULL);	User *new_user;		while((user_socket = accept(listener, (struct sockaddr*)&client, &addres_size))){		printf("Esperando al cliente\n");					//Reply to the client		char *message = "Hola Cliente, te pasare al manejador de conexion\n";        write(user_socket , message , strlen(message));				pthread_mutex_lock(&user_list_mutex);				user_list = add_user(user_list, DUMMY_USER, DUMMY_NICK, inet_ntoa(client.sin_addr),                             length(user_list), DUMMY_CHANNEL, user_socket);							 		pthread_mutex_unlock(&user_list_mutex);		new_user = user_list->payload;				if( pthread_create( &(new_user->thread), NULL ,  (void * (*) (void *)) connection_handler , new_user) < 0){            perror("Error no se puede crear el hilo");            return 1;        }        puts("Manejador asignado");				//char* msg = "hola";		//send(user_socket,msg,strlen(msg),0); /* Servidor envia un mensaje*/		//char server_reply[2000];		//recv(user_socket, server_reply , 2000 , 0); /*cliente responde y lo coloca en server_reply*/		//puts(server_reply); 	}		if(user_socket < 0){		printf("No se puede conectar socket secundario\n");		return 1;	}		return 0;	}/*Implementacion del socket*/int open_socket(){	int s = socket(AF_INET,SOCK_STREAM,0); /* AF = Direcci�n de la Familia, PF = Protocolo de Familia, SOCK_STREAM connection oriented*/	if(s == -1)		printf("Error al abrir socket\n");	return s;}int bind_to_port(int socket, int port){	//Prepare the sockaddr_in structure	struct sockaddr_in server;  		bzero(&server, sizeof(server));	server.sin_family	= AF_INET;	server.sin_port = (in_port_t)htons(port);	server.sin_addr.s_addr = htonl(INADDR_ANY);		/*Avoid probles reusing the port*/	int reuse = 1;	if(setsockopt(socket,SOL_SOCKET,SO_REUSEADDR,(char*)&reuse,sizeof(int))==-1){		perror("No es posible reusar el sockets\n");		return -1;	}	/* bind() elegir� un puerto aleatoriamente */    if( bind(socket,(struct sockaddr *)&server , sizeof(server)) < 0){		perror("No se puede enlazar al puerto : direccion ya esta en uso \n");		return -1;	}    //puts("bind done");	return 0;}