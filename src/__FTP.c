#include "../headers/protocoles.h"

void call_ftp(const u_char *packet, int size, int verbosite)
{

    if (verbosite == 3)
    {
        printf("\n\n-----------FTP-----------\n\n");

        if (size > 0)
        {
            printf("FTP Header\n");
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
            printf("FTP: Data present\n");
        }
        else
        {
            printf("FTP: No Data\n");
        }
    }

    else
    {
        printf(" ,FTP\n");
    }
}