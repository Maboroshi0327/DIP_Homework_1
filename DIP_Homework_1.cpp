#include <iostream>
#include <stdlib.h>
#include "bmp.h"
#include "My_Image.h"

using namespace std;
using namespace arma;

int R[MaxBMPSizeX][MaxBMPSizeY];
int r[MaxBMPSizeX][MaxBMPSizeY];
int G[MaxBMPSizeX][MaxBMPSizeY];
int g[MaxBMPSizeX][MaxBMPSizeY];
int B[MaxBMPSizeX][MaxBMPSizeY];
int b[MaxBMPSizeX][MaxBMPSizeY];

int main(int argc, char* argv[])
{
    int width, height;

#pragma region Problem 1
    open_bmp((char*)"test images\\lena_pepper_and_salt_noise10%.bmp", R, G, B, width, height);

    My_Image image(R, G, B, width, height);
    image.Adaptive_Median_Filtering();
    image.To_array(r, g, b);

    save_bmp((char*)"test images\\Problem_1.bmp", r, g, b);
    cout << "Problem 1 Job Finished!" << endl;
    close_bmp();
#pragma endregion Problem 1

#pragma region Problem 2
    open_bmp((char*)"test images\\lena_std.bmp", R, G, B, width, height);

    image.change_image(R, G, B, width, height);
    image.Perspective_Transformation();
    image.To_array(r, g, b);

    save_bmp((char*)"test images\\Problem_2.bmp", r, g, b);
    cout << "Problem 2 Job Finished!" << endl;
    close_bmp();
#pragma endregion Problem 2

#pragma region Problem 3
    open_bmp((char*)"test images\\lighthouse.bmp", R, G, B, width, height);

    image.change_image(R, G, B, width, height);
    image.Canny(5, 0.1, 150, 80);
    image.To_array(r, g, b);

    save_bmp((char*)"test images\\Problem_3.bmp", r, g, b);
    cout << "Problem 3 Job Finished!" << endl;
    close_bmp();
#pragma endregion Problem 3

    system("PAUSE");
    return 0;
}
