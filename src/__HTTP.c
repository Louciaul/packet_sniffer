#include "../headers/protocoles.h"

void call_http(const u_char *packet, int size, int verbosite)
{

    if (verbosite == 3)
    {
        printf("\n\n-----------HTTP-----------\n\n");

        if (size > 0)
        {
            printf("HTTP Header\n");
            printf("Data: \n");

            for (int k = 0; k < size; k++)
            {
                if (isprint(packet[k]))
                {
                    printf("%c", packet[k]);
                }
                else
                {
                    printf(".");
                }
            }

            printf("\n");
        }
        else
        {
            printf("No Data\n");
        }
    }
    else if (verbosite == 2)
    {
        if (size > 0)
        {
            printf("HTTP: Data present\n");
        }
        else
        {
            printf("HTTP: No Data\n");
        }
    }

    else
    {
        printf(" ,HTTP\n");
    }
}