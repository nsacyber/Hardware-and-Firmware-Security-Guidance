/*
*  US Government source code released to the public domain
*  A product of NSA's Cybersecurity Directorate
*
*  Take multiple externally-calculated SHA-256 hashes and compile them into a
*    single EFI Signature List (ESL) file.
*
*  Input: hex uuids and hex sha256 hashes representing precalculated EFI hashes
*    Precalculated assumes that the hashes were produced by vendors, BIOS/UEFI
*    config interface, or EFI hashing/signing tools. A simple sha256sum doesn't
*    comply with the PECOFF authentication specs.
*
*  Output: binary ESL ready for use with Keytool, MokManager, and MokUtil
*
*  Code supports Linux and Windows.
*  Linux compile: gcc hex-hashes-to-esl.c -o hex-hashes-to-esl -std=c99
*  Windows compile: cl hex-hashes-to-esl.c
*/

#define _CRT_SECURE_NO_DEPRECATE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  float version = 1.1;
  FILE  *outfile;
  char  byte,
        *buf;
  int   argl,
        buflen,
        bufcell;

//HELP AND VERSION TEXT OUTPUT CHECKS RETURN 0
  //display help
  if(argc > 1 && (strcmp("--help", argv[1]) == 0 || strcmp("-h", argv[1]) == 0)) {
    printf("Use: hex-hashes-to-esl [uuid]hex-hash [[uuid]hex-hash ...] efi-signature-list\n"
           "  This program writes 1 or up to 64 SHA256 hashes to an EFI Signature List.\n"
           "  Note that precomputed hex hashes are expected. Use hash-to-efi-sig-list to\n"
           "    process binaries. UUIDs are optional and will be zeros if omitted.\n"
           "\n"
           "  Inputs: [hex-uuid]hex-hash is the hex SHA256 hash of an EFI binary\n"
           "          efi-signature-list is the string name of the output file\n"
           "  Outputs: EFI Signature List (ESL) with 1 to 64 hashes; no certificates\n"
           "\n"
           "  Example usage:\n"
           "    hex-hashes-to-esl \\\n"
           "      0ee7f01275cea42913f6dc4a562180eceea69edf3105123a89f8b06e5f176ab7 \\\n"
           "      d41170ac846e271ab0f09d0df8b3eab3fa644e3773ad2bfe045884216730e168 \\\n"
           "      out.esl\n"
           "\n"
           "    hex-hashes-to-esl \\\n"
           "      7368696d7836342e65666900000000000\\\n"
           "      ee7f01275cea42913f6dc4a562180eceea69edf3105123a89f8b06e5f176ab7 \\\n"
           "      out.esl\n");
    return 0;
    }

  //display version
  if(argc > 1 && (strcmp("--version", argv[1]) == 0 || strcmp("-v", argv[1]) == 0)) {
    printf("%f\n", version);
    return 0;
    }

//ARGUMENT COUNT CHECKS RETURN 1
  //argument count check
  if(argc < 3) {
    printf("Error: too few arguments\n");
    return 1;
    }
  if(argc > 66) {
    printf("Error: too many arguments; maximum of 64 hashes\n");
    return 1;
    }

//ARGUMENT VALIDATION RETURN 2
  //validate hashes
  for(int i = 1; i < (argc - 1); i++) {
    if(strlen(argv[i]) != 64 && strlen(argv[i]) != 96) {
      printf("Error: hash %i is of incorrect length (expected 64 hex chars for sha256, or 96 hex chars for uuid+sha256)\n", i);
      return 2;
    }
  }

  //validate the output filename
  if(strlen(argv[argc - 1]) == 64) {
    printf("Error: last argument must be a filename, not a hash\n");
    return 2;
    }
  if(strlen(argv[argc - 1]) > 32) {
    printf("Error: filename too long for some UEFI config interfaces; 32 character max\n");
    return 2;
    }
  argl = strlen(argv[argc - 1]);
  if(argv[argc - 1][argl - 1] != 'l' ||
     argv[argc - 1][argl - 2] != 's' ||
     argv[argc - 1][argl - 3] != 'e' ||
     argv[argc - 1][argl - 4] != '.') {
    printf("Error: filename must end in .esl\n");
    return 2;
  }

//DO THE ACTUAL WORK BECAUSE OUR INPUT IS GOOD ENOUGH
  //construct the ESL data
  buflen = 16 + 4 + 4 + 4 + ((16 + 32) * (argc - 2));
  buf = (char*)calloc(buflen, sizeof(char));

  //insert EFI_GUID value for EFI_CERT_SHA256_GUID
  buf[0] = 38;
  buf[1] = 22;
  buf[2] = 196;
  buf[3] = 193;
  buf[4] = 76;
  buf[5] = 80;
  buf[6] = 146;
  buf[7] = 64;
  buf[8] = 172;
  buf[9] = 169;
  buf[10] = 65;
  buf[11] = 249;
  buf[12] = 54;
  buf[13] = 147;
  buf[14] = 67;
  buf[15] = 40;

  //set the size
  if(buflen > 255) {
    buf[16] = buflen / 16;
    buf[17] = buflen % 16;
  }
  else
    buf[16] = buflen;

  //all signatures are uuid + sha256 (48 bytes)
  buf[24] = 48;
  bufcell = 28;

  for(int i = 1; i < argc - 1; i++) {
    if(strlen(argv[i]) == 64) {
      //UUID not present so create one of all zero
      for(int j = 0; j < 16; j++) {
        buf[bufcell] = 0;
        bufcell++;
      }
    }

    for(int j = 0; j < strlen(argv[i]); j += 2) {
      byte = 0;

      //process the SHA256 hex string
      if(argv[i][j] >= '0' && argv[i][j] <= '9')
        byte = argv[i][j] - 48;
      else if(argv[i][j] >= 'a' && argv[i][j] <= 'f')
        byte = argv[i][j] - 87;
      else if(argv[i][j] >= 'A' && argv[i][j] <= 'F')
        byte = argv[i][j] - 55;

      byte *= 16;

      if(argv[i][j+1] >= '0' && argv[i][j+1] <= '9')
        byte += argv[i][j+1] - 48;
      else if(argv[i][j+1] >= 'a' && argv[i][j+1] <= 'f')
        byte += argv[i][j+1] - 87;
      else if(argv[i][j+1] >= 'A' && argv[i][j+1] <= 'F')
        byte += argv[i][j+1] - 55;

      buf[bufcell] = byte;
      bufcell++;
      }
    }

//FILE ACCESS CHECK RETURNS 3
  //open output file for writing and overwriting
  if((outfile = fopen(argv[argc - 1], "wb+")) == NULL) {
    printf("Error: could not open file\n");
    return 3;
  }

  //write content to output file
  fwrite(buf, sizeof(char), buflen, outfile);

  //cleanup
  fclose(outfile);
  free(buf);

  return 0;
}
