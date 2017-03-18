/**
 * Searches data file for JPEGs.
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t BYTE;

// isjpeghead returns 1 if the 1st 4 bytes meet jpeg header spec
int isjpeghead(BYTE b[]) {
    if (b[0] == 0xff &&
        b[1] == 0xd8 &&
        b[2] == 0xff &&
        (b[3] & 0xf0) == 0xe0) {
        return 1;      
    }
    return 0;
}

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover image\n");
        return 1;
    }
    
    // open input file 
    FILE *card = fopen(argv[1], "r");
    if (card == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", argv[1]);
        return 2;
    }

    BYTE buf[512];
    int imgnum = 0;
    char filename[] = "000.jpg";
    FILE *img = NULL;
    
    // read through card in blocks of 512 bytes, find and save jpegs
    while(fread(buf, 512, 1, card) == 1) {
        if(isjpeghead(buf)) {
            
            // close previous image
            if(img != NULL) {
                fclose(img);
            }
            
            // open new output file
            sprintf(filename,"%03d.jpg", imgnum++);
            img = fopen(filename, "w");
            if (img == NULL)
            {
                fclose(img);
                fprintf(stderr, "Could not create %s.\n", filename);
                return 3;
            }
        }
        // write the block of bytes if in an image
        if(img != NULL) {
            fwrite(buf, 512, 1, img);
        }
    }
    
    // close files
    fclose(img);
    fclose(card);
    
    // success
    return 0;
}
