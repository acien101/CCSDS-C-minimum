#include <stdio.h>
#include <stdlib.h>
#include "CCSDS.h"
#include <string.h>
#include <time.h>

// Some definitions
#define MAJORVERSIONNUMBER 0b1
#define MINORVERSIONNUMBER 0b10
#define PATCHVERSIONNUMBER 0b11


// Get endian
const int i = 1;
#define is_bigendian() ( (*(char*)&i) == 0 )

void printCCSDS(CCSDS_primary_header obj);
void exampleGenPacketWithScdHeader();
void exampleReadPacketWithScdHeader();
char readFile();


int main() {
    exampleGenPacketWithScdHeader();
    exampleReadPacketWithScdHeader();
    return 0;
}


void printCCSDS(CCSDS_primary_header obj){
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
    CCSDS_primary_header my_packet;
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

void exampleGenPacketWithScdHeader(){
    time_t seconds_sinceepoch = time(NULL);

    // Create packet
    char data[10] = {'P', 'R', 'U', 'E', 'B', 'A', '1', '2', '3', '4'};     // Some test data
    CCSDS_secondary_header secondaryHeader = ccsdsSecondaryHeader((unsigned int) seconds_sinceepoch, MAJORVERSIONNUMBER,
                                                                    MINORVERSIONNUMBER, PATCHVERSIONNUMBER);
    CCSDS_data_field dataField = ccsdsDataField(secondaryHeader, &data);

    unsigned short dataLength = sizeof(data) + SECONDARY_HEADER_LENTH;      // Length of the data field

    CCSDS_primary_header primaryHeader = ccsdsPrimaryHeader(0b001, PACKET_TYPE_TELEMETRY,
                                        SECONDAY_HEADER_FLAG_EXIST, 0b001, SEQUENCE_FLAG_UNSEGMENTED,
                                        0b0, dataLength - 1);

    CCSDS_packet packet = ccsdsPacket(primaryHeader, dataField);
    void* str = &packet;

    unsigned short packetLength = PRIMARY_HEADER_LENGTH + dataLength;

    FILE *fp;

    fp = fopen("first.bin", "w");
    fwrite(&primaryHeader, PRIMARY_HEADER_LENGTH, 1, fp);   //Write primary header
    fwrite(&secondaryHeader, SECONDARY_HEADER_LENTH, 1, fp);    //Write secondary header
    fwrite(data, sizeof(data), 1, fp);      // Write data
    fclose(fp);
}

void exampleReadPacketWithScdHeader(){
    CCSDS_packet packet;
    FILE *fp;

    if ((fp = fopen("first.bin","rb")) == NULL){
        printf("Error! opening file");

        // Program exits if the file pointer returns NULL.
        exit(1);
    }

    fread(&(packet.primary_header), PRIMARY_HEADER_LENGTH, 1, fp);              // Read primary header (length fixed)
    if(packet.primary_header.sec_header_flag == SECONDAY_HEADER_FLAG_EXIST)
        fread(&(packet.dataField.secondaryHeader), SECONDARY_HEADER_LENTH, 1, fp);  // Read secondary header (length fixed)
    ccsdsGetFullData(fp, &packet);      // Malloc length and store data, Not recommended, Data could be too large

    printCCSDS(packet.primary_header);
    if(packet.primary_header.sec_header_flag == SECONDAY_HEADER_FLAG_EXIST)
        printSecondaryHeader(packet.dataField.secondaryHeader);
    printData(&packet);
}
