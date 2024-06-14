#include <time.h>
#include <stdbool.h>

void format_time_iso8601(time_t time, char *buffer, size_t buffer_size);
int last_available_position();
void print_user_table();
void add_user(const char* username, const char* ip, bool is_busy, bool is_connected, time_t last_connection);
int check_user_index(char*username);
void update_user_busy(char*username , bool isBusy);
void update_user_lastconnection(char*username,bool isConected);