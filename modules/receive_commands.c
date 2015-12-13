#include "../include/user.h"
#include "../include/util.h"
#include "../include/text.h"
#include "../include/receive_commands.h"
//#include "../include/responses.h"
//#include "../include/errors.h"
#include "../include/commands.h"

void receive_nick(User *user, Node *users, char *name, char *send_line) {
    send_line = strset(":");
    send_line = stradd(send_line, user->name);
    send_line = stradd(send_line, "!");
    send_line = stradd(send_line, user->hostname);
    send_line = stradd(send_line, " ");
	if(name != NULL) {
		send_line = stradd(send_line, NICK);
		send_line = stradd(send_line, " :");
		send_line = stradd(send_line, name);
		send_line = stradd(send_line, "\n");
		user->hostname = strset(name);
	}
	
	/*
    if(name == NULL) {
        send_line = stradd(send_line, ERR_NONICKNAMEGIVEN);
        send_line = stradd(send_line, NONICKNAMEGIVEN);
        send_line = stradd(send_line, "\n");
    }
    else if(get_user_by_name(users, name) != NULL) {
        send_line = stradd(send_line, ERR_NICKNAMEINUSE);
        send_line = stradd(send_line, " ");
        send_line = stradd(send_line, name);
        send_line = stradd(send_line, " ");
        send_line = stradd(send_line, name);
        send_line = stradd(send_line, "\n");
    }
    else {
        send_line = stradd(send_line, NICK);
        send_line = stradd(send_line, " :");
        send_line = stradd(send_line, name);
        send_line = stradd(send_line, "\n");
        user->name = strset(name);
        send_all(user->current_channel, send_line, users);
        return;
    };
	
	*/
    write(user->socket, send_line, strlen(send_line));
	/*
    if(strcmp(user->current_channel, DUMMY_CHANNEL) != 0) {
        send_names(user, users, send_line);
    };
	*/
}

void receive_user(User *user, char *send_line) {
    send_line = stradd(send_line, WELCOME);
    send_line = stradd(send_line, user->name);
    send_line = stradd(send_line, WELCOME_HOSTNAME);
    send_line = stradd(send_line, user->hostname);
    send_line = stradd(send_line, "\n");

    write(user->socket, send_line, strlen(send_line));
};
