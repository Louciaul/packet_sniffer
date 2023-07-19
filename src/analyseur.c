#include "../headers/protocoles.h"

void usage()
{
    printf("Usage : ./nomduprogramme options...\n");
    printf("Options disponibles:\n"
           "-i <interface>           pour de l'analyse live\n"
           "-o <fichier>             pour de l'analyse offline\n"
           "-f <filtre>              filtre BPF (optionnel)(non implémenté encore)\n"
           "-v <1,2,3>               verbosité (1= très concis; "
           "2=synthétique; 3=complet) par défaut est fixé à 3\n");
}

void initOptions(struct arg_options *options)
{
    options->filter = NULL;
    options->online = NULL;
    options->offline = NULL;
    options->verbosite = 3;
}

void callback(u_char *args, const struct pcap_pkthdr *header,
              const u_char *packet)
{

    (void)header;

    call_ethernet(packet, (int)args[1]);
}

int main(int argc, char *argv[])
{

    if (argc == 1)
    {
        usage();
        return -1;
    }

    struct arg_options *my_options =
        malloc(sizeof(struct arg_options));

    initOptions(my_options);

    int c;
    while ((c = getopt(argc, argv, "i:o:f:v:h")) != -1)
    {
        switch (c)
        {
        case 'i':
            my_options->online = optarg;
            break;
        case 'o':
            my_options->offline = optarg;
            break;
        case 'f':
            my_options->filter = optarg;
            break;
        case 'v':
            my_options->verbosite = atoi(optarg);
            break;
        case 'h':
            usage();
            return 0;
        }
    }

    (void)my_options;

    char sterr[256];

    (void)argc;

    (void)argv;

    pcap_t *stream = malloc(sizeof(pcap_t *));

    u_char *verb = malloc(sizeof(u_char *));
    verb[1] = (u_char)my_options->verbosite;

    if (my_options->filter != NULL)
    {
        struct bpf_program *filter = malloc(sizeof(struct bpf_program));

        if (pcap_compile(stream, filter, my_options->filter, 0, PCAP_NETMASK_UNKNOWN) == -1)
        {
            printf("error with filter compiling %s\n", sterr);
            return -1;
        }
        if (pcap_setfilter(stream, filter) == PCAP_ERROR)
        {
            printf("error to setfilter: %s\n", sterr);
            return -1;
        }
    }

    if (my_options->online != NULL)
    {
        stream = pcap_open_live(my_options->online, 8192, 1, 1000,
                                sterr); // sudo !!!

        if (stream == NULL)
        {
            printf("error in opening %s\n", sterr);
        }

        pcap_loop(stream, -1, callback, verb);
    }

    if (my_options->offline != NULL)
    {
        stream = pcap_open_offline(my_options->offline, sterr);

        if (stream == NULL)
        {
            printf("error in opening %s\n", sterr);
        }

        pcap_loop(stream, -1, callback, verb);
        pcap_close(stream);
    }

    free(my_options);

    return 0;
}
