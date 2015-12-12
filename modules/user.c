#include "../include/user.h"

Node *empty_user_list() {
    Node *users         = malloc(sizeof(Node));
    users->next         = users;
    users->prev         = users;
    users->payload      = malloc(sizeof(User));
    User *user          = (User *) users->payload;
    user->id            = -1;
    return users;
};

int length(Node *list) {
    if(list == NULL || (list->payload)->id == -1) {
        return 0;
    }
    else {
        Node *first = list;
        Node *p     = first;
        int   i     = 1;
        p           = p->next;
        while(p != first) {
            p  = p->next;
            i += 1;
        };
        return i;
    };
};

Node *add_node(Node *list) {
    Node *node = malloc(sizeof(Node));
    if(list == NULL) {
        list          = node;
        list->prev    = list;
        list->next    = list;
    }
    else {
        node->next       = list;
        node->prev       = list->prev;
        list->prev->next = node;
        list->prev       = node;
        if(list->next == list) {
            list->next = node;
        }
        list = node;
    };
    list->payload = malloc(sizeof(User));
    return list;
};

Node *add_user(Node *list, char *name, char *hostname,
               int id, char *channel, int socket) {
    if(list == NULL) {
        return list;
    }
    else {
        User *user            = (User *) list->payload;
        if (user->id != -1) {
            list = add_node(list);
        };
        user                  = (User *) list->payload;
        user->id              = id;
        user->socket          = socket;
        user->name            = malloc(strlen(name) + 1);
        user->current_channel = malloc(strlen(channel) + 1);
        user->hostname        = malloc(strlen(hostname) + 1);
        strcpy(user->name, name);
        strcpy(user->hostname, hostname);
        strcpy(user->current_channel, channel);
        return list;
    };
};