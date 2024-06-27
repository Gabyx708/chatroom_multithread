#ifndef USER_H
#define USER_H

#include <time.h>
#include <stdbool.h>

#define MAX_USER_IP_LENGTH 16
#define USERS_MAX_COUNT 10

struct User {
    char username[20];
    char ip[MAX_USER_IP_LENGTH];
    int port;
    bool is_busy;
    bool is_connected;
    time_t last_connection;
};

extern struct User user_table[USERS_MAX_COUNT];

#endif /* USER_H */
