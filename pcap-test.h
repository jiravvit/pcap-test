#pragma once
#include <pcap.h>
#include <stdbool.h>
#include <stdio.h>
#include <libnet.h> // libnet

void print_eth_h(const u_char* packet);
void print_ip_h(const u_char* packet);
void print_tcp_h(const u_char* packet);
void print_data(const u_char* packet, uint32_t tcp_offset, uint32_t total_len);
void print_packet(const u_char* packet, uint32_t total_len);
