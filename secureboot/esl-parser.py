import sys
import os
import argparse

def read_file_as_bytes(file_path):
    try:
        with open(file_path, 'rb') as f:
            return bytearray(f.read())
    except Exception as e:
        print(f"Error: Could not load the input file. {e}")
        sys.exit(1)

def write_bytes_to_file(file_path, byte_data):
    with open(file_path, 'wb') as f:
        f.write(byte_data)

def main():
    #handle arguments and require in_file and out_prefix
    parser = argparse.ArgumentParser(description="Process an EFI Signature List file. Outputs .cer certificates and .hsh binary hash files.")
    parser.add_argument("in_file", type=str, help="Input ESL file name (required, eg: dbx.esl)")
    parser.add_argument("out_prefix", type=str, help="Output file prefix string (required, eg: dbx)")
    parser.add_argument("-d", "--debug", action='store_true', help="Enable debug mode (optional)")
    args = parser.parse_args()

    #try to read the input file
    byte_array = read_file_as_bytes(args.in_file)

    #stop if the input file is missing or too small
    if len(byte_array) < 76:
        print("Error: Input file is too small.")
        sys.exit(2)

    #get ready for parsing
    input_length = len(byte_array)
    head_index = 0
    cert_counter = 0
    hash_counter = 0

    if args.debug:
        print(f"Input file size is {input_length}")

    #there may be multiple ESL structures concatenated together in the input file
    while head_index < input_length:
        cert_mode = False
        hash_mode = False

        #do we have certs or hashes? An ESL only contains one type. Handle a non-match too
        if byte_array[head_index] == 161:
            if args.debug:
                print("Found certificate GUID")
            cert_mode = True
        elif byte_array[head_index] == 38:
            if args.debug:
                print("Found hash GUID")
            hash_mode = True
        else:
            print("Error: Unrecognized EFI_GUID signature type.")
            sys.exit(3)

        #handle the structure header info (same for certs and hashes)
        sig_list_size = (byte_array[head_index + 16] +
                         byte_array[head_index + 17] * 256 +
                         byte_array[head_index + 18] * 65536 +
                         byte_array[head_index + 19] * 16777216)

        if args.debug:
            print(f"Signature list size is {sig_list_size}")

        sig_header_size = (byte_array[head_index + 20] +
                           byte_array[head_index + 21] * 256 +
                           byte_array[head_index + 22] * 65536 +
                           byte_array[head_index + 23] * 16777216)

        if args.debug:
            print(f"Signature header size is {sig_header_size}")

        sig_size = (byte_array[head_index + 24] +
                    byte_array[head_index + 25] * 256 +
                    byte_array[head_index + 26] * 65536 +
                    byte_array[head_index + 27] * 16777216)

        if args.debug:
            print(f"Signature size is {sig_size}")

        #parse the actual payload now (1 or more certs, OR 1 or more hashes)
        if cert_mode:
            cert_name = f"{args.out_prefix}{cert_counter}.cer"
            out_certificate = byte_array[head_index + 44 + sig_header_size: head_index + sig_list_size]
            write_bytes_to_file(cert_name, out_certificate)
            if args.debug:
                print(f"Wrote certificate {cert_name}")
            cert_counter += 1

        elif hash_mode:
            total_hashes = (sig_list_size - 28) // sig_size
            if args.debug:
                print(f"Total hashes in this ESL are {total_hashes}")

            for i in range(total_hashes):
                hash_name = f"{args.out_prefix}{hash_counter}.hsh"
                out_hash = byte_array[head_index + 28 + sig_header_size + (i * sig_size) + 16:
                                      head_index + 28 + sig_header_size + (i * sig_size) + 47]
                write_bytes_to_file(hash_name, out_hash)
                if args.debug:
                    print(f"Wrote hash {hash_name}")
                hash_counter += 1

        head_index += sig_list_size

    #warn the user if there's a misalignment that indicates a problem with the ESL structure
    if head_index != input_length:
        print("Error parsing contents. Some data may be truncated.")
        sys.exit(4)

#go back up there and do the thing
main()
