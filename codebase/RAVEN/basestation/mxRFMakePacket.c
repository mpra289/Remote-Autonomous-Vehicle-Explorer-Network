//C MEX file for RF communication
#include "math.h"
#include "mex.h"

#define START_DELIM 0x7E

#define LEAD_SH1 0x00
#define LEAD_SH2 0x13
#define LEAD_SH3 0xA2
#define LEAD_SH4 0x00
#define LEAD_SL1 0x40
#define LEAD_SL2 0x47
#define LEAD_SL3 0x51
#define LEAD_SL4 0xDC
#define FOL1_SH1 0x00
#define FOL1_SH2 0x13
#define FOL1_SH3 0xA2
#define FOL1_SH4 0x00
#define FOL1_SL1 0x40
#define FOL1_SL2 0x47
#define FOL1_SL3 0x2C
#define FOL1_SL4 0x6E
#define BASE_SH1 0x00
#define BASE_SH2 0x13
#define BASE_SH3 0xA2
#define BASE_SH4 0x00
#define BASE_SL1 0x40
#define BASE_SL2 0x68
#define BASE_SL3 0xB4
#define BASE_SL4 0xBF


void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]){
    // [DELIM] [LENGTH] [TYPE] [ID] [DEST] [RES] [RADIUS] [OPTIONS] [PAYLOAD] [CHECKSUM]
    char payload;
    char destination;
    char packet[19];
    int checksum = 0;
    char frameID;
    double* returnPacket;
    int i;
    plhs[0] = mxCreateDoubleMatrix(1,19,mxREAL);
    
    if(nlhs > 1){
        mexErrMsgTxt("too many output arguments");
        return;
    }
    if(nrhs != 3){
        mexErrMsgTxt("incorrect command arguments");
        return;
    }

    returnPacket = mxGetPr(plhs[0]);

    destination = *(mxGetPr(prhs[0]));
    payload = *(mxGetPr(prhs[1]));
    frameID = *(mxGetPr(prhs[2]));
    
    packet[0] = START_DELIM;
    packet[1] = 0x00;
    packet[2] = 0x0F; //length = 15
    packet[3] = 0x10; // transmit request
    checksum += packet[3];
    packet[4] = frameID;
    checksum += packet[4];
    
    switch(destination){
        case 1:
            // LEADER
            packet[5] = LEAD_SH1;
            packet[6] = LEAD_SH2;
            packet[7] = LEAD_SH3;
            packet[8] = LEAD_SH4;
            packet[9] = LEAD_SL1;
            packet[10] = LEAD_SL2;
            packet[11] = LEAD_SL3;
            packet[12] = LEAD_SL4;
            checksum += packet[5]+packet[6]+packet[7]+packet[8];
            checksum += packet[9]+packet[10]+packet[11]+packet[12];
            break;
        case 2:
            // FOLLOWER (1)
            packet[5] = FOL1_SH1;
            packet[6] = FOL1_SH2;
            packet[7] = FOL1_SH3;
            packet[8] = FOL1_SH4;
            packet[9] = FOL1_SL1;
            packet[10] = FOL1_SL2;
            packet[11] = FOL1_SL3;
            packet[12] = FOL1_SL4;
            checksum += packet[5]+packet[6]+packet[7]+packet[8];
            checksum += packet[9]+packet[10]+packet[11]+packet[12];
            break;
         default:
            break;
    }

    packet[13] = 0xFF;
    checksum += packet[13];
    packet[14] = 0xFE; // reserved
    checksum += packet[14];
    packet[15] = 0x00; // radius
    packet[16] = 0x00; // options
    packet[17] = payload;
    checksum += packet[17];
    packet[18] = 0xFF - (char)(checksum & 0xFF);

    // convert to characters (positive)
    for(i=0;i<19;i++){
        returnPacket[i] = packet[i];
        if(returnPacket[i] < 0){
            returnPacket[i] += 256;
        }
    }
    
    

    return;
}
