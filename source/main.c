#include <stdio.h>
#include <3ds.h>
#include "functions.h"
#include "template.h"
#include <dirent.h>
#include <unistd.h>

// ANSI color codes
char *colors[] = {
    "\x1b[32m", // Green
    "\x1b[31m", // Red
    "\x1b[34m", // Blue
    "\x1b[33m", // Yellow
    "\x1b[36m", // Cyan
    "\x1b[35m", // Magenta
    "\x1b[37m", // White
};

// Variables
int speed = 5;
int color = 0;

int main(int argc, char **argv) {

    // Check if directory exists
    DIR *dir = opendir("sdmc:/3ds/Hackertyper3DS");
    if (dir) {
        closedir(dir);
    } else {
        mkdir("sdmc:/3ds/Hackertyper3DS", 0777);
    }

    // Check if config file exists
    if (access("sdmc:/3ds/Hackertyper3DS/text.txt", F_OK) != 0) {
        FILE *fptr = fopen("sdmc:/3ds/Hackertyper3DS/text.txt", "w");
        fprintf(fptr, "%s", TEMPLATE);
        if (fptr) {
            fclose(fptr);
        }
    }

    char *text = readFile("sdmc:/3ds/Hackertyper3DS/text.txt");
    u32 speed = 1;
    char *color = colors[0];

    u32 currentChar = 0;
    u8 currentColor = 0;

    gfxInitDefault();
    consoleInit(GFX_TOP, NULL);

    while (aptMainLoop()) {
        hidScanInput();
        u32 kDown = hidKeysDown();

        if (kDown) {

            u32 key = kDown;

            if (keysDown() & KEY_L) {
                if (key == KEY_DUP) {
                    speed = (speed + 1) % 20;
                }
                if (key == KEY_DDOWN) {
                    speed = (speed - 1 + 20) % 20;
                }
            }

            switch (key) {
                case KEY_START:
                    break;
                case KEY_SELECT:
                    currentColor = (currentColor + 1) % 16;
                    color = colors[currentColor];
                    break;
                default:
                    printf("%s", color); // Applies color
                    for (int i = 0; i < (int)speed; i++) {

                        if (!text[currentChar]) currentChar = 0;
                        printf("%c", text[currentChar]); // Prints character
                        currentChar++;
                    }
            }
        }

        gfxFlushBuffers();
        gfxSwapBuffers();
        gspWaitForVBlank();
    }

    gfxExit();
    return 0;
}
