#include "user.h"


/*==================================================================================*/
 /* imprime la tabla de usuarios que maniene el servidor*/
 /*================================================================================*/
void print_user_table() {
    printf("User Table:\n");

    for (int i = 0; i < USERS_MAX_COUNT; i++) {

            
            if(strlen(user_table[i].username) > 0 ){
                    char last_connection_str[20];

            format_time_iso8601(user_table[i].last_connection, last_connection_str, sizeof(last_connection_str));
            printf("User %d:\n", i);
            printf("  Username: %s\n", user_table[i].username);
            printf("  IP: %s\n", user_table[i].ip);
            printf("  Busy: %s\n", user_table[i].is_busy ? "Yes" : "No");
            printf("  Connected: %s\n", user_table[i].is_connected ? "Yes" : "No");
            printf("  Last Connection: %s\n", last_connection_str);
            }
            
        
    }
}

/*==================================================================================*/
 /* agrega un nuevo usuario al servidor*/
 /*================================================================================*/
void add_user(const char* username, const char* ip, bool is_busy, bool is_connected, time_t last_connection) {
    
    int index = last_available_position();

    if (index == -1) {
        printf("Invalid index\n");
        return;
    }


    strncpy(user_table[index].username, username, USERS_MAX_COUNT - 1);
    user_table[index].username[USERS_MAX_COUNT - 1] = '\0';

    strncpy(user_table[index].ip, ip, MAX_USER_IP_LENGTH  - 1);
    user_table[index].ip[MAX_USER_IP_LENGTH  - 1] = '\0';
    user_table[index].is_busy = is_busy;
    user_table[index].is_connected = is_connected;
    user_table[index].last_connection = last_connection;
}

/*==================================================================================*/
 /* devuelve el indice del usuario , si no existe devuelve 99r*/
 /*================================================================================*/
int check_user_index(char*username){

    for(int i=0 ; i< USERS_MAX_COUNT;i++)
    {
            if(user_table[i].username == username){
                    return i;
            }
    }

    return 99;
}

/*==================================================================================*/
 /* actualiza el estado , si el usuario se encuentra en una conversaicon o no*/
 /*================================================================================*/
void update_user_busy(char*username , bool isBusy){

    int index = check_user_index(username);
    user_table[index].is_busy = isBusy;

}

/*==================================================================================*/
 /* setea un timestamp que corresponde con la ultima conexion del usuario*/
 /*================================================================================*/
void update_user_lastconnectgion(char*username){
    int index = check_user_index(username);
    user_table[index].last_connection = time(NULL);

}

/*==================================================================================*/
 /* devuelve la ultima posicion disponible en la tabla de usuarios*/
 /*================================================================================*/
int last_available_position() {
    for (int i = 0; i < USERS_MAX_COUNT; i++) {
        if (strlen(user_table[i].username) == 0) {
            return i; 
        }
    }
    return -1;
}