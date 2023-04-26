#ifndef MY_IMAGE
#define MY_IMAGE

#include <cstddef>
#include <iostream>
#include <vector>
#include <algorithm>
#include <armadillo>
#include <cmath>

using namespace std;
using namespace arma;

class My_Image
{
public:
    template <typename type, size_t x_size, size_t y_size>
    My_Image(type(&R)[x_size][y_size], type(&G)[x_size][y_size], type(&B)[x_size][y_size], int w, int h);

    ~My_Image();

    template <typename type, size_t x_size, size_t y_size>
    void change_image(type(&R)[x_size][y_size], type(&G)[x_size][y_size], type(&B)[x_size][y_size], int w, int h);

    template <typename type, size_t x_size, size_t y_size>
    void To_array(type(&R)[x_size][y_size], type(&G)[x_size][y_size], type(&B)[x_size][y_size]);

    int Median_Filtering(int window_size);

    int Adaptive_Median_Filtering();

    void Perspective_Transformation();

    int Canny(int Gaussian_kernel_size, double sigma, int tH, int tL);

private:
    int** R_origin, ** G_origin, ** B_origin;
    int width_origin, height_origin;

    int** R_output{}, ** G_output{}, ** B_output{};
    int width_output{}, height_output{};

    int delete_origin_mem();
    int new_output_mem();
    int delete_output_mem();
    int find_median(int x, int y, int window_size, int median[3]);
    int find_median(int x, int y, int window_size, int median[3], int max[3], int min[3]);
    int find_median_one_channel(int x, int y, int window_size, int channel, int& median, int& max, int& min);
    int find_AMF_output(int x, int y, int output[3]);
    void find_PT_xy(int u, int v, int& x, int& y);
    void color_to_gray(vector<vector<double>>& input);
    void make_GF_kernel(vector<vector<double>>& Gaussian_kernel, int Gaussian_kernel_size, double sigma);
    int Gaussian_Filtering(int Gaussian_kernel_size, double sigma, vector<vector<double>>& input, vector<vector<double>>& result);
    void Sobel(vector<vector<double>>& input, vector<vector<double>>& gradient, vector<vector<int>>& direction);
    void Non_maximum_suppression(int kernel_size, vector<vector<double>>& gradient, vector<vector<int>>& direction, vector<vector<double>>& result);
    void Double_Threshold(int scan_times, int tL, int tH, vector<vector<double>>& input, vector<vector<int>>& result);
    void Direction_visualization(vector<vector<double>>& gradient, vector<vector<int>>& direction);

    // (1 channel)  input: 2d vector, output: 2d vector
    void window_filtering(vector<vector<double>>& window, vector<vector<double>>& input, vector<vector<double>>& result);

    // (3 channel)  input: RGB_origin, output: 3d vector
    void window_filtering(vector<vector<double>>& window, vector<vector<vector<double>>>& result);

    // (3 channel)  use: RGB_origin, output: 3 value for RGB
    void find_WF(vector<vector<double>>& window, int x, int y, double value[3]);

    // (1 channel)  use: input(2d vector), output: 1 value
    void find_WF(vector<vector<double>>& window, vector<vector<double>>& input, int x, int y, double& value);

    // (3 channel)  use: input(3d vector), output: 3 value for RGB
    void find_WF(vector<vector<double>>& window, vector<vector<vector<double>>>& input, int x, int y, double value[3]);
};

#include "My_Image.cpp"

#endif /* MY_IMAGE */