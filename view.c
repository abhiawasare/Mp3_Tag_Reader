#include "main.h"

/*
 * Function Name : view_tags
 * Description   : Reads and displays ID3v2 tags from an MP3 file.
 * Input         : filename -> Name of the MP3 file
 * Output        : SUCCESS or FAILURE
 */

int view_tags(char *filename)
{
    /* Open the MP3 file in binary read mode */
    FILE *fp = fopen(filename, "rb");

    /* Check if file opened successfully */
    if(fp == NULL)
    {
        printf("\033[31mERROR: File Not Opening\n\033[0m");
        return FAILURE;
    }

    /* ID3 header is 3 bytes ("ID3") */
    char header[4];

    /* Read first 3 bytes from file */
    fread(header, 3, 1, fp);

    /* Add null character to make it a string */
    header[3] = '\0';

    /* Verify whether file contains ID3 tag */
    if(strcmp(header, "ID3") != 0)
    {
        printf("\033[31mERROR: ID3 tag not found\n\033[0m");
        fclose(fp);
        return FAILURE;
    }

    /*
     * ID3 header size = 10 bytes
     * First 3 bytes = "ID3"
     * Next 7 bytes = version, flags, size
     * So skip total 10 bytes to reach first frame
     */

    fseek(fp, 10, SEEK_SET);

    char frame_id[5];              // To store frame name (e.g., TIT2)
    unsigned char size_buf[4];     // To store 4-byte frame size
    int frame_size;                // To store calculated frame size
    char data[1024];               // To store tag content

    printf("\033[35m--------------- [ MP3 TAG DETAILS ] ---------------\n\033[0m");

    /*
     * Loop to read each frame
     * Each frame structure:
     * 4 bytes -> Frame ID
     * 4 bytes -> Frame size
     * 2 bytes -> Flags
     * n bytes -> Data
     */

    while (fread(frame_id, 4, 1, fp) == 1)
    {
        /* Add null terminator to frame ID */
        frame_id[4] = '\0'; 
        
        /* Read 4 bytes of frame size */
        fread(size_buf, 4, 1, fp);

        /*
         * Convert 4 bytes into integer
         * Big-endian format conversion
         */
        frame_size = (size_buf[0] << 24) |
                     (size_buf[1] << 16) |
                     (size_buf[2] << 8) |
                     (size_buf[3]);

        /* Skip 2 bytes of frame flags */
        fseek(fp, 2, SEEK_CUR);

        /*
         * Validation:
         * If frame size invalid or too large,
         * break the loop to avoid corruption
         */
        if(frame_size <= 0 || frame_size > 1000)
        {
            break;
        }

        fgetc(fp);

        /*
         * Read actual frame data
         * frame_size - 1 because we skipped encoding byte
         */
        fread(data,frame_size-1, 1, fp);

        /* Add null terminator to make it string */
        data[frame_size - 1] = '\0';

        /*
         * Compare frame ID and print corresponding tag
         */
        if(strcmp(frame_id, "TIT2") == 0)
            printf("Title : %s\n", data);
        else if(strcmp(frame_id, "TPE1") == 0)
            printf("Artist : %s\n",data);
        else if(strcmp(frame_id, "TALB") == 0)
            printf("Album : %s\n",data);
        else if (strcmp(frame_id,"TYER") == 0 || strcmp(frame_id, "TDRC") == 0)
            printf("Year : %s\n",data);
        else if(strcmp(frame_id, "COMM") == 0)
            printf("Comment : %s\n",data);

        /* Other frames are ignored */
        else
        {

        }
    }
    printf("-----------------------------------------\n");
    fclose(fp);
    return SUCCESS;
}