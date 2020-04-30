//
// Created by acien101 on 29/4/20.
//
#include <stdio.h>
#include <stdlib.h>
#include "CCSDS.h"

void printData(CCSDS_packet *packet){
    long i = packet->primary_header.length + 1;
    for(;i > 0; i--){
        printf("GET: %c\n", *(unsigned char*)(packet->data++));
    }
}

void putData(FILE *fp, CCSDS_packet *packet){
    packet->data = (unsigned char*)malloc(packet->primary_header.length + 1);
    fread(packet->data, packet->primary_header.length +1, 1, fp);
}

