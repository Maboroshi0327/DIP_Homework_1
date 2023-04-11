#include <iostream>
#include <stdlib.h>
#include "bmp.h"
#include "My_Image.h"

using namespace std;

int R[MaxBMPSizeX][MaxBMPSizeY];
int r[MaxBMPSizeX][MaxBMPSizeY];

int main(int argc, char* argv[])
{
    int width, height;
    open_bmp((char*)"lena_pepper_and_salt_noise10%.bmp", R, R, R, width, height);

    My_Image image(R, R, R, width, height);
    image.Adaptive_Median_Filtering();
    image.To_array(r, r, r);

    save_bmp((char*)"lena_new.bmp", r, r, r);
    cout << "Job Finished!" << endl;

    close_bmp();

    system("PAUSE");
    return 0;
}
