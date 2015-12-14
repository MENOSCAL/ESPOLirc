#include "../include/user.h"
#include "../include/util.h"
#include "../include/text.h"
#include "../include/receive_commands.h"
//#include "../include/responses.h"
//#include "../include/errors.h"
#include "../include/commands.h"

void receive_nick(User *user, Node *users, char *nick, char *send_line) {
	if(nick == NULL){
		send_line = strset("Nick no ingresado!!!, Ingrese un nick. \n");
		write(user->socket, send_line, strlen(send_line));
		return;
	}
	if(get_user_by_nick(users, nick) != NULL){
		send_line = strset("Este Nick ya existe!!!, Ingrese un nuevo nick. \n");
		write(user->socket, send_line, strlen(send_line));
		return;
	}
	
	user->nick = strset(nick);
	
	send_line = strset(":");
    send_line = stradd(send_line, user->nick);
    send_line = stradd(send_line, "!");
    send_line = stradd(send_line, user->hostname);
    send_line = stradd(send_line, " ");
	
	send_line = stradd(send_line, NICK);
	send_line = stradd(send_line, " :");
	send_line = stradd(send_line, user->nick);
	send_line = stradd(send_line, "\n");
	send_all(user->current_channel, send_line, users);	
}

void receive_user(User *user, char *name, char *send_line) {
    send_line = stradd(send_line, WELCOME);
	if(name != NULL)
		user->name = strset(name);
    send_line = stradd(send_line, user->name);
	send_line = stradd(send_line, "\n");
    send_line = stradd(send_line, WELCOME_NICK);
    send_line = stradd(send_line, user->nick);
    send_line = stradd(send_line, "\n");

    write(user->socket, send_line, strlen(send_line));
}

void receive_list(User *user, Node *users, char *send_line) {
	char *channel;
	Node *first  = users;
    Node *p      = users;
    User *target = (User *) p->payload;
char *prim_line = strset("Los canales son:: ");
char arreglo[10][20];
char aux[20];
int i, j, k;
int l=0;

//if(strcmp(target->current_channel, DUMMY_CHANNEL) != 0){
	strcpy(arreglo[l++], target->current_channel);	
//}

	//send_line = stradd(send_line, target->current_channel);
	channel = target->current_channel;
    p = p->next;
    target = (User *) p->payload;

	
    while(p != first) {
       // if(strcmp(target->current_channel, channel) != 0){
			//send_line = stradd(send_line, ",");
			//send_line = stradd(send_line, target->current_channel);
	//	}	
	//if(strcmp(target->current_channel, DUMMY_CHANNEL) != 0){
		strcpy(arreglo[l++], target->current_channel);		
	//}	
        p = p->next;
        target = (User *) p->payload;
    }
	    // ORDENAR CADENAS
    for(i=0; i<l-1; i++)
    {
        k=i;
        strcpy(aux, arreglo[i]);
        for(j=i+1; j<l; j++)
        {
            if(strcmp(arreglo[j], aux)<0)
            {
                k=j;
                strcpy(aux, arreglo[j]);
               //permite hacer una copia auxiliar de la cadena arreglo[j];
            }
        }
        strcpy(arreglo[k],arreglo[i]);
        strcpy(arreglo[i],aux);
    }






//	char *tmp=strtok(send_line, ",\t\r\n");
//	while(tmp != NULL){
//		if(strcmp(target->current_channel, tmp) == 0){
//			i++;
//		}
		/* Extraemos la siguiente palabra */
//		tmp=strtok(NULL, ",\t\r\n");
//	}


//strcpy(arrreglo[l++], arreglo);	
send_line = stradd(prim_line, *arreglo);
for(i=1;i<l;i++){
if(strcmp(arreglo[i], arreglo[i-1]) != 0){
	send_line = stradd(send_line, ", ");
	send_line = stradd(send_line, arreglo[i]);

}
}
	send_line = stradd(send_line, "\n");
    write(user->socket, send_line, strlen(send_line));
}
void receive_join(User *user, Node *users, char *channel, char *send_line) {
	if(channel == NULL){
		send_line = strset("Channel no ingresado!!!, Ingrese un channel. \n");
		write(user->socket, send_line, strlen(send_line));
		return;
	}
	if(strcmp(user->nick, NICK) == 0){
		send_line = strset("Nick no ingresado!!!, Ingrese un nick. \n");
		write(user->socket, send_line, strlen(send_line));
		return;
	}
    send_line = strset(":");
    send_line = stradd(send_line, user->nick);
    send_line = stradd(send_line, "!");
    send_line = stradd(send_line, user->hostname);
    send_line = stradd(send_line, " ");
    send_line = stradd(send_line, JOIN);

    send_line = stradd(send_line, " #");
    send_line = stradd(send_line, channel);
    send_line = stradd(send_line, "\n");

    user->current_channel = strset(channel);
    write(user->socket, send_line, strlen(send_line));
    send_names(user, users, send_line);
}

void receive_part(User *user, Node *users, char *send_line) {
    send_line = strset(":");
    send_line = stradd(send_line, user->nick);
    send_line = stradd(send_line, "!");
    send_line = stradd(send_line, user->hostname);
    send_line = stradd(send_line, " ");
    send_line = stradd(send_line, PART);

    send_line = stradd(send_line, " #");
    send_line = stradd(send_line, user->current_channel);
    send_line = stradd(send_line, "\n");

    send_all(user->current_channel, send_line, users);
    user->current_channel = strset(DUMMY_CHANNEL);
}

void receive_privmsg(User *user, Node *users, char *send_line, char *message) {
    char *line, *channel, *word;

    line           = malloc(strlen(message) + 1);
    line           = strcpy(line, message);

    strtok(line, " \t\r\n/");
    channel        = strtok(NULL, " #\t\r\n/");
	
	if(strlen(channel)==2){
		send_line = strset("Channel no ingresado!!!, Ingrese un channel. \n");
		write(user->socket, send_line, strlen(send_line));
		return;
	}	

    send_line      = strset(":");
    send_line      = stradd(send_line, user->nick);
    send_line      = stradd(send_line, "!");
    send_line      = stradd(send_line, user->hostname);
    send_line      = stradd(send_line, " ");
    send_line      = stradd(send_line, PRIVMSG);

    send_line      = stradd(send_line, " #");
    send_line      = stradd(send_line, channel);
    send_line      = stradd(send_line, " :");

    word           = strtok(NULL, " :\t\r\n/");

    while (word   != NULL) {
        send_line  = stradd(send_line, word);
        send_line  = stradd(send_line, " ");
        word       = strtok(NULL, " \t\r\n/");
    }
	send_line      = stradd(send_line, "\n");
    send_others(user->nick, channel, send_line, users);
}
