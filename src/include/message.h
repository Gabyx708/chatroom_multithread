#include "proto.h"

struct  message
{
    char from[IP_LENGHT];
    char to[IP_LENGHT];
    int type;
    char payload[MAX_MSG_LENGHT];
};

