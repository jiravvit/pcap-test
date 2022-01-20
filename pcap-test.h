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

void print_packet(const u_char* packet, uint32_t total_len)
{
    printf("===================================================\n");
    printf("Header\n");
    printf("[Ethernet] src mac: , dst mac: \n");
    printf("[      IP] src ip: , dst ip: \n");
    printf("[     TCP] src port: , dst port: \n");
    printf("Payload\n");
    printf("[    DATA] src port: , dst port: \n");
}
