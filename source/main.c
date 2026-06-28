#include <stdio.h>
#include <3ds.h>
#include "functions.h"
#include "template.h"
#include <dirent.h>
#include <unistd.h>

// colors
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

    // check if text file exists
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

    printf("Hackertyper3DS (SEL:color L+U/D:speed)\n\n");

    while (aptMainLoop()) {
        hidScanInput(); // scans for key pressings
        u32 kDown = hidKeysDown(); // gets keys that are down
        u32 kHeld = hidKeysHeld(); // gets keys that are held

        // key input handling
        if (kHeld & KEY_L) { // checks if L is held
            if (kDown & KEY_DUP) { // checks if d pad up is pressed
                if (speed < 49) speed += 1; // increase speed
            }
            if (kDown & KEY_DDOWN) { // checks if d pad down is pressed
                if (speed > 1) speed -= 1; // decrease speed
            }
        } else if (kDown & KEY_START) { // checks if start is pressed
            break; // quit app
        } else if (kDown & KEY_SELECT) { // checks if select is pressed
            currentColor = (currentColor + 1) % 7; // prevents the current color from going over 6
            color = colors[currentColor]; // set color
            printf("\x1b[2J"); // clear screen
            printf("%s", color); // print color
            currentChar = 0; // reset current char to start of file
            printf("Hackertyper3DS (SEL:color L+U/D:speed)\n\n");
        } else if (kDown) {
            for (int i = 0; i < (int)speed; i++) { // repeats for speed amount of times
                if (!text[currentChar]) currentChar = 0; // checks if file already reached end and loops back
                printf("%c", text[currentChar]); // prints char
                if (text[currentChar] == ' ') {
                    i--; // skips over space
                }
                currentChar++; // counts up current char
            }
        }

        // graphics stuff
        gfxFlushBuffers();
        gfxSwapBuffers();
        gspWaitForVBlank();
    }

    // exits
    gfxExit();
    return 0;
}
