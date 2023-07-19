#include "../headers/protocoles.h"

void call_UDP(const u_char *packet, int size, int verbosite)
{

    struct udphdr *head_udp = (struct udphdr *)packet;

    if (verbosite == 3)
    {
        printf("\n\n--------------UDP--------------\n\n");
        printf("Port source number: %d\n", ntohs(head_udp->uh_sport));
        printf("Port destination number: %d\n", ntohs(head_udp->uh_dport));
        printf("Lentgh (UDP header + Data): %d\n", ntohs(head_udp->len));
        printf("Checksum: %d\n", ntohs(head_udp->check));
    }

    else if (verbosite == 2)
    {
        printf("UDP: Port source number: %d, ", ntohs(head_udp->uh_sport));
        printf("Port destination number: %d\n", ntohs(head_udp->uh_dport));
    }

    int payload_size = size - 8;

    const u_char *next_packet = packet + sizeof(struct uphdr *);

    uint16_t source = ntohs(head_udp->uh_sport);
    uint16_t dest = ntohs(head_udp->uh_dport);

    if (dest == BOOTPC || source == BOOTPC)
    {
        call_bootp(next_packet, payload_size, verbosite);
    }
    else if (dest == BOOTPS || source == BOOTPS)
    {
        call_bootp(next_packet, payload_size, verbosite);
    }
    else if (dest == DNS || source == DNS)
    {
        call_dns(verbosite);
    }
    else
    {
        if (verbosite == 3 || verbosite == 2)
        {
            printf("\nProtocol: not implemented");
        }
    }
}