#include "../headers/protocoles.h"

void call_smtp(const u_char *packet, int size, int verbosite)
{

    if (verbosite == 3)
    {
        printf("\n\n-----------SMTP-----------\n\n");

        if (size > 0)
        {
            printf("SMTP Header\n");
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

        printf("\n");
    }
    else if (verbosite == 2)
    {
        if (size > 0)
        {
            printf("SMTP: Data present\n");
        }
        else
        {
            printf("SMTP: No Data\n");
        }
    }

    else
    {
        printf(" ,SMTP\n");
    }
}