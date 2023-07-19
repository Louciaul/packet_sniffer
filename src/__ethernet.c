#include "../headers/protocoles.h"

void call_ethernet(const u_char *packet, int verbosite)
{

    struct ether_header *eth_head = (struct ether_header *)packet;

    printf("\n---------------------------------------------------------------------------------------\n\n");

    if (verbosite == 3)
    {
        // Affichage du contenu de la trame

        printf("-----------Ethernet-----------\n\n");

        printf("Entête Ethernet:\nAdresse MAC Destination: %02x:%02x:%02x:%02x:%02x:%02x\n",
               eth_head->ether_dhost[0],
               eth_head->ether_dhost[1],
               eth_head->ether_dhost[2],
               eth_head->ether_dhost[3],
               eth_head->ether_dhost[4],
               eth_head->ether_dhost[5]);

        printf("Adresse MAC Source: %02x:%02x:%02x:%02x:%02x:%02x\n",
               eth_head->ether_shost[0],
               eth_head->ether_shost[1],
               eth_head->ether_shost[2],
               eth_head->ether_shost[3],
               eth_head->ether_shost[4],
               eth_head->ether_shost[5]);
    }
    else if (verbosite == 2)
    {
        printf("Ethernet: Adresse MAC Destination: %02x:%02x:%02x:%02x:%02x:%02x,Adresse MAC Source: %02x:%02x:%02x:%02x:%02x:%02x\n",
               eth_head->ether_dhost[0],
               eth_head->ether_dhost[1],
               eth_head->ether_dhost[2],
               eth_head->ether_dhost[3],
               eth_head->ether_dhost[4],
               eth_head->ether_dhost[5], eth_head->ether_shost[0],
               eth_head->ether_shost[1],
               eth_head->ether_shost[2],
               eth_head->ether_shost[3],
               eth_head->ether_shost[4],
               eth_head->ether_shost[5]);
    }

    // Protocole suivant

    if (ntohs(eth_head->ether_type) == ETHERTYPE_IP)
    {
        call_ip4(packet + sizeof(struct ether_header), verbosite);
    }
    else if (ntohs(eth_head->ether_type) == ETHERTYPE_IPV6)
    {
        call_ip6(packet + sizeof(struct ether_header), verbosite);
    }
    else if (ntohs(eth_head->ether_type) == ETHERTYPE_ARP)
    {
        call_arp(packet + sizeof(struct ether_header), verbosite);
    }
    else
    {
        printf("Ethernet: next protocol Unknown\n");
    }
}