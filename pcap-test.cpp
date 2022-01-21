#include "pcap-test.h"

void print_eth_h(const u_char* packet)
{
     struct libnet_ethernet_hdr *eth = (struct libnet_ethernet_hdr *) packet;
     uint16_t ether_type = ntohs(eth->ether_type);
     if (ether_type != 0x0800) // Is it Ethernet type is IPv4?
     {
         printf("[!] ERROR : no IPv4\n");
         return ;
     }

     printf("[+] Ethernet \n    src mac >> %02x:%02x:%02x:%02x:%02x:%02x\n    dst mac >> %02x:%02x:%02x:%02x:%02x:%02x\n",
            eth->ether_shost[0],eth->ether_shost[1],eth->ether_shost[2],eth->ether_shost[3],eth->ether_shost[4], eth->ether_shost[5],
             eth->ether_dhost[0],eth->ether_dhost[1],eth->ether_dhost[2],eth->ether_dhost[3],eth->ether_dhost[4],eth->ether_dhost[5] );
}

void print_ip_h(const u_char* packet)
{
    struct libnet_ipv4_hdr *iph = (struct libnet_ipv4_hdr *) packet;
    uint8_t ip_protocal = iph->ip_p;
    if(ip_protocal != 6) // Is it Protocol type is TCP?
    {
        printf("[!] ERROR : no TCP\n");
        return ;
    }

    // Why do not work 'iph->ip_dst'?
    printf("[+] IP \n    src ip >> %s\n    dst ip >> %d.%d.%d.%d\n",
           inet_ntoa(iph->ip_src), packet[16], packet[17], packet[18], packet[19]);
}

void print_tcp_h(const u_char* packet)
{
    struct libnet_tcp_hdr *tcph = (struct libnet_tcp_hdr *) packet;

    printf("[+] TCP \n    src port >> %d\n    dst port >> %d\n",
           ntohs(tcph->th_sport), ntohs(tcph->th_dport));
}

void print_data(const u_char* packet, uint32_t tcp_offset, uint32_t total_len)
{
    struct libnet_tcp_hdr *tcph = (struct libnet_tcp_hdr *)(packet);
    uint32_t data_offset = tcp_offset + (tcph->th_off)*4;
    //printf("%d %d\n", tcp_offset, (tcph->th_off)*4);

    if (total_len - data_offset == 0)
    {
       printf("[!] ERROR : no Data\n\n");
       return ;
    }

    u_char* data = (u_char*)(packet + data_offset - 34);    // why sub 34..?

    printf("[+] DATA (8 bytes) \n    >> ");
    for (int i = 0; i < 8; i++) printf("%02X ", data[i]);
    printf("\n\n");

}

void print_packet(const u_char* packet, uint32_t total_len)
{
    uint32_t ip_offset = sizeof(struct libnet_ethernet_hdr);
    struct libnet_ipv4_hdr *iph = (struct libnet_ipv4_hdr *)(packet + ip_offset);
    uint32_t tcp_offset = ip_offset + (iph->ip_hl)*4;

    printf("===================================================\n\n");
    printf("[*] Header\n");

    print_eth_h(packet);
    print_ip_h(packet + ip_offset);
    print_tcp_h(packet + tcp_offset);

    printf("[*] Payload\n");

    print_data(packet + tcp_offset, tcp_offset, total_len);
}
