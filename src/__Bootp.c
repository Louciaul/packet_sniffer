#include "../headers/protocoles.h"
#include "../headers/bootp.h"

void call_bootp(const u_char *packet, int size, int verbosite)
{

    (void)size;
    struct bootp_hdr *head_bootp = (struct bootp_hdr *)packet;

    if (verbosite == 3)
    {
        // Affichage du contenu de la trame

        printf("\n\n-----------BOOTP-----------\n\n");

        printf("Packet opcode type: %i\n", head_bootp->op);
        printf("Hardware addr type: %i\n", head_bootp->htype);
        printf("Hardware addr length: %i\n", head_bootp->hlen);
        printf("Gateway hops: %i\n", head_bootp->hops);
        printf("Transaction id: 0x%04X\n", ntohl(head_bootp->xid));
        printf("Seconds since boot began: %i\n", ntohs(head_bootp->secs));
        printf("Bootp flags: 0x%02X\n", head_bootp->flags);
        printf("Client IP address: %s\n", inet_ntoa(head_bootp->ciaddr));
        printf("'Your' IP address: %s\n", inet_ntoa(head_bootp->yiaddr));
        printf("Server IP address: %s\n", inet_ntoa(head_bootp->siaddr));
        printf("Gateway IP address: %s\n", inet_ntoa(head_bootp->giaddr));
        printf("Client address MAC: %02x:%02x:%02x:%02x:%02x:%02x\n",
               head_bootp->chaddr[0],
               head_bootp->chaddr[1],
               head_bootp->chaddr[2],
               head_bootp->chaddr[3],
               head_bootp->chaddr[4],
               head_bootp->chaddr[5]);

        if (head_bootp->sname[0] == 0)
        {
            printf("Server host name not given\n");
        }
        else
        {
            printf("Server host name: %s\n", (char *)head_bootp->sname);
        }

        if (head_bootp->file[0] == 0)
        {
            printf("Boot file name not given\n");
        }
        else
        {
            printf("Boot file name: %s\n", (char *)head_bootp->file);
        }

        // Magic cookie for DHCP

        if (head_bootp->vend[0] == 99 && head_bootp->vend[1] == 130 && head_bootp->vend[2] == 83 &&
            head_bootp->vend[3])
        {
            printf("Magic Cookie\n");
            printf("\nDHCP HEADER\n\n");

            struct bootp_option *opt = (struct bootp_option *)(head_bootp->vend + 4);

            while (1)
            {
                printf("%s: ", bootp_options_name[opt->code].name);

                if (opt->code == TAG_END)
                {
                    printf("\n");
                    return;
                }

                if (opt->code == TAG_DHCP_MESSAGE)
                {
                    printf("%s\n", dhcp_messages[opt->data[0]]);
                }
                else
                {
                    switch (bootp_options_name[opt->code].displaytype)
                    {
                    case (PRINT_STR):
                        fwrite(opt->data, opt->len, 1, stdout);
                        break;
                    case (PRINT_IP):
                        printf("%s", inet_ntoa(*(struct in_addr *)opt->data));
                        break;
                    case (PRINT_UINT_32):
                        printf("%u", ntohl(*((u_int32_t *)opt->data)));
                        break;
                    case (PRINT_HEX):
                        for (int k = 0; k < opt->len; k++)
                        {
                            printf("%02x", opt->data[k]);
                        }
                        break;
                    default:
                        for (int k = 0; k < opt->len; k++)
                        {
                            printf("%d", opt->data[k]);
                        }
                        break;
                    }
                    printf("\n");
                }
                // on reprend notre point de départ et on avance de la taille de bootp_option
                opt = (struct bootp_option *)(head_bootp->vend + 4 + opt->len + 2);
            }
        }
    }

    else if (verbosite == 2)
    {
        if (head_bootp->file[0] == 0)
        {
            printf("BOOTP: Boot file name not given");
        }
        else
        {
            printf("BOOTP: Boot file name: %s ", (char *)head_bootp->file);
        }
        if (head_bootp->vend[0] == 99 && head_bootp->vend[1] == 130 && head_bootp->vend[2] == 83 &&
            head_bootp->vend[3])
        {
            printf(" DHCP\n");
        }
        else
        {
            printf("\n");
        }
    }
    else
    {
        if (head_bootp->vend[0] == 99 && head_bootp->vend[1] == 130 && head_bootp->vend[2] == 83 &&
            head_bootp->vend[3])
        {
            printf(" DHCP\n");
        }
        else
        {
            printf(" BOOTP only\n");
        }
    }
}