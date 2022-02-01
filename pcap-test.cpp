#include "pcap-test.h"

void print_eth_h(const struct libnet_ethernet_hdr* eth)
{
     // dupul
     printf("[+] Ethernet \n    src mac >> %02x:%02x:%02x:%02x:%02x:%02x\n    dst mac >> %02x:%02x:%02x:%02x:%02x:%02x\n",
            eth->ether_shost[0],eth->ether_shost[1],eth->ether_shost[2],eth->ether_shost[3],eth->ether_shost[4], eth->ether_shost[5],
             eth->ether_dhost[0],eth->ether_dhost[1],eth->ether_dhost[2],eth->ether_dhost[3],eth->ether_dhost[4],eth->ether_dhost[5] );
}

void print_ip_h(const struct libnet_ipv4_hdr* iph)
{
    printf("[+] IP \n    src ip >> %s\n", inet_ntoa(iph->ip_src));
    printf("    dst ip >> %s\n",inet_ntoa(iph->ip_dst));
}

void print_tcp_h(const struct libnet_tcp_hdr* tcph)
{
    printf("[+] TCP \n    src port >> %d\n    dst port >> %d\n",
           ntohs(tcph->th_sport), ntohs(tcph->th_dport));
}

void print_data(u_char *data_offset, uint32_t header_len, uint32_t total_len)
{
    u_char* data = data_offset;

    if (total_len <= header_len)
    {
       printf("[!] ERROR : no Data\n\n");
       return ;
    }

    int cnt = total_len - header_len;
    if (cnt > 8 ) cnt = 8;

    printf("[+] DATA (8 bytes) \n    >> ");
    for (int i = 0; i < cnt; i++) printf("%02X ", data[i]);
    printf("\n\n");

}

void print_packet(const u_char* packet, uint32_t total_len)
{
    struct libnet_ethernet_hdr *eth = (struct libnet_ethernet_hdr *) packet;
    uint16_t ether_type = ntohs(eth->ether_type);

    if (ether_type != 0x0800) // Is it Ethernet type is IPv4?
    {
        printf("[!] ERROR : no IPv4\n");
        return ;
    }

    uint32_t ip_offset = sizeof(struct libnet_ethernet_hdr);
    struct libnet_ipv4_hdr *iph = (struct libnet_ipv4_hdr *)(packet + ip_offset);

    uint8_t ip_protocal = iph->ip_p;
    if(ip_protocal != 6) // Is it Protocol type is TCP?
    {
        printf("[!] ERROR : no TCP\n");
        return ;
    }

    uint32_t tcp_offset = ip_offset + (iph->ip_hl)*4;
    struct libnet_tcp_hdr *tcph = (struct libnet_tcp_hdr *)(packet + tcp_offset);

    printf("===================================================\n\n");
    printf("[*] Header\n");

    print_eth_h(eth);
    print_ip_h(iph);
    print_tcp_h(tcph);

    printf("[*] Payload\n");

    uint32_t header_len = sizeof(struct libnet_ethernet_hdr) + (iph->ip_hl)*4 + (tcph->th_off)*4;
    u_char *data_offset = (u_char *)(packet + header_len);
    print_data(data_offset, header_len, total_len);
}
