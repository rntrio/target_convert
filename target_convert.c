/* Public Domain */

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

void usage(char *prog) {
    fprintf(stderr,
            "%s -e|-d < infile > outfile\n"
            "\n"
            "-d    decode (t3000 -> t3001)\n"
            "-e    encode (t3001 -> t3000)\n",
            prog);
    exit(1);
}

int main(int argc, char** argv)
{
    if (argc < 4 || argv[1][0] != '-' || argv[1][2] != '\0' || (argv[1][1] != 'd' && argv[1][1] != 'e'))
        usage(argv[0]);

    bool encode = argv[1][1] == 'e';
    const char* in_path = argv[2];
    const char* out_path = argv[3];

    FILE* in_file = 0;
    errno_t err_in = fopen_s(&in_file, in_path, "rb");
    if (err_in != 0) {
        perror("Error opening input file");
        return 1;
    }

    FILE* out_file = 0;
    errno_t err_out = fopen_s(&out_file, out_path, "wb");
    if (err_out != 0) {
        perror("Error opening output file");
        fclose(in_file); // Make sure to close the input file if it was opened successfully
        return 1;
    }

    uint8_t p = 18;
    int i;
    while ((i = fgetc(in_file)) != EOF) {
        uint8_t c = i;
        if (encode)
            c = ((c << 5 | c >> 3) - p) ^ 0xCC;
        else {
            c = p + (c ^ 0xCC);
            c = c << 3 | c >> 5;
        }
        fputc(c, out_file);
        p++;
    }

    fclose(in_file);
    fclose(out_file);

    return 0;
}
