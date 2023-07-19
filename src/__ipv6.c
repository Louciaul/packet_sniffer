#include "../headers/protocoles.h"

void call_ip6(const u_char *packet, int verbosite)
{
    struct ip6_hdr *head = (struct ip6_hdr *)packet;

    if (verbosite == 3)
    {
        printf("\n\n-----------IP-----------\n\n");

        printf("\nEntête IP:\n");

        uint32_t flow = ntohl(head->ip6_ctlun.ip6_un1.ip6_un1_flow);

        printf("Flow: first 4 bits version, next 8 bits Traffic class,next 20 bits Flow Label %08x\n", flow);

        printf("Payload lentgh: %d\n", ntohs(head->ip6_ctlun.ip6_un1.ip6_un1_plen));

        if (head->ip6_ctlun.ip6_un1.ip6_un1_nxt == TCP)
        {
            printf("Next head: TCP\n");
        }
        else if (head->ip6_ctlun.ip6_un1.ip6_un1_nxt == UDP)
        {
            printf("Next head: UDP\n");
        }
        else
        {
            printf("Next head: %02x\n", head->ip6_ctlun.ip6_un1.ip6_un1_nxt);
        }

        printf("Hop limit: %d\n", head->ip6_ctlun.ip6_un1.ip6_un1_hlim);

        char str[INET6_ADDRSTRLEN];
        inet_ntop(AF_INET6, &head->ip6_src, str, INET6_ADDRSTRLEN);
        printf("Ip source: %s\n", str);

        inet_ntop(AF_INET6, &head->ip6_dst, str, INET6_ADDRSTRLEN);
        printf("Ip destination: %s\n", str);
    }

    else if (verbosite == 2)
    {
        char src[INET6_ADDRSTRLEN];
        inet_ntop(AF_INET6, &head->ip6_src, src, INET6_ADDRSTRLEN);
        char dst[INET6_ADDRSTRLEN];
        inet_ntop(AF_INET6, &head->ip6_dst, dst, INET6_ADDRSTRLEN);
        printf("IPv6,Ip source: %s,Ip destination, %s\n", src, dst);
    }
    else
    {
        char src[INET6_ADDRSTRLEN];
        inet_ntop(AF_INET6, &head->ip6_src, src, INET6_ADDRSTRLEN);
        char dst[INET6_ADDRSTRLEN];
        inet_ntop(AF_INET6, &head->ip6_dst, dst, INET6_ADDRSTRLEN);
        printf("Ip source: %s,Ip destination, %s", src, dst);
    }

    int payload_size = ntohs(head->ip6_ctlun.ip6_un1.ip6_un1_plen) - 40;

    if (head->ip6_ctlun.ip6_un1.ip6_un1_nxt == TCP)
    {
        call_TCP(packet + sizeof(struct ip6_hdr), payload_size, verbosite);
    }
    else if (head->ip6_ctlun.ip6_un1.ip6_un1_nxt == UDP)
    {
        call_UDP(packet + sizeof(struct ip6_hdr), payload_size, verbosite);
    }
}