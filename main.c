#include <stdio.h>
#include <stdlib.h>
#include "CCSDS.h"
#include <string.h>

// Get endian
const int i = 1;
#define is_bigendian() ( (*(char*)&i) == 0 )

void printCCSDS(CCSDS_primary obj);

int main() {

    CCSDS_primary obj = {0b001,PACKET_TYPE_TELEMETRY, SECONDAY_HEADER_FLAG_NOTEXIST, 0b101, SEQUENCE_FLAG_UNSEGMENTED, 0b0, 0b10};
    void* my_pointer = &obj;
    /*
    CCSDS_primary my_packet = {0b0, PACKET_TYPE_TELEMETRY, SECONDAY_HEADER_FLAG_EXIST, 0b101, SEQUENCE_FLAG_UNSEGMENTED, 0b0, 0b10};
    CCSDS_primary src;
    unsigned char my_bin_packet_c[6] = {0b00000001, 0b0, 0b0, 0b0, 0b0, 0b1};

    printf("IS BIGENDIAN: %d \n", is_bigendian());

    if (!is_bigendian()) {
        memcpy(&src, my_bin_packet_c, sizeof(src));
    } else {
        unsigned char buff[6];
        snprintf(buff, sizeof(my_bin_packet_c),"%c%c%c%c%c%c", my_bin_packet_c[0], my_bin_packet_c[1], my_bin_packet_c[2], my_bin_packet_c[3], my_bin_packet_c[5], my_bin_packet_c[4]);
        memcpy(&src, buff, sizeof(src));
    }

    */

    printCCSDS(obj);

    printf("%d\n", sizeof(obj));
    //printf("%d\n", sizeof(my_bin_packet_c));

    return(0);
}


void printCCSDS(CCSDS_primary obj){
    printf("Version: %d\n", obj.version);
    if(obj.type == PACKET_TYPE_TELEMETRY) printf("TYPE: TELEMETRY\n");
    else printf("TYPE: TELECOMMAND\n");
    printf("Secondary Header Flag: %d\n", obj.sec_header_flag);
    printf("Application proccess identifier: %d\n", obj.proc_id);
    printf("Sequence Flags: %d\n", obj.seq_flags);
    printf("Packet Sequence count: %d\n", obj.seq_cnt);
    printf("Packet Data length: %d\n", obj.length);
    printf("BINARY FORMAT: \n")
}
