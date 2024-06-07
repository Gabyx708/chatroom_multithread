#include <time.h>
#include <stdbool.h>

#ifndef USER_H
#define USER_H

#define MAX_USER_NAME_LENGTH 30
#define MAX_USER_IP_LENGTH 16
#define USERS_MAX_COUNT 10

typedef struct {
    char username[MAX_USER_NAME_LENGTH];
    char ip[MAX_USER_IP_LENGTH];
    bool is_busy;
    bool is_connected;
    time_t last_connection;
} User;

User user_table[USERS_MAX_COUNT];


void print_user_table();

#endif /* USER_H */
