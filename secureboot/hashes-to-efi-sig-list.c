/* Take multiple externally-calculated SHA-256 hashes and compile them into a single EFI Signature List (ESL) file
 *  input: an output file names and one or more hexadecimal hashes
 *  output: binary ESL ready for use with Keytool
 */

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  //check for too few arguments (also handles help cases)
  if(argc < 3) {
    printf("Usage: hashes-to-efi-sig-list <output file name> <hex SHA256 hash> [<hex SHA256 hashes>]\n"
            "   This application compiles one or more SHA256 hashes into an EFI Signature List.\n"
            "   Note that this program takes hashes, not binaries. Use hash-to-efi-sig-list to process binaries.\n"
            "   Inputs: <output file name> text string output name like out.esl\n"
            "           <hex SHA256 hash> one or more user-provided hashes (from sha256sum or openssl)\n"
            "   Outputs: EFI Signature List with one or more hashes, no certificates\n"
            "   Example usage: hashes-to-efi-sig-list output.esl 0ee7f01275cea42913f6dc4a562180eceea69edf3105123a89f8b06e5f176ab7 d41170ac846e271ab0f09d0df8b3eab3fa644e3773ad2bfe045884216730e168\n");
    return 1;
    }

  //read the output file name
  

  //read the hashes
  for(int i = 2; i < argc; i++) {
    
    }

  //write content to output file
  return 1;
  }
