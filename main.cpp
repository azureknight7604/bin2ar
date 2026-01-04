/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++
  Author: Azure (@azureknight7604)
  Release Date: 04/JAN/2026

  bin2ar (Binary 2 Action Replay)
  -
  A simple program that converts binary (payload.bin)
  to Action Replay Cheat

  Tested on Windows 10

+++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#define _CRT_SECURE_NO_DEPRECATE
#include <iostream>

// Some emulators have a code limit per cheat, so we seperate
// longer code into multiple cheats to overcome the long code limitation.
// e.g. melonDS emulator has a 64 line code limit (doesn't accept code
// greater than 64 lines).
#define AR_MAXLINE_MELONDS      64
#define AR_MAXLINE_DESMUME      910


int main()
{
    /*-----[ You can changes these values here ]-----*/

    // The payload address
    unsigned int address = 0x02124868;
    /*---------------------------------------------------------
    Yu-Gi-Oh! 5D's World Championship 2011 - Over the Nexus
    0x02124960      USA
    0x0212487C      Europe
    0x02124868      Japan
    ---------------------------------------------------------*/

    // Should the cheat have a buttn activator ?
    bool button_activator = true;

    // Button to press to activate the cheat
    unsigned int button_press = 0xFFF70000;

    

    /*-----[ Do NOT modify values below here ]-----*/

    FILE* f_bin;
    FILE* f_txt;
    FILE* f_txt_melonds_android;
    FILE* f_txt_melonds;
    FILE* f_txt_desmume;

    f_bin = fopen("payload.bin", "rb");
    f_txt = fopen("output.txt", "wb+");
    f_txt_melonds_android = fopen("output_melonDS_android.txt", "wb+");
    f_txt_melonds = fopen("output_melonDS.txt", "wb+");
    f_txt_desmume = fopen("output_desmume.txt", "wb+");
    
    
    int file_seek_index = 3;
    int byte_count = 0;
    int lineCount_melonds = 0;//count to 64 lines (for melonDS)
    int lineCount_desmume = 0;//count to 910 lines (for desmume)
    unsigned char byte;

    // Start Button Activator
    if (button_activator) {
        fprintf(f_txt, "94000130 %08X\n", button_press);
        fprintf(f_txt_melonds_android, "94000130 %08X ", button_press);
        fprintf(f_txt_melonds, "94000130 %08X\n", button_press);
        fprintf(f_txt_desmume, "94000130 %08X\n", button_press);
        lineCount_melonds = 1;
        lineCount_desmume = 1;
        printf("94000130 %08X\n", button_press);
    }

    fprintf(f_txt, "%08X ", address);
    fprintf(f_txt_melonds_android, "%08X ", address);
    fprintf(f_txt_melonds, "%08X ", address);
    fprintf(f_txt_desmume, "%08X ", address);
    printf("%08X ", address);
    

    fseek(f_bin, 0L, SEEK_END);
    int max_payload_filesize = ftell(f_bin);
    while (1) {
        if (byte_count == 4) {
            file_seek_index += 8;
            lineCount_melonds++;
            lineCount_desmume++;

            if (file_seek_index >= max_payload_filesize)
                break;

            address += 4;
            fprintf(f_txt, "\n%08X ", address);


            // Seperate long code into multiple cheats for melonDS emulator
            if (lineCount_melonds == AR_MAXLINE_MELONDS - 1 && button_activator) {
                fprintf(f_txt_melonds_android, " D2000000 00000000\n\n");
                fprintf(f_txt_melonds_android, "94000130 %08X", button_press);
                fprintf(f_txt_melonds, "\nD2000000 00000000\n\n");
                fprintf(f_txt_melonds, "94000130 %08X", button_press);
                lineCount_melonds = 1;
            }
            if (lineCount_melonds == AR_MAXLINE_MELONDS) {
                fprintf(f_txt_melonds_android, "\n\n%08X ", address);
                fprintf(f_txt_melonds, "\n\n%08X ", address);
                lineCount_melonds = 0;
            }
            else {
                fprintf(f_txt_melonds_android, " %08X ", address);
                fprintf(f_txt_melonds, "\n%08X ", address);
            }


            // Seperate long code into multiple cheats for desmume emulator
            if (lineCount_desmume == AR_MAXLINE_DESMUME - 1 && button_activator) {
                fprintf(f_txt_desmume, "\nD2000000 00000000\n\n\n\n\n\n\n\n\n\n\n\n\n");
                fprintf(f_txt_desmume, "94000130 %08X", button_press);
                lineCount_desmume = 1;
            }
            if (lineCount_desmume == AR_MAXLINE_DESMUME) {
                fprintf(f_txt_desmume, "\n\n%08X ", address);
                lineCount_desmume = 0;
            }
            else {
                fprintf(f_txt_desmume, "\n%08X ", address);
            }
            

            printf("\n%08X ", address);
            byte_count = 0;
        }
        fseek(f_bin, file_seek_index, SEEK_SET);
        byte = fgetc(f_bin);
        fprintf(f_txt, "%02X", byte);
        fprintf(f_txt_melonds_android, "%02X", byte);
        fprintf(f_txt_melonds, "%02X", byte);
        fprintf(f_txt_desmume, "%02X", byte);
        printf("%02X", byte);
        byte_count++;
        file_seek_index--;
    }

    // End Button Activator
    if (button_activator) {
        fprintf(f_txt, "\nD2000000 00000000");
        fprintf(f_txt_melonds_android, " D2000000 00000000");
        fprintf(f_txt_melonds, "\nD2000000 00000000");
        fprintf(f_txt_desmume, "\nD2000000 00000000");
        printf("\nD2000000 00000000");
    }

    fclose(f_bin);
    fclose(f_txt);
    fclose(f_txt_melonds_android);
    fclose(f_txt_melonds);
    fclose(f_txt_desmume);
}