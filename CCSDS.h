#ifndef CCSDS_h
#define CCSDS_h

#include "string.h"

/*

enum PACKET_TYPE{
    TELEMETRY = 0,
    TELECOMMAND = 1
};

*/

#define PACKET_TYPE_TELEMETRY 0b0               // 4.1.2.3.2.3
#define PACKET_TYPE_TELECOMMAND 0b1
#define SECONDAY_HEADER_FLAG_EXIST 0b1          // 4.1.2.3.3.2
#define SECONDAY_HEADER_FLAG_NOTEXIST 0b0
#define SEQUENCE_FLAG_CONT 0b00                 // 4.1.2.4.2.2 a)
#define SEQUENCE_FLAG_FIRST 0b01                // 4.1.2.4.2.2 b)
#define SEQUENCE_FLAG_LAST 0b10                 // 4.1.2.4.2.2 c)
#define SEQUENCE_FLAG_UNSEGMENTED 0b11          // 4.1.2.4.2.2 d)
#define SECONDARY_HEADER_LENGTH 7                // Number of bytes of secondary header
#define PRIMARY_HEADER_LENGTH 6                 // Number of bytes of primary header

#define CCSDS_VERSION_MASK 0b111
#define CCSDS_VERSION_OFFSET 45         // Bits offst

#define CCSDS_TYPE_MASK 0b1
#define CCSDS_TYPE_OFFSET 44

#define CCSDS_SEC_HEADER_FLAG_MASK 0b1
#define CCSDS_SEC_HEADER_FLAG_OFFSET 43

#define CCSDS_PROC_ID_MASK 0x7ff
#define CCSDS_PROC_ID_OFFSET 32

#define CCSDS_SEQ_FLAGS_MASK 0b11
#define CCSDS_SEQ_FLAGS_OFFSET 30

#define CCSDS_SEQ_CNT_MASK 0x3fff
#define CCSDS_SEQ_CNT_OFFSET 16

#define CCSDS_LENGTH_MASK 0xFFFF
#define CCSDS_LENGTH_OFFSET 0

typedef struct {
    unsigned short version : 3;
    unsigned short type : 1;
    unsigned short sec_header_flag : 1;
    unsigned short proc_id : 11;
    unsigned short seq_flags : 2;
    unsigned short seq_cnt : 14;    //Seq count or packet name
    unsigned short length;
} CCSDS_primary_header;

typedef struct {
    unsigned int epoch;  // Time code field

    // Ancillary Data Field
    // Version number in major.minor.patch format
    // as defined in the Semantic Versioning 2.0.0 standard.
    unsigned char majorVersionNumber;
    unsigned char minorVersionNumber;
    unsigned char patchVersionNumber;

} CCSDS_secondary_header;

typedef struct {
    CCSDS_secondary_header* secondaryHeader;
    void* userData;
} CCSDS_data_field;

typedef struct{
    CCSDS_primary_header* primary_header;
    CCSDS_data_field* dataField;
    uint16_t length;
} CCSDS_packet;

typedef struct{         // Structure used when creating the buffer and send it over different channels
    uint8_t* packet;
    uint16_t length;
} CCSDS_buffer;

/**
 * Create the skeleton of a CCSDS_packet. Malloc primary_header and dataField and return the pointer
 * @return CCSDS_packet
 */
CCSDS_packet ccsdsPacket_new(void);

/**
 * Create a CCSDS_primary_header
 * @param version - 4.1.2.2 Packet Version Number (3 bits) Max value 7
 * @param type - 4.1.2.3.2 Packet Type (1 bit) 0 -> PACKET_TYPE_TELEMETRY, 1 -> PACKET_TYPE_TELECOMMAND
 * @param sec_header_flag - 4.1.2.3.3 Secondary Header Flag (1 bit) 0 -> SECONDAY_HEADER_FLAG_NOTEXIST,
 *                                                                  1 -> SECONDAY_HEADER_FLAG_EXIST
 * @param proc_id - 4.1.2.3.4 Application Process Identifier (11 bits) Max value 2047
 * @param seq_flags - 4.1.2.4.2 Sequence Flags (2 bits) 00 -> SEQUENCE_FLAG_CONT
 *                                                      01 -> SEQUENCE_FLAG_FIRST
 *                                                      10 -> SEQUENCE_FLAG_LAST
 *                                                      11 -> SEQUENCE_FLAG_LAST
 * @param seq_cnt - 4.1.2.4.3 Packet Sequence Count or Packet Name (14 bits) Max value 16383
 * @param length - 4.1.2.5 Packet Data Length (2 bytes)
 * @return CCSDS_primary_header
 */
CCSDS_primary_header ccsdsPrimaryHeader(unsigned short version, unsigned short type, unsigned short sec_header_flag,
                                        unsigned short proc_id, unsigned short seq_flags, unsigned short seq_cnt,
                                        unsigned short length);

/**
 * Create a CCSDS_secondary_header. Content is not defined in the recommended protocol, is user defined. In my case its
 * fixed length to SECONDARY_HEADER_LENGTH with epoch and version.
 * @param epoch - 4 bytes epoch, seconds since 1970
 * @param majorVersionNumber - 1 byte
 * @param minorVersionNumber - 1 byte
 * @param patchVersionNumber - 1 byte
 * @return CCSDS_secondary_header
 */
CCSDS_secondary_header ccsdsSecondaryHeader(unsigned int epoch, unsigned char majorVersionNumber,
                                            unsigned char minorVersionNumber, unsigned char patchVersionNumber);

/**
 * Build CCSDS_data_field given CCSDS_secondary_header and a pointer to userData
 * @param secondaryHeader - Pointer to CCSDS_secondary_header, if there is no secondary_header put it to NULL
 * @param data - Pointer to userData, length is defined in CCSDS_primary_header
 * @return CCSDS_data_field
 */
CCSDS_data_field ccsdsDataField(CCSDS_secondary_header* secondaryHeader, void* data);

/**
 * Build CCSDS_packet
 * @param primaryHeader - pointer to CCSDS_primary_header
 * @param dataField - pointer to CCSDS_data_field
 * @return CCSDS_packet
 */
CCSDS_packet ccsdsPacketBuild(CCSDS_primary_header* primaryHeader, CCSDS_data_field* dataField);    //TODO: if no userData put at least one byte

// Given an I/O stream read primary header and put it into packet->primaryHeader
void ccsdsReadPrimaryHeader(FILE *fp, CCSDS_packet *packet);

// Given an I/O stream read secondary header and put it into packet->dataField->secondaryHeader
void ccsdsReadSecondaryHeader(FILE *fp, CCSDS_packet *packet);

// Given an I/O stream malloc length and print userData. Not recommended, only for testing. Big userData length can collapse system.
void ccsdsReadFullUserData(FILE *fp, CCSDS_packet *packet);

// Given an I/O stream print packet
size_t write_packet(FILE *fp, CCSDS_packet *packet);

// Given a CCSDS_secondary_header print its content
void printSecondaryHeader(CCSDS_secondary_header* secondaryHeader);

// Given a CCSDS_packet print the DataField Content. If the packet have secondary header
// it prints too.
void printDataField(CCSDS_packet* packet);

// Given a CCSDS_primary_header print the primary header content
void printPrimaryHeader(CCSDS_primary_header* packet);

/**
 * Write ccsds packet into a pointer using the correct binary structure. Data field of the packet is not modified, only copied.
 * @param pointer to CCSDS_packet
 * @return pointer to address where binary packet is written. Length is set on ccsds packet header
 */
void* writeInBuffer(CCSDS_packet *packet);


#endif
