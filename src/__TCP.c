#include "../headers/protocoles.h"

void call_TCP(const u_char *packet, int size, int verbosite)
{
    struct tcphdr *header = (struct tcphdr *)packet;

    if (verbosite == 3)
    {

        printf("\n\n-----------TCP-----------\n\n");

        printf("\nEntête TCP:\n");
        printf("Source Port: %i\n", ntohs(header->th_sport));
        printf("Destination Port: %i\n", ntohs(header->th_dport));
        printf("Sequence number: %u\n", ntohl(header->th_seq));
        printf("Acknowledgement number: %u\n", ntohl(header->ack_seq));
        printf("Data offset: %d\n", header->th_off);

        printf("Flags: ");

        if (header->th_flags & 1)
        {
            printf("FIN ");
        }

        if (header->th_flags & 1 << 1)
        {
            printf("SYN ");
        }

        if (header->th_flags & 1 << 2)
        {
            printf("RST ");
        }

        if (header->th_flags & 1 << 3)
        {
            printf("PUSH ");
        }

        if (header->th_flags & 1 << 4)
        {
            printf("ACK ");
        }

        if (header->th_flags & 1 << 5)
        {
            printf("URG ");
        }

        printf("\n");

        printf("Window: %d\n", ntohs(header->th_win));
        printf("Checksum: 0X%04x\n", ntohs(header->th_sum));
        printf("Urgent pointer: %d\n", ntohs(header->th_urp));
    }

    else if (verbosite == 2)
    {
        printf("TCP: Source Port: %i, Destination Port: %i\n", ntohs(header->th_sport), ntohs(header->th_dport));
    }

    int payload_size = size - header->th_off * 4;

    // on regarde le port source et le port de destination

    uint16_t source = ntohs(header->th_sport);
    uint16_t dest = ntohs(header->th_dport);

    const u_char *next_packet = packet + header->th_off * 4;

    if (source == TELNET || dest == TELNET)
    {
        call_telnet(next_packet, payload_size, verbosite);
    }
    else if (source == HTTP || dest == HTTP)
    {
        call_http(next_packet, payload_size, verbosite);
    }
    else if (source == FTP || dest == FTP)
    {
        call_ftp(next_packet, payload_size, verbosite);
    }
    else if (source == POP2 || dest == POP2)
    {
        call_pop(next_packet, payload_size, verbosite);
    }
    else if (source == POP3 || dest == POP3)
    {
        call_pop(next_packet, payload_size, verbosite);
    }
    else if (source == IMAP || dest == IMAP)
    {
        call_imap(next_packet, payload_size, verbosite);
    }
    else if (source == HTTPS || dest == HTTPS)
    {
        call_https(verbosite);
    }
    else if (source == SMTP || dest == SMTP)
    {
        call_smtp(next_packet, payload_size, verbosite);
    }
    else
    {
        printf("\nProtocol: not implemented\n");
    }
}