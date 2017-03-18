/**
 * Resizes a BMP piece by piece.
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./resize n infile outfile\n");
        return 1;
    }
    
    // convert input and ensure valid float
    float f;
    // modified this approach from cs50 get_float
    char *tail;
    errno = 0;
    f = strtof(argv[1], &tail);
    // ensure that input could be converted to float without error.
    if (errno != 0 || *tail != '\0') {
        fprintf(stderr, "Usage: ./resize n(float) infile outfile\n");
        return 1;
    }
    // ensure within allowed range
    if (f < 0.0 || f >= 100.0) {
        fprintf(stderr, "Usage: ./resize n(0.0, 100.0] infile outfile\n");
        return 1;
    }
    // disallow hexadecimal and exponents
    if (strcspn(argv[1], "XxEePp") != strlen(argv[1])) {
        fprintf(stderr, "Usage: ./resize n(float) infile outfile\n");
        return 1;
    }

    // remember filenames
    char *infile = argv[2];
    char *outfile = argv[3];

    // open input file 
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }
    
    // special case of f = 1.0, copy the file stright through
    if(f == 1.0) {
        // got this approach from fread() in cs50.reference
        char ptr[1];
        while(fread(ptr, 1, 1, inptr) == 1) {
            fwrite(ptr, 1, 1, outptr);
        }
        // close files
        fclose(inptr);
        fclose(outptr);
        // success
        return 0;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER inbf;
    fread(&inbf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER inbi;
    fread(&inbi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (inbf.bfType != 0x4d42 || inbf.bfOffBits != 54 || inbi.biSize != 40 || 
        inbi.biBitCount != 24 || inbi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }
    
    // determine padding for scanlines
    int inpadding = (4 - (inbi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // save infiles's scanlines as 2-D array of rgbtriples
    RGBTRIPLE intriples[abs(inbi.biHeight)][inbi.biWidth];
    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(inbi.biHeight); i < biHeight; i++) {
        // iterate over pixels in scanline
        for (int j = 0; j < inbi.biWidth; j++) {
            // read RGB triple from infile
            fread(&intriples[i][j], sizeof(RGBTRIPLE), 1, inptr);
        }
        // skip over padding, if any
        fseek(inptr, inpadding, SEEK_CUR);
    }
    // close infile
    fclose(inptr);    
    
    // calculate/set changes to outfile's BITMAPFILEHEADER and BITMAPINFOHEADER
    // start with copy of infile's headers
    BITMAPINFOHEADER outbi = inbi;
    BITMAPFILEHEADER outbf = inbf;
    outbi.biWidth  = (int) (inbi.biWidth * f); // doesnt include padding
    outbi.biHeight = (int) (inbi.biHeight * f);
    int outpadding = (4 - (outbi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    outbi.biSizeImage = ((sizeof(RGBTRIPLE) * outbi.biWidth) + outpadding) * abs(outbi.biHeight);
    outbf.bfSize = outbi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    
    // write outfile's BITMAPFILEHEADER
    fwrite(&outbf, sizeof(BITMAPFILEHEADER), 1, outptr);
    // write outfile's BITMAPINFOHEADER
    fwrite(&outbi, sizeof(BITMAPINFOHEADER), 1, outptr);
    
    // iterate over outfiles's scanlines
    for (int i = 0, biHeight = abs(outbi.biHeight); i < biHeight; i++) {
        // iterate over pixels in scanline
        for (int j = 0; j < outbi.biWidth; j++) {
            // write copy of infile's corresponding RGB triple to outfile based on math for size changes
            fwrite(&intriples[(int) (i/f)][(int) (j/f)], sizeof(RGBTRIPLE), 1, outptr);
        }
        // add padding
        for (int k = 0; k < outpadding; k++) {
            fputc(0x00, outptr);
        }
    }

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
