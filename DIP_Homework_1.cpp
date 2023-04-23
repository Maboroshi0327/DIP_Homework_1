#include <iostream>
#include <stdlib.h>
#include "bmp.h"
#include "My_Image.h"

using namespace std;

int R[MaxBMPSizeX][MaxBMPSizeY];
int r[MaxBMPSizeX][MaxBMPSizeY];
int G[MaxBMPSizeX][MaxBMPSizeY];
int g[MaxBMPSizeX][MaxBMPSizeY];
int B[MaxBMPSizeX][MaxBMPSizeY];
int b[MaxBMPSizeX][MaxBMPSizeY];

int main(int argc, char* argv[])
{
    int width, height;
    open_bmp((char*)"test images\\lena_std.bmp", R, G, B, width, height);

    My_Image image(R, R, R, width, height);
    image.Adaptive_Median_Filtering();
    image.To_array(r, g, b);

    save_bmp((char*)"test images\\lena_new.bmp", r, g, b);
    cout << "Job Finished!" << endl;

    close_bmp();

    system("PAUSE");
    return 0;
}
