//
// Created by acien101 on 29/4/20.
//
#include <stdio.h>
#include <stdlib.h>
#include <bits/types/struct_tm.h>
#include <time.h>
#include "CCSDS.h"

void printData(CCSDS_packet *packet){
    long i;
    if(packet->primary_header.sec_header_flag == SECONDAY_HEADER_FLAG_EXIST)
        i = packet->primary_header.length + 1 - SECONDARY_HEADER_LENTH;
    else
        i = packet->primary_header.length + 1;

    for(;i > 0; i--){
        printf("GET: %c\n", *(unsigned char*)(packet->dataField.data++));
    }
}

void ccsdsGetFullData(FILE *fp, CCSDS_packet *packet){
    if(packet->primary_header.sec_header_flag == SECONDAY_HEADER_FLAG_EXIST) {
        packet->dataField.data = (unsigned char *) malloc(packet->primary_header.length + 1 - SECONDARY_HEADER_LENTH);
        fread(packet->dataField.data, packet->primary_header.length +1 - SECONDARY_HEADER_LENTH, 1, fp);
    } else {
        packet->dataField.data = (unsigned char *) malloc(packet->primary_header.length + 1);
        fread(packet->dataField.data, packet->primary_header.length +1, 1, fp);
    }
}

CCSDS_primary_header ccsdsPrimaryHeader(unsigned short version, unsigned short type, unsigned short sec_header_flag,
                                        unsigned short proc_id, unsigned short seq_flags, unsigned short seq_cnt,
                                        unsigned short length) {      //Build a CCSDS_primary_header packet
    CCSDS_primary_header res = {version, type, sec_header_flag, proc_id, seq_flags, seq_cnt, length};
    return res;
}
CCSDS_secondary_header ccsdsSecondaryHeader(unsigned int epoch, unsigned char majorVersionNumber,
                                            unsigned char minorVersionNumber, unsigned char patchVersionNumber){
    CCSDS_secondary_header res = {epoch, majorVersionNumber, minorVersionNumber, patchVersionNumber};
    return res;
}

CCSDS_packet ccsdsPacket(CCSDS_primary_header primaryHeader, CCSDS_data_field dataField) {   // Build CCSDS packet
    CCSDS_packet res = {primaryHeader, dataField};
    return res;
}

CCSDS_data_field ccsdsDataField(CCSDS_secondary_header secondaryHeader, void *data) {    // Build data field given second header and pointer
    CCSDS_data_field res = {secondaryHeader, data};
    return res;
}

void printSecondaryHeader(CCSDS_secondary_header secondaryHeader) {        // Giver a secondary header print data
    time_t rawtime = (time_t) secondaryHeader.epoch;
    struct tm  ts;
    char       buf[80];

    // Format time, "ddd yyyy-mm-dd hh:mm:ss zzz"
    ts = *localtime(&rawtime);
    strftime(buf, sizeof(buf), "%a %Y-%m-%d %H:%M:%S %Z", &ts);
    printf("%s\n", buf);

    printf("MAJOR VERSION NUMBER: %d\n", secondaryHeader.majorVersionNumber);
    printf("MINOR VERSION NUMBER: %d\n", secondaryHeader.minorVersionNumber);
    printf("PATCH VERSION NUMBER: %d\n", secondaryHeader.patchVersionNumber);
}