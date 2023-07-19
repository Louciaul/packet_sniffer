#include "../headers/protocoles.h"

void call_imap(const u_char *packet, int size, int verbosite)
{

    if (verbosite == 3)
    {
        printf("\n\n-----------IMAP-----------\n\n");

        if (size > 0)
        {
            printf("IMAP Header\n");
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
            printf("IMAP: Data present\n");
        }
        else
        {
            printf("IMAP: No Data\n");
        }
    }

    else
    {
        printf(" ,IMAP\n");
    }
}