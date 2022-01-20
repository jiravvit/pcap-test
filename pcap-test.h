#pragma once
#include <pcap.h>
#include <stdbool.h>
#include <stdio.h>
#include <libnet.h> // libnet

void usage() {
    printf("syntax: pcap-test <interface>\n");
    printf("sample: pcap-test wlan0\n");
}

typedef struct {
    char* dev_;
} Param;

Param param  = {
    .dev_ = NULL
};

bool parse(Param* param, int argc, char* argv[]) {
    if (argc != 2) {
        usage();
        return false;
    }
    param->dev_ = argv[1];
    return true;
}

void print_eth_h(const u_char* packet)
{
     struct libnet_ethernet_hdr *eth = (struct libnet_ethernet_hdr *) packet;
     uint16_t ether_type = ntohs(eth->ether_type);
     if (ether_type != 0x0800) // Is it Ethernet type is IPv4?
     {
         printf("[!] ERROR : no IPv4\n");
         return ;
     }

     // print
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

void print_data(const u_char* packet, uint32_t total_len)
{
   if (total_len-14-20-32 == 0)
   {
       printf("[!] ERROR : no Data\n");
       return ;
   }

    //packet + ethernet_header_len + ip_header_len + tcp_header_len
    u_char* data = (u_char*)(packet + 14 + 20 + 32);

    printf("[+] DATA (8 bytes) \n    >> ");
    for (int i = 0; i < 8; i++) printf("%02X ", data[i]);
    printf("\n\n");

}

void print_packet(const u_char* packet, uint32_t total_len)
{
    printf("===================================================\n");
    printf("[*] Header\n");

    print_eth_h(packet);
    print_ip_h(packet + 14);       // Ethernet Static header size: 14 bytes
    print_tcp_h(packet + 14 + 20); // IPv4 Static header size: 20 bytes

    printf("[*] Payload\n");

    print_data(packet, total_len);
}

