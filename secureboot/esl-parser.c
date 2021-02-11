/*
*  US Government source code released to the public domain
*  A product of NSA's Cybersecurity Directorate
*
*  Parse ESL (EFI Signature List) files produced by Linux or Windows utilities.
*    Complex files with multiple sequential lists and numerous records are
*    supported. Mixed type ESLs are also supported.
*
*  Input: An ESL file is expected for parsing. ESL files can be created by
*    Linux efi-tools utilities or Windows PowerShell. Multiple lists can be in
*    a single ESL file. ESLs containing certificates and hashes are supported.
*    The .esl extension is not required. ESL files are expected to be binary
*    data.
*
*  Output: EFI certificates are DER encoded. Each unique certificate detected
*    will be written to a file outfilename.###.cer. The #'s are replaced by an
*    integer that starts at 001 and increments up to 999. Files with matching
*    names will be overwritten.
*
*    Hashes are appended to the file outfilename.txt. Each line consists of an
*    originator UUID and SHA-256 hash. Both values are in hex. Outfilename.txt
*    will always be appended to -- not overwritten or cleared/deleted.
*
*  Code supports Linux and Windows.
*  Linux compile: gcc esl-parser.c -o esl-parser -std=c99
*  Windows compile: cl esl-parser.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>

//constants
#define PROGRAM_VERSION 1
#define MAX_FILENAME_LENGTH 1024

//error codes
#define ERROR_HELP 1
#define ERROR_VERSION 2
#define ERROR_INFILE_MISSING 10
#define ERROR_INFILE_LENGTH 11
#define ERROR_INFILE_READ 12
#define ERROR_INFILE_CONTENT 13
#define ERROR_INFILE_STRUCTURE 14
#define ERROR_INFILE_TRUNCATION 15
#define ERROR_OUTFILE_MISSING 20
#define ERROR_OUTFILE_LENGTH 21
#define ERROR_OUTFILE_OPEN 22

// **** PRINT HELP INFO **** //
void printHelp() {
  printf("Use: esl-parser -i infile.esl -o outfile\n"
         "  This program parses EFI Signature List (ESL) files produced by Linux\n"
         "  and Windows utilities. ESL files are expected to be in binary format\n"
         "  and contain one or more SHA-256 hashes and/or certificates. Mixed\n"
         "  lists are supported. Multiple appended/concatenated lists are also\n"
         "  supported\n"
         "\n"
         "  Inputs: --in | -i | /i [infile]    Name of binary esl input file\n"
         "          --out | -o | /o [outfile]  Base name of file to be written to\n"
         "          --version | -v | /v        Display version number\n"
         "          --help | -h | /h           Display this help message\n"
         "          --debug | -d | /d          Print additional debugging details\n"
         "          --silent | -s | /s         Do not print error messages\n"
         "\n"
         "  Outputs: Outfile.txt shall contain a list of detected hashes as:\n"
         "             [Originator UUID] [SHA-256]\n"
         "           Hash records/lines are appended to outfile.txt. The file is\n"
         "           not cleared or overwritten. Outfile.txt will be created if it\n"
         "           does not exist.\n"
         "\n"
         "           Outfile.###.cer shall contain a DER encoded certificate. One\n"
         "           certificate per file. ### increments from 001 to 999. Files\n"
         "           will be overwritten if they exist, or created if they do not\n"
         "           exist.\n"
         "\n"
         "  Example usage:\n"
         "    esl-parser -i db.esl -o frontdeskpc\n"
         "\n"
         "    esl-parser -i dbx.old.esl -o exclusions -d\n"
         "\n"
         "    esl-parser -v\n");
  exit(ERROR_HELP);
}

// **** PRINT VERSION INFO **** //
void printVersion() {
  printf("%i\n", PROGRAM_VERSION);
  exit(ERROR_VERSION);
}

// **** SHA-256 HASH PARSER **** //
int parseHashes(bool debug, bool silent, FILE *infile, unsigned int hashcount, unsigned char *outfilename) {
  unsigned int  listsize = 0,
                headersize = 0,
                sigsize = 0,
                records = 0,
                readcount = 0;
  unsigned char *buf = NULL,
                outfilenameEX[MAX_FILENAME_LENGTH + 4] = "";
  FILE          *outfile = NULL;

  //read the structure sizing and record count info
  fread(&listsize, 4, 1, infile);
  fread(&headersize, 4, 1, infile);
  fread(&sigsize, 4, 1, infile);
  records = (listsize - 28) / sigsize;

  if(debug) printf("Parsing hash list\n  listsize %i, header %i, sigsize %i, records %i\n", listsize, headersize, sigsize, records);
  
  //add the txt extension to the filename
  strcat(outfilenameEX, outfilename);
  strcat(outfilenameEX, ".txt");

  //open the outfile for appending
  if((outfile = fopen(outfilenameEX, "a")) == NULL) {
    if(!silent) printf("Error: could not open outfile %s\n", outfilenameEX);
    return UINT_MAX - 1;
  }

  if(debug) printf("Writing hashes to %s\n", outfilenameEX);

  //set up a buffer big enough for origiinator UUID and SHA-256
  buf = calloc(sigsize, sizeof(unsigned char));
  for(int i = 0; i < records; i++) {
    //read originator UUID and SHA-256 records one at a time
    readcount = fread(buf, sigsize, 1, infile);
    if(debug) printf("  Parsing hash %i (%i bytes, %i read boolean)\n", hashcount, sigsize, readcount);
    
    //validate that all 48 bytes were read (16 originator UUID + 32 SHA-256 = 48)
    if(readcount != 1) {
      if(!silent) printf("Error: Signature size exceeds bytes actually read.\n");
      free(buf);
      fclose(outfile);
      return UINT_MAX;
    }
    
    //write each byte to file
    for(int j = 0; j < sigsize; j++) {
      fprintf(outfile, "%02x", buf[j]);    //write as hex string with a zero pad if necessary
      
      //add hashes to the originator UUID, and a space between it and the hash
      if(j == 4 || j == 6 || j == 8 || j == 10)
        fprintf(outfile, "-");
      else if(j == 16)
        fprintf(outfile, " ");
    }
    
    fprintf(outfile, "\n");
    hashcount++;
  }
  
  free(buf);
  fclose(outfile);
  return hashcount;
}

// **** CERTIFICATE PARSER **** //
int parseCerts(bool debug, bool silent, FILE *infile, unsigned int certcount, unsigned char *outfilename) {
  unsigned int  listsize = 0,
                headersize = 0,
                sigsize = 0,
                records = 0,
                readcount = 0;
  unsigned char *buf = NULL,
                outfilenameEX[MAX_FILENAME_LENGTH + 7] = "",
                temp[4];
  FILE          *outfile = NULL;
  
  //read size and record count info
  fread(&listsize, 4, 1, infile);
  fread(&headersize, 4, 1, infile);
  fread(&sigsize, 4, 1, infile);
  records = (listsize - 28) / sigsize;

  if(debug) printf("Parsing cert list\n  listsize %i, header %i, sigsize %i, records %i\n", listsize, headersize, sigsize, records);

  //null terminate the outfile name
  strcat(outfilenameEX, outfilename);
  
  //make a buffer big enough to contaiin each cert
  buf = calloc(sigsize, sizeof(unsigned char));
  for(int i = 0; (i < records) && (certcount < 1000); i++) {
    //copy the entire originator UUID and cert into our buffer
    readcount = fread(buf, sigsize, 1, infile);
    if(debug) printf("  Parsing cert %i (%i bytes, %i read boolean)\n", certcount, sigsize, readcount);
    
    //validate that the file had the amount of data the structure indicated
    if(readcount != 1) {
      if(!silent) printf("Error: Signature size exceeds bytes actually read.\n");
      free(buf);
      return UINT_MAX;
    }
    
    //append the outfilename with a 3-digit number and the cer extension (certs are DER encoded)
    sprintf(temp, "%03d", certcount);
    strcat(outfilenameEX, temp);
    strcat(outfilenameEX, ".cer");

    //open the outfile
    if((outfile = fopen(outfilenameEX, "w")) == NULL) {
      if(!silent) printf("Error: could not open outfile %s\n", outfilenameEX);
      return UINT_MAX - 1;
    }

    //one certificate per file
    if(debug) printf("Writing certificate to %s\n", outfilenameEX);
    
    //write the certificate to the file, but omit the originator UUID
    for(int j = 16; j < sigsize; j++) {
      fputc(buf[j], outfile);
    }
    
    fclose(outfile);
    certcount++;
  }
  
  free(buf);
  return certcount++;
}

// **** MAIN FUNCTION **** //
int main(int argc, char *argv[]) {
  bool          debug = false,
                silent = false;
  char          *infilename = NULL,
                *outfilename = NULL;
  unsigned char efisigtype[16];
  FILE          *infile = NULL;
  unsigned int  infilelen = 0,
                hashcount = 1,
                certcount = 1,
                offset = 0;

  //process arguments
  if(argc < 2)
    printHelp();  //always print help if there are not enough arguments
  else {
    for(int i = 1; i < argc; i++) {
      if(!strcmp("-h", argv[i]) || !strcmp("/?", argv[i]) || !strcmp("--help", argv[i]))
        printHelp();
      else if(!strcmp("-v", argv[i]) || !strcmp("/v", argv[i]) || !strcmp("--version", argv[i]))
        printVersion();
      else if(!strcmp("-d", argv[i]) || !strcmp("/d", argv[i]) || !strcmp("--debug", argv[i]))
        debug = true;
      else if(!strcmp("-s", argv[i]) || !strcmp("/s", argv[i]) || !strcmp("--silent", argv[i]))
        silent = true;
      else if((!strcmp("-i", argv[i]) || !strcmp("/i", argv[i]) || !strcmp("--in", argv[i])) && (i + 1) < argc)
        infilename = argv[++i];
      else if((!strcmp("-o", argv[i]) || !strcmp("/o", argv[i]) || !strcmp("--out", argv[i])) && (i + 1) < argc)
        outfilename = argv[++i];
    }
  }
  
  //validate arguments
  if(infilename == NULL) {
    if(!silent) printf("Error: Infile is missing.\n");
    exit(ERROR_INFILE_MISSING);
  }
  else if(strlen(infilename) > MAX_FILENAME_LENGTH) {
    if(!silent) printf("Error: Infile name is too long (max %i).\n", MAX_FILENAME_LENGTH);
    exit(ERROR_INFILE_LENGTH);
  }
    
  if(outfilename == NULL) {
    if(!silent) printf("Error: Outfile is missing.\n");
    exit(ERROR_OUTFILE_MISSING);
  }
  else if(strlen(outfilename) > MAX_FILENAME_LENGTH) {
    if(!silent) printf("Error: Outfile name is too long (max %i).\n", MAX_FILENAME_LENGTH);
    exit(ERROR_OUTFILE_LENGTH);
  }
  
  //open infile
  if((infile = fopen(infilename, "r")) == NULL) {
    if(!silent) printf("Error: Could not open infile.\n");
    exit(ERROR_INFILE_READ);
  }
  
  //validate infile by length
  fseek(infile, 0, SEEK_END);
  infilelen = ftell(infile);
  fseek(infile, 0, SEEK_SET);
  if(infilelen < 76) {
    if(!silent) printf("Error: Infile is too short. Minimum length is 76 bytes.\n");
    fclose(infile);
    exit(ERROR_INFILE_CONTENT);
  }
  
  //parse the infile
  while((ftell(infile) < infilelen) && (certcount < 1000)) {
    //read the EFI_GUID
    fread(efisigtype, 16, 1, infile);

    //branch based off the first and last values of EFI_GUID (validate by values)
    if(efisigtype[0] == 38 && efisigtype[15] == 40)
      hashcount = parseHashes(debug, silent, infile, hashcount, outfilename);
    else if(efisigtype[0] == 161 && efisigtype[15] == 114)
      certcount = parseCerts(debug, silent, infile, certcount, outfilename);
    else {
      if(!silent) printf("Error: Malformed infile header structure.\n");
      fclose(infile);
      exit(ERROR_INFILE_STRUCTURE);
    }
    
    //handle parser errors
    if(hashcount == UINT_MAX || certcount == UINT_MAX) {
      fclose(infile);
      exit(ERROR_INFILE_TRUNCATION);
    }
    else if(hashcount == UINT_MAX - 1 || certcount == UINT_MAX - 1) {
      fclose(infile);
      exit(ERROR_OUTFILE_OPEN);
    }
  }

  //cleanup
  fclose(infile);
  return 0;
}
