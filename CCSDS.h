#ifndef CCSDS_h
#define CCSDS_h

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
#define SEQUENCE_FLAG_FIRST 0b00                // 4.1.2.4.2.2 b)
#define SEQUENCE_FLAG_LAST 0b00                 // 4.1.2.4.2.2 c)
#define SEQUENCE_FLAG_UNSEGMENTED 0b00          // 4.1.2.4.2.2 d)
#define SECONDARY_HEADER_LENTH 8                // Number of bytes of secondary header
#define PRIMARY_HEADER_LENGTH 8                 // Number of bytes of primary header

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
    CCSDS_secondary_header secondaryHeader;
    void* data;
} CCSDS_data_field;

typedef struct{
    CCSDS_primary_header primary_header;
    CCSDS_data_field dataField;
} CCSDS_packet;

void printData(CCSDS_packet *packet);
void ccsdsGetFullData(FILE *fp, CCSDS_packet *packet);

CCSDS_primary_header ccsdsPrimaryHeader(unsigned short version, unsigned short type, unsigned short sec_header_flag,
                                  unsigned short proc_id, unsigned short seq_flags, unsigned short seq_cnt,
                                  unsigned short length);      //Build a CCSDS_primary_header packet

CCSDS_secondary_header ccsdsSecondaryHeader(unsigned int epoch, unsigned char majorVersionNumber,
                                            unsigned char minorVersionNumber, unsigned char patchVersionNumber);
                                            // Build a CCSDS_secondary_header packet

CCSDS_data_field ccsdsDataField(CCSDS_secondary_header secondaryHeader, void *data);    // Build data field given second header and pointer

CCSDS_packet ccsdsPacket(CCSDS_primary_header primaryHeader, CCSDS_data_field dataField);   // Build CCSDS packet

void printSecondaryHeader(CCSDS_secondary_header secondaryHeader);        // Giver a secondary header print data

#endif
