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
#define SECONDAY_HEADER_FLAG_EXIST 0b0          // 4.1.2.3.3.2
#define SECONDAY_HEADER_FLAG_NOTEXIST 0b1
#define SEQUENCE_FLAG_CONT 0b00                 // 4.1.2.4.2.2 a)
#define SEQUENCE_FLAG_FIRST 0b00                // 4.1.2.4.2.2 b)
#define SEQUENCE_FLAG_LAST 0b00                 // 4.1.2.4.2.2 c)
#define SEQUENCE_FLAG_UNSEGMENTED 0b00          // 4.1.2.4.2.2 d)

typedef struct {
    unsigned short version : 3;
    unsigned short type : 1;
    unsigned short sec_header_flag : 1;
    unsigned short proc_id : 11;
    unsigned short seq_flags : 2;
    unsigned short seq_cnt : 14;    //Seq count or packet name
    unsigned short length;
} CCSDS_primary;

typedef struct{
    CCSDS_primary primary_header;
    void* data;
} CCSDS_packet;

void printData(CCSDS_packet *packet);
void putData(FILE *fp, CCSDS_packet *packet);

#endif
