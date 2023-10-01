#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t BYTE;
const int BLOCK_SIZE = 512;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./recover IMAGE\n");
        return 1;
    }

    // file
    FILE *raw = fopen(argv[1], "r");
    if (raw == NULL)
    {
        fclose(raw);
        printf("Could not open file.\n");
        return 1;
    }

    BYTE buffer[BLOCK_SIZE];
    int counter = 0;
    FILE *img = NULL;
    char filename[8];

    while (fread(buffer, sizeof(buffer), 1, raw) == 1)
    {
        //find jpeg
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            //close old jpeg, since new one found | can use "img != NULL" as alternative
            if (counter > 0)
            {
                fclose(img);
            }

            sprintf(filename, "%03i.jpg", counter);
            img = fopen(filename, "w");
            counter++;
        }
        // write the data. can use != null.
        if (counter > 0)
        {
            fwrite(buffer, sizeof(buffer), 1, img);
        }

    }
    fclose(img);
    fclose(raw);
    return 0;
}