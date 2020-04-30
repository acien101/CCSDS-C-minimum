#include <stdio.h>
#include <stdlib.h>
#include "CCSDS.h"
#include <string.h>

// Get endian
const int i = 1;
#define is_bigendian() ( (*(char*)&i) == 0 )

void printCCSDS(CCSDS_primary obj);
void genRead();
char readFile();


int main() {
    CCSDS_packet packet;
    int num;
    FILE *fp;

    if ((fp = fopen("../sample.bin","rb")) == NULL){
        printf("Error! opening file");

        // Program exits if the file pointer returns NULL.
        exit(1);
    }

    unsigned char bu = 'a';

    fread(&(packet.primary_header), sizeof(packet.primary_header), 1, fp);
    printCCSDS(packet.primary_header);
    putData(fp, &packet);
    printData(&packet);

    fclose(fp);
    return 0;
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
    //printf("BINARY FORMAT: \n", )
}

char readFile(){
    CCSDS_primary my_packet;
    int num;
    FILE *fp;

    if ((fp = fopen("ahora.bin","rb")) == NULL){
        printf("Error! opening file");

        // Program exits if the file pointer returns NULL.
        exit(1);
    }

    fread(&my_packet, sizeof(my_packet), 1, fp);
    printCCSDS(my_packet);

    fclose(fp);
    return 0;
}

void genFile(){
    CCSDS_primary obj = {0b001,PACKET_TYPE_TELEMETRY, SECONDAY_HEADER_FLAG_NOTEXIST, 0b101, SEQUENCE_FLAG_UNSEGMENTED, 0b0, 0b11};
    char data[3] = {'D', 'P', 'M'};
    void* my_pointer = &obj;

    printCCSDS(obj);

    printf("%d\n", sizeof(obj));
    printf("%d\n", sizeof(my_pointer));
    //printf("%d\n", sizeof(my_bin_packet_c));

    FILE *fp;

    fp = fopen("ahora.bin", "w");
    fwrite(&obj, sizeof(CCSDS_primary), 1, fp);
    fclose(fp);
}
