#ifndef MY_IMAGE
#define MY_IMAGE

#include <cstddef>
#include <iostream>
#include <vector>
#include <algorithm>
#include <armadillo>

using namespace std;
using namespace arma;

class My_Image
{
public:
    template <typename type, size_t x_size, size_t y_size>
    My_Image(type(&R)[x_size][y_size], type(&G)[x_size][y_size], type(&B)[x_size][y_size], int w, int h);

    ~My_Image();

    int** R_origin, ** G_origin, ** B_origin;
    int width_origin, height_origin;

    int** R_output{}, ** G_output{}, ** B_output{};
    int width_output{}, height_output{};

    template <typename type, size_t x_size, size_t y_size>
    void change_image(type(&R)[x_size][y_size], type(&G)[x_size][y_size], type(&B)[x_size][y_size], int w, int h);

    template <typename type, size_t x_size, size_t y_size>
    void To_array(type(&R)[x_size][y_size], type(&G)[x_size][y_size], type(&B)[x_size][y_size]);

    int Median_Filtering(int window_size);

    int Adaptive_Median_Filtering();

    void Perspective_Transformation();

private:
    int delete_origin_mem();
    int new_output_mem();
    int delete_output_mem();
    int find_median(int x, int y, int window_size, int median[3]);
    int find_median(int x, int y, int window_size, int median[3], int max[3], int min[3]);
    int find_median_one_channel(int x, int y, int window_size, int channel, int& median, int& max, int& min);
    int find_AMF_output(int x, int y, int output[3]);
    void find_PT_xy(int u, int v, int& x, int& y);
};

#include "My_Image.cpp"

#endif /* MY_IMAGE */