#include <stdio.h>
#include "message_functions.h"

/*==================================================================================*/
 /* imprime informacion de un mensaje*/
 /*================================================================================*/
void print_message(struct message msg)
{
    printf("======MESSAGE=======\n");
    printf("content: %s",msg.payload);
}