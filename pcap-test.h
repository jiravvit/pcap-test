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
     printf("[+] (Ethernet) src mac: %02x:%02x:%02x:%02x:%02x:%02x, dst: %02x:%02x:%02x:%02x:%02x:%02x\n",
            eth->ether_shost[0],eth->ether_shost[1],eth->ether_shost[2],eth->ether_shost[3],eth->ether_shost[4], eth->ether_shost[5],
             eth->ether_dhost[0],eth->ether_dhost[1],eth->ether_dhost[2],eth->ether_dhost[3],eth->ether_dhost[4],eth->ether_dhost[5] );
}

void print_ip_h(const u_char* packet)
{

}

void print_tcp_h(const u_char* packet)
{

}

void print_data(const u_char* packet)
{

}

void print_packet(const u_char* packet)
{
    printf("===================================================\n");
    printf("[*] Header\n");
    print_eth_h(packet);
    printf("[+] (IP) src ip: , dst ip: \n");
    printf("[+] (TCP) src port: , dst port: \n");
    printf("[*] Payload\n");
    printf("[+] (DATA) src port: , dst port: \n\n");
}

