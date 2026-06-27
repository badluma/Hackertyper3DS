#include <stdio.h>
#include <3ds.h>
#include "functions.h"
#include "template.h"
#include <dirent.h>
#include <unistd.h>

// the colors
char *colors[] = {
    "\x1b[32m", // Green
    "\x1b[31m", // Red
    "\x1b[34m", // Blue
    "\x1b[33m", // Yellow
    "\x1b[36m", // Cyan
    "\x1b[35m", // Magenta
    "\x1b[37m", // White
};

// default values
int speed = 5;
int color = 0;

int main(int argc, char **argv) {

    // check if directory exists
    DIR *dir = opendir("sdmc:/3ds/Hackertyper3DS");
    if (dir) {
        closedir(dir);
    } else {
        mkdir("sdmc:/3ds/Hackertyper3DS", 0777);
    }

    // check if config file exists
    if (access("sdmc:/3ds/Hackertyper3DS/text.txt", F_OK) != 0) {
        FILE *fptr = fopen("sdmc:/3ds/Hackertyper3DS/text.txt", "w");
        if (fptr) {
            fprintf(fptr, "%s", TEMPLATE);
            fclose(fptr);
        }
    }

    // inits variables
    char *text = readFile("sdmc:/3ds/Hackertyper3DS/text.txt");
    char *color = colors[0];
    u32 currentChar = 0;
    u8 currentColor = 0;

    // inits console
    gfxInitDefault();
    consoleInit(GFX_TOP, NULL);

    while (aptMainLoop()) {
        hidScanInput(); // scans for key pressings
        u32 kDown = hidKeysDown();

        // key input handling
        if (kDown & KEY_L) {
            if (kDown & KEY_DUP) {
                speed = (speed + 1) % 20; // prevents speed from going above 19
            }
            if (kDown & KEY_DDOWN) {
                speed = (speed - 1 + 20) % 20; // prevents speed from going below 0
            }
        } else if (kDown & KEY_START) {
            break; // quit app
        } else if (kDown & KEY_SELECT) {
            currentColor = (currentColor + 1) % 16; // prevents the current color from going over 15
            color = colors[currentColor];
        } else {
            printf("%s", color); // prints color
            for (int i = 0; i < (int)speed; i++) {
                if (!text[currentChar]) currentChar = 0; // loop
                printf("%c", text[currentChar]); // prints char
                currentChar++; // counts up current char
            }
        }

        gfxFlushBuffers();
        gfxSwapBuffers();
        gspWaitForVBlank();
    }

    // exits
    gfxExit();
    return 0;
}
