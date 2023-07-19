#include "../headers/protocoles.h"

void call_ip4(const u_char *packet, int verbosite)
{

    struct iphdr *head = (struct iphdr *)packet;

    if (verbosite == 3)
    {
        printf("\n\n-----------IP-----------\n\n");

        printf("IP version: %d\n", head->version);

        if (head->version == 4)
        {

            printf("IHL: %d\n", head->ihl);
            printf("Type Of Service: %08x\n", ntohs(head->tos));
            printf("Total length: %i bytes\n", ntohs(head->tot_len));
            printf("Identification: %i\n", ntohs(head->id));
            printf("Flags : ");

            int nb_flags = 0;

            if (head->frag_off & 1 << 14)
            {
                printf("DF ");
                nb_flags++;
            }
            if (head->frag_off & 1 << 13)
            {
                printf("MF\n");
                nb_flags++;
            }
            if (nb_flags == 0)
            {
                printf("No flags");
            }

            printf("\n");

            uint16_t frag_off = head->frag_off & ~(0b111 << 12);

            printf("Fragment Offset: %i\n", frag_off);
            printf("Time To Live (TTL): %i\n", head->ttl);

            if (head->protocol == TCP)
            {
                printf("Protocol: TCP\n");
            }
            else if (head->protocol == UDP)
            {
                printf("Protocol: UDP\n");
            }
            else
            {
                printf("Protocol unknown\n");
            }

            printf("Adress source: %s\n", inet_ntoa(*(struct in_addr *)&head->saddr));
            printf("Adress destination: %s\n", inet_ntoa(*(struct in_addr *)&head->daddr));
        }
        else if (head->version == 6)
        {
            call_ip6(packet, verbosite);
        }
        else
        {
            printf("Ip version: %d not implemented\n", head->version);
        }
    }

    else if (verbosite == 2)
    {
        printf("IP: version: %d,Adress source: %s,Adress destination: %s\n",
               head->version, inet_ntoa(*(struct in_addr *)&head->saddr), inet_ntoa(*(struct in_addr *)&head->daddr));
    }

    else
    {
        printf("Adress source: %s,Adress destination: %s,",
               inet_ntoa(*(struct in_addr *)&head->saddr), inet_ntoa(*(struct in_addr *)&head->daddr));
    }

    int payload_size = ntohs(head->tot_len) - head->ihl * 4;

    // Puis biffurcation des protocoles

    if (head->protocol == TCP)
    {
        call_TCP(packet + head->ihl * 4, payload_size, verbosite);
    }

    if (head->protocol == UDP)
    {
        call_UDP(packet + head->ihl * 4, payload_size, verbosite);
    }
}