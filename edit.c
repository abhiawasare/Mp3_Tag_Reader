#include "main.h"

/* Function to edit selected ID3 tag in MP3 file */
int edit_tags(char *filename, char *tag, char *value)
{
    /* ---------- STEP 1: Create Backup of Original File ---------- */

    char backup_name[100];
    sprintf(backup_name, "backup_%s", filename);

    FILE *orig = fopen(filename, "rb");
    if(orig == NULL)
    {
        printf("\033[31mERROR: File Not Opening\n\033[0m");
        return FAILURE;
    }

    FILE *backup_file = fopen(backup_name, "wb");
    if(backup_file == NULL)
    {
        fclose(orig);
        printf("\033[31mERROR: Backup file not created\n\033[0m");
        return FAILURE;
    }

    /* Copy complete original file into backup */
    char ch;
    while(fread(&ch, 1, 1, orig))
    {
        fwrite(&ch, 1, 1, backup_file);
    }

    fclose(orig);
    fclose(backup_file);

    printf("\033[32mBackup created: %s\n\033[0m", backup_name);


    /* ---------- STEP 2: Open Original and Temp File for Editing ---------- */

    FILE *fp = fopen(filename, "rb");
    if(fp == NULL)
    {
        printf("\033[31mERROR: File reopening failed\n\033[0m");
        return FAILURE;
    }

    /* Temp file will store edited content */
    FILE *temp = fopen("temp.mp3", "wb");
    if(temp == NULL)
    {
        fclose(fp);
        printf("\033[31mERROR: Temp file not created\n\033[0m");
        return FAILURE;
    }

    /* Copy first 10 bytes (ID3 header) */
    char header[10];
    fread(header, 10, 1, fp);
    fwrite(header, 10, 1, temp);

    char frame_id[5];
    unsigned char size_buf[4];
    int frame_size;
    char data[4096];

    /* ---------- STEP 3: Read Frames One by One ---------- */

    while (fread(frame_id, 4, 1, fp) == 1)
    {
        frame_id[4] = '\0';

        /* Read frame size */
        fread(size_buf, 4, 1, fp);
        frame_size = (size_buf[0] << 24) |
                     (size_buf[1] << 16) |
                     (size_buf[2] << 8)  |
                     (size_buf[3]);

        /* Read frame flags */
        char flags[2];
        fread(flags, 2, 1, fp);

        /* Write frame header to temp file */
        fwrite(frame_id, 4, 1, temp);
        fwrite(size_buf, 4, 1, temp);
        fwrite(flags, 2, 1, temp);

        int match = 0;

        /* Check which tag user wants to edit */
        if(strcmp(tag, "-t") == 0 && strcmp(frame_id, "TIT2") == 0)
            match = 1;
        else if(strcmp(tag, "-a") == 0 && strcmp(frame_id, "TPE1") == 0)
            match = 1;
        else if(strcmp(tag, "-A") == 0 && strcmp(frame_id, "TALB") == 0)
            match = 1;
        else if(strcmp(tag, "-y") == 0 &&
               (strcmp(frame_id, "TYER") == 0 || strcmp(frame_id, "TDRC") == 0))
            match = 1;
        else if(strcmp(tag, "-c") == 0 && strcmp(frame_id, "COMM") == 0)
            match = 1;

        if(match)
        {
            /* Skip old frame data */
            fseek(fp, frame_size, SEEK_CUR);

            /* Write encoding byte */
            fputc(0, temp);

            /* Write new tag value */
            fwrite(value, strlen(value), 1, temp);

            /* Fill remaining bytes with zeros */
            int remain = frame_size - (strlen(value) + 1);
            while(remain-- > 0)
                fputc(0, temp);
        }
        else
        {
            /* Validate frame size */
            if(frame_size <= 0 || frame_size > 4096)
                break;

            /* Copy original frame data */
            fread(data, frame_size, 1, fp);
            fwrite(data, frame_size, 1, temp);
        }
    }

    /* Close files */
    fclose(fp);
    fclose(temp);

    /* ---------- STEP 4: Replace Original With Edited File ---------- */

    remove(filename);                 // Delete old original
    rename("temp.mp3", filename);     // Rename temp as original

    printf("\033[32mFile edited successfully.\n\033[0m");

    return SUCCESS;
}