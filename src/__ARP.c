#include "../headers/protocoles.h"

void call_arp(const u_char *packet, int verbosite)
// Ne traite pas le cas où des adresses MAC seraient de longueurs différentes
{
    (void)packet;
    (void)verbosite;
    struct arphdr *arp_hd = (struct arphdr *)packet;

    char *code;

    // Analyse du code d'opération

    switch (ntohs(arp_hd->ar_op))
    {
    case 1:
        code = "ARP request";
        break;
    case 2:
        code = "ARP reply";
        break;
    case 3:
        code = "RARP request";
        break;
    case 4:
        code = "RARP reply";
        break;
    default:
        code = "Unknown";
    }

    const uint8_t *payload = packet + sizeof(struct arphdr);

    if (verbosite == 3)
    {
        // Affichage du contenu du paquet selon les valeurs obtenus (ipv4,ipv6, ARP, RARP)

        if (ntohs(arp_hd->ar_op) == ARPOP_RREQUEST || ntohs(arp_hd->ar_op) == ARPOP_RREPLY)
        {
            printf("\n\n-----------RARP-----------\n\n");
        }
        else
        {
            printf("\n\n-----------ARP-----------\n\n");
        }

        printf("Format of hardware address: 0x%04x\n", ntohs(arp_hd->ar_hrd));
        printf("Format of protocol address: 0x%04x\n", ntohs(arp_hd->ar_pro));
        printf("Length of hardware address: %u\n", arp_hd->ar_hln);
        printf("Length of protocol address: %u\n", arp_hd->ar_pln);

        printf("Op code: %s\n", code);

        printf("Sender MAC address: %02x:%02x:%02x:%02x:%02x:%02x\n",
               payload[0], payload[1], payload[2], payload[3], payload[4], payload[5]);

        payload = payload + arp_hd->ar_hln;

        if (arp_hd->ar_pln == 4)
        {
            printf("Sender IP Address: %s\n", inet_ntoa(*(struct in_addr *)payload));
        }
        else if (arp_hd->ar_pln == 16)
        {
            char str[INET6_ADDRSTRLEN];
            inet_ntop(AF_INET6, (struct in6_addr *)payload, str, INET6_ADDRSTRLEN);
            printf("Sender IP Address: %s\n", str);
        }

        payload = payload + arp_hd->ar_pln;

        printf("Target MAC address: %02x:%02x:%02x:%02x:%02x:%02x\n",
               payload[0], payload[1], payload[2], payload[3], payload[4], payload[5]);

        payload = payload + arp_hd->ar_hln;

        if (arp_hd->ar_pln == 4)
        {
            printf("Target IP Address: %s\n", inet_ntoa(*(struct in_addr *)payload));
        }
        else if (arp_hd->ar_pln == 16)
        {
            char str[INET6_ADDRSTRLEN];
            inet_ntop(AF_INET6, (struct in6_addr *)payload, str, INET6_ADDRSTRLEN);
            printf("Target IP Address: %s\n", str);
        }
    }

    else
    {
        // Pour une verbosité plus faible on va chercher à afficher la demande ARP

        if (ntohs(arp_hd->ar_op) == ARPOP_RREQUEST || ntohs(arp_hd->ar_op) == ARPOP_RREPLY)
        {
            printf("RARP: ");
        }
        else
        {
            printf("ARP: ");
        }

        char *mac_src = malloc(100 * sizeof(char));
        char *ip_src = malloc(30 * sizeof(char));
        char *mac_dst = malloc(100 * sizeof(char));
        char *ip_dst = malloc(30 * sizeof(char));

        // Copie des Adresses

        snprintf(mac_src, 100, "%02x:%02x:%02x:%02x:%02x:%02x",
                 payload[0], payload[1], payload[2], payload[3], payload[4], payload[5]);

        payload = payload + arp_hd->ar_hln;

        if (arp_hd->ar_pln == 4)
        {
            strcpy(ip_src, inet_ntoa(*(struct in_addr *)payload));
        }
        else if (arp_hd->ar_pln == 16)
        {
            inet_ntop(AF_INET6, (struct in6_addr *)payload, ip_src, INET6_ADDRSTRLEN);
        }

        payload = payload + arp_hd->ar_pln;

        snprintf(mac_dst, 100, "%02x:%02x:%02x:%02x:%02x:%02x",
                 payload[0], payload[1], payload[2], payload[3], payload[4], payload[5]);

        payload = payload + arp_hd->ar_hln;

        if (arp_hd->ar_pln == 4)
        {
            strcpy(ip_dst, inet_ntoa(*(struct in_addr *)payload));
        }
        else if (arp_hd->ar_pln == 16)
        {
            inet_ntop(AF_INET6, (struct in6_addr *)payload, ip_dst, INET6_ADDRSTRLEN);
        }

        // Affichage de la requête à l'aide des adresses

        if (ntohs(arp_hd->ar_op) == ARPOP_REQUEST)
        {
            printf("Who has %s ? Tell %s\n", ip_dst, ip_src);
        }

        else if (ntohs(arp_hd->ar_op) == ARPOP_RREQUEST)
        {
            printf("Who is %s ? Tell %s\n", mac_dst, mac_src);
        }
        else if (ntohs(arp_hd->ar_op) == ARPOP_REPLY)
        {
            printf("%s is at %s\n", ip_src, mac_dst);
        }
        else if (ntohs(arp_hd->ar_op) == ARPOP_RREPLY)
        {
            printf("%s it at %s\n", mac_src, ip_src);
        }
        else
        {
            printf("Unknown OpCode\n");
        }

        free(mac_dst);
        free(mac_src);
        free(ip_src);
        free(ip_dst);
    }
}