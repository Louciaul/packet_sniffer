#include "../headers/protocoles.h"

void call_telnet(const u_char *packet, int size, int verbosite)
{

    if (verbosite == 3)
    {
        printf("\n\n-----------TELNET-----------\n\n");

        if (size == 0)
        {
            printf("No Data\n");
            return;
        }

        int k = 0;

        // Quelques sous options qui s'affichent un peu mal
        // Le premier caractère est parfois invisible sur le terminal
        // Si on print une option on print le code tout de suite
        // derrière (et on avance donc de 2)

        while (size > 0)
        {
            switch (packet[k])
            {
            case IAC:
                printf("\n IAC ");
                break;
            case DONT:
                printf("DONT ");
                k++;
                size--;
                printf("%d ", packet[k]);
                break;
            case DO:
                printf("DO ");
                k++;
                size--;
                printf("%d ", packet[k]);
                break;
            case WONT:
                printf("WONT ");
                k++;
                size--;
                printf("%d ", packet[k]);
                break;
            case WILL:
                printf("WILL ");
                k++;
                size--;
                printf("%d ", packet[k]);
                break;
            case SB:
                printf("SB ");
                break;
            case GA:
                printf("GA ");
                break;
            case EL:
                printf("EL ");
                break;
            case EC:
                printf("EC ");
                break;
            case AYT:
                printf("AYT ");
                break;
            case AO:
                printf("AO ");
                break;
            case IP:
                printf("IP ");
                break;
            case BREAK:
                printf("BREAK ");
                break;
            case DM:
                printf("DM ");
                break;
            case NOP:
                printf("NOP ");
                break;
            case SE:
                printf("SE ");
                break;
            case EOR:
                printf("EOR ");
                break;
            case ABORT:
                printf("ABORT ");
                break;
            default:
                printf("%c ", packet[k]);
            }
            fflush(stdout);
            size--;
            k++;
        }

        printf("\n");
    }

    else if (verbosite == 2)
    {
        if (size > 0)
        {
            if (packet[0] == IAC)
            {
                printf("Telnet: Commands\n");
            }
            else
            {
                printf("Telnet: Only Data (no command)\n");
            }
        }
        else
        {
            printf("Telnet: No Data\n");
        }
    }

    else
    {
        printf(" Telnet\n");
    }
}