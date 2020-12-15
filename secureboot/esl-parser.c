/*
*  US Government source code released to the public domain
*  A product of NSA's Cybersecurity Directorate
*
*  Lol, yeah, this code is hacked together quickly and could be so much more optimized. FIXME
*/

#define _CRT_SECURE_NO_DEPRECATE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  float         version = 1.1;
  FILE          *infile,
                *outfile;
  unsigned char *inbuf,
                *outbuf;
  unsigned int  inbuflen,
                inbufcalclen,
                outbuflen,
                temp,
                loops;

//HELP AND VERSION TEXT OUTPUT CHECKS RETURN 0
  //display help
  if(argc > 1 && (strcmp("--help", argv[1]) == 0 || strcmp("-h", argv[1]) == 0)) {
    printf("Use: esl-parser infile.esl outfile.txt\n"
           "  This program parses EFI Signature List (ESL) files. Hashes are recorded in\n"
           "    hexadecimal format in a text (TXT) file. Certificates are stored as\n"
           "    uuid.cer.\n"
           "\n"
           "  Inputs: infile.esl is a text string name for the ESL input file.\n"
           "          outfile.txt is a text file to be populated with hex values.\n"
           "  Outputs: A text file full of hashes, or certificates(s).\n"
           "\n"
           "  Example usage:\n"
           "    esl-parser allowlist.esl allowed.txt\n");
    return 0;
    }

  //display version
  if(argc > 1 && (strcmp("--version", argv[1]) == 0 || strcmp("-v", argv[1]) == 0)) {
    printf("%f\n", version);
    return 0;
    }

//ARGUMENT COUNT CHECKS RETURN 1
  //argument count check
  if(argc != 3) {
    printf("Error: exactly 2 arguments (infile, outfile) are required\n");
    return 1;
    }

//ARGUMENT VALIDATION RETURN 2
  //validate the output filename
  if(strlen(argv[1]) > 32) {
    printf("Error: infile name too long\n");
    return 2;
    }
  if(strlen(argv[2]) > 32) {
    printf("Error: outfile name too long\n");
    return 2;
    }

  //open the infile
  if((infile = fopen(argv[1], "r")) == NULL) {
    printf("Error: could not open infile\n");
    return 3;
  }
  
  //get the length of the infile
  fseek(infile, 0, SEEK_END);
  inbuflen = ftell(infile);
  fseek(infile, 0, SEEK_SET);
  
  //allocate a buffer to contain the infile
  inbuf = (char*)calloc(inbuflen, sizeof(char));
  
  //read the entire infile into the buffer
  fread(inbuf, sizeof(char), inbuflen, infile);
  
  //check to make sure we have at least 28 bytes
  if(inbuflen < 28) {
    printf("Error: infile is too short\n");
    free(inbuf);
    fclose(infile);
    return 3;
  }
  
  //read the file length as recorded in the ESL structure
  inbufcalclen = 0;
  inbufcalclen = (inbuf[19] << 24) | (inbuf[18] << 16) | (inbuf[17] << 8) | inbuf[16];
  
  //our seek size should match the ESL structure's size
  if(inbuflen != inbufcalclen) {
    printf("Error: infile length and calculated EFI_GUID structure length mismatched\n");
    free(inbuf);
    fclose(infile);
    return 3;
  }

//INFILE VALIDATION RETURN 3
  //Check for hashes
  if(inbuf[0] == 38 && inbuf[15] == 40) {
  //HASHES
    //Infile must be (size - 28) % 48 = 0 when hashes are present
    if(((inbufcalclen - 28) % 48) != 0) {
      printf("Error: infile length - 28 must be a multiple of 48\n");
      free(inbuf);
      fclose(infile);
      return 3;
    }
    
    //calculate number of loops
    loops = (inbufcalclen - 28) / 48;
    
    //data in the infile is good enough, so parse it to the outfile
    outbuflen = inbufcalclen - 28;
    outbuf = (char*)calloc(outbuflen, sizeof(char));
    for(int i = 0; i < loops; i++) {
      //convert mixed-endian Microsoft COM/OLE format to normal big-endian
      outbuf[i*48+3] = inbuf[i*48+28];
      outbuf[i*48+2] = inbuf[i*48+1+28];
      outbuf[i*48+1] = inbuf[i*48+2+28];
      outbuf[i*48] = inbuf[i*48+3+28];
      
      outbuf[i*48+5] = inbuf[i*48+4+28];
      outbuf[i*48+4] = inbuf[i*48+5+28];

      outbuf[i*48+7] = inbuf[i*48+6+28];
      outbuf[i*48+6] = inbuf[i*48+7+28];
      
      outbuf[i*48+8] = inbuf[i*48+8+28];
      outbuf[i*48+9] = inbuf[i*48+9+28];
      outbuf[i*48+10] = inbuf[i*48+10+28];
      outbuf[i*48+11] = inbuf[i*48+11+28];
      outbuf[i*48+12] = inbuf[i*48+12+28];
      outbuf[i*48+13] = inbuf[i*48+13+28];
      outbuf[i*48+14] = inbuf[i*48+14+28];
      outbuf[i*48+15] = inbuf[i*48+15+28];
      
      //grab the hash
      for(int j = 16; j < 48; j++)
        outbuf[i*48+j] = inbuf[i*48+j+28];
    }
    
  //OUTFILE ERRORS RETURN 4
    //open output file for writing and overwriting
    if((outfile = fopen(argv[2], "wb+")) == NULL) {
      printf("Error: could not open outfile\n");
      free(outbuf);
      free(inbuf);
      fclose(infile);
      return 4;
    }

    for(int i = 0; i < loops; i++) {
      //write content to output file
      for(int j = 0; j < 48; j++) {
        if(j == 4 || j == 6 || j == 8 || j == 10) fprintf(outfile, "-");
        else if(j == 16) fprintf(outfile, " ");
        
        fprintf(outfile, "%02x", outbuf[i * 48 + j]);
        
        if(j == 47) fprintf(outfile, "\n");
      }
    }
  }
  else if(inbuf[0] == 161 && inbuf[15] == 114) {
  //CERTIFICATES
    printf("certificates\n");
    //data in the infile is good enough, so parse it into outfiles
    //FIXME
  }
  else {
    printf("Error: failed to detect EFI_CERT_X509_GUID or EFI_CERT_SHA256_GUID in infile\n");
    free(inbuf);
    fclose(infile);
    return 3;
  }

  free(outbuf);  
  free(inbuf);
  fclose(infile);

  return 0;
}
