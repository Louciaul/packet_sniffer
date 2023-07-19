#ifndef ETHERNET_H
#define ETHERNET_H

#include <unistd.h>
#include <pcap.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <net/ethernet.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <netinet/tcp.h>
#include <netinet/ip6.h>
#include <ctype.h>
#include <net/if_arp.h>
#include <arpa/telnet.h>

#define TCP 6
#define UDP 17
#define FTP 21
#define TELNET 23
#define SMTP 25
#define DNS 53
#define BOOTPS 67
#define BOOTPC 68
#define HTTP 80
#define POP2 109
#define POP3 110
#define IMAP 143
#define HTTPS 443

struct arg_options
{
    char *online;
    char *offline;
    char *filter;
    int verbosite;
};

void call_ethernet(const u_char *packet, int verbosite);
void call_ip4(const u_char *packet, int verbosite);
void call_ip6(const u_char *packet, int verbosite);
void call_TCP(const u_char *packet, int size, int verbosite);
void call_UDP(const u_char *packet, int size, int verbosite);
void call_telnet(const u_char *packet, int size, int verbosite);
void call_http(const u_char *packet, int size, int verbosite);
void call_https(int verbosite);
void call_bootp(const u_char *packet, int size, int verbosite);
void call_ftp(const u_char *packet, int size, int verbosite);
void call_pop(const u_char *packet, int size, int verbosite);
void call_imap(const u_char *packet, int size, int verbosite);
void call_smtp(const u_char *packet, int size, int verbosite);
void call_arp(const u_char *packet, int verbosite);
void call_dns(int verbosite);

#endif