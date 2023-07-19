#include "../headers/protocoles.h"

void call_https(int verbosite)
{
    if (verbosite == 3 || verbosite == 2)
    {
        printf("Protocol HTTPS (secured)\n");
    }

    else
    {
        printf(" HTTPS\n");
    }
}