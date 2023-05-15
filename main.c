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

void exampleGenPacketWithScdHeader();
void exampleGenPacketWithoutScdHeader();
void exampleReadPacketWithScdHeader();
char readFile();
void other();

int main() {
    exampleGenPacketWithScdHeader();
    //exampleGenPacketWithoutScdHeader();
    exampleReadPacketWithScdHeader();
    return 0;
}

void exampleGenPacketWithScdHeader(){
    time_t seconds_sinceepoch = time(NULL);

    // Create packet
    char data[10] = {'1', '2', '3', '4', '5', '6', '7', '8', '9', 'a'};     // Some test userData
    CCSDS_secondary_header secondaryHeader = ccsdsSecondaryHeader((unsigned int) seconds_sinceepoch, MAJORVERSIONNUMBER,
                                                                    MINORVERSIONNUMBER, PATCHVERSIONNUMBER);
    CCSDS_data_field dataField = ccsdsDataField(&secondaryHeader, &data);

    unsigned short dataLength = sizeof(data) + SECONDARY_HEADER_LENGTH;      // Length of the userData field

    CCSDS_primary_header primaryHeader = ccsdsPrimaryHeader(0b001, PACKET_TYPE_TELEMETRY,
                                        SECONDAY_HEADER_FLAG_EXIST, 0b001, SEQUENCE_FLAG_UNSEGMENTED,
                                        0b0, dataLength - 1);

    CCSDS_packet packet = ccsdsPacketBuild(&primaryHeader, &dataField);

    // Get packet length
    unsigned short packet_length = PRIMARY_HEADER_LENGTH + dataLength;
    
    // Write packet into buffer using binary structure of CCSDS
    void* buff = writeInBuffer(&packet);

    FILE *fp;

    fp = fopen("first.bin", "w");
    int bytesWritten = fwrite(buff, packet_length, 1, fp);
    printf("Written successfully %d elements \n", bytesWritten*packet_length);
    fclose(fp);
}

void exampleGenPacketWithoutScdHeader(){
    // Create packet
    char data[10] = {'1', '2', '3', '4', '5', '6', '7', '8', '9', 'a'};     // Some test userData

    unsigned short dataLength = sizeof(data);      // Length of the userData field

    CCSDS_data_field dataField = ccsdsDataField(NULL, data);        // Create userData field without header

    CCSDS_primary_header primaryHeader = ccsdsPrimaryHeader(0b001, PACKET_TYPE_TELEMETRY,
                                                            SECONDAY_HEADER_FLAG_NOTEXIST, 0b011, SEQUENCE_FLAG_UNSEGMENTED,
                                                            0b0, dataLength - 1);

    CCSDS_packet packet = ccsdsPacketBuild(&primaryHeader, &dataField);

    // Get packet length
    unsigned short packet_length = PRIMARY_HEADER_LENGTH + dataLength;
    packet_length += (primaryHeader.sec_header_flag == SECONDAY_HEADER_FLAG_EXIST)? SECONDARY_HEADER_LENGTH : 0;

    // Write packet into buffer using binary structure of CCSDS
    void* buff = writeInBuffer(&packet);

    FILE *fp;

    fp = fopen("first.bin", "w");
    int bytesWritten = fwrite(buff, packet_length, 1, fp);
    printf("Written successfully %d elements \n", bytesWritten * packet_length);
    fclose(fp);
}

void exampleReadPacketWithScdHeader(){
    CCSDS_packet packet = ccsdsPacket_new();
    FILE *fp;

    if ((fp = fopen("first.bin","rb")) == NULL){
        printf("Error! opening file");

        // Program exits if the file pointer returns NULL.
        exit(1);
    }

    ccsdsReadPrimaryHeader(fp, &packet);// Read primary header (length fixed)
    
    if(packet.primary_header->sec_header_flag == SECONDAY_HEADER_FLAG_EXIST) {
        ccsdsReadSecondaryHeader(fp, &packet);
    }
    ccsdsReadFullUserData(fp, &packet);      // Malloc length and store userData, Not recommended, Data could be too large

    printPrimaryHeader(packet.primary_header);
    printDataField(&packet);
    

   fclose(fp);
}
