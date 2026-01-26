#include <stdio.h>
#include "bmp_loader.h"

int main(void)
{
    const char* file_name = "./images/test_stan.bmp";
    if (BmpLoadRgb(file_name) != 0)
    {
        printf("BMP load failed\n");
        return -1;
    }

    char* path = "C:/Users/23dej/Desktop/fakultet/Cetvrta godina/SDOS/ProjektniZadatak/src/image_rgb";
    if (ExportRgbToHeaderAndSource(path) != 0)
    {
        printf("Export failed\n");
        return -1;
    }

    printf("RGB header generated successfully\n");
    return 0;
}
