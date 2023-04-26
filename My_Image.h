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
    /** @brief �غc�禡�A�ǤJ R, G, B 2D array.
      * @param R, G, B: input image array
      * @param w: input image's width
      * @param h: input image's height
      * @return none
      */
    template <typename type, size_t x_size, size_t y_size>
    My_Image(type(&R)[x_size][y_size], type(&G)[x_size][y_size], type(&B)[x_size][y_size], int w, int h);

    /** @brief �Ѻc�禡�A���� RGB_origin �M RGB_output ���O����
      * @param none
      * @return none
      */
    ~My_Image();

    /** @brief ���� R_origin, G_origin, B_origin �x�s���Ϥ�
      * @param R, G, B: input image array
      * @param w: input image's width
      * @param h: input image's height
      * @return none
      */
    template <typename type, size_t x_size, size_t y_size>
    void change_image(type(&R)[x_size][y_size], type(&G)[x_size][y_size], type(&B)[x_size][y_size], int w, int h);

    /** @brief ��X R_output, G_output, B_output �� R, G, B array
      * @param R, G, B: output image array
      * @return none
      */
    template <typename type, size_t x_size, size_t y_size>
    void To_array(type(&R)[x_size][y_size], type(&G)[x_size][y_size], type(&B)[x_size][y_size]);

    /** @brief �� R_origin, G_origin, B_origin �� Median Filtering
      * @param window_size: window �j�p(window_size * window_size)
      * @return �ǤJ�� window_size �O�_�O�_�� (Yes:0, No:1)
      */
    int Median_Filtering(int window_size);

    /** @brief �� R_origin, G_origin, B_origin �� Adaptive_Median Filtering
      * @param none
      * @return �{������O�_�X��
      */
    int Adaptive_Median_Filtering();

    /** @brief �� R_origin, G_origin, B_origin �� Perspective Transformation
      * @param none
      * @return none
      */
    void Perspective_Transformation();

    /** @brief �� R_origin, G_origin, B_origin �� Canny Edge detection
      * @param Gaussian_kernel_size: �M�w Gaussian kernel ���j�p (Gaussian_kernel_size * Gaussian_kernel_size)
      * @param sigma: �M�w Gaussian kernel �� sigma
      * @param param tL, tH: �� Double Threshold �ɩһݪ��Ѽ�
                             >tH : �@�w�O��t
                             <tL : �@�w���O��t
                        >tL, <tH : �Y���񦳨��I >tH ���I�A�h���I�]������t
      * @return �ǤJ�� Gaussian_kernel_size �O�_�O�_�� (Yes:0, No:1)
      */
    int Canny(int Gaussian_kernel_size, double sigma, int tH, int tL);

private:
    int** R_origin, ** G_origin, ** B_origin;
    int width_origin, height_origin;

    int** R_output{}, ** G_output{}, ** B_output{};
    int width_output{}, height_output{};

    /** @brief ����Ȧs��l�Ϥ����O����.
      * @param none
      * @return R_origin, G_origin, B_origin �O�_���VNULL (Yes:1, No:0)
      */
    int delete_origin_mem();

    /** @brief ���t�Ȧs��X�Ϥ����O����
      * @param none
      * @return R_output, G_output, B_output �O�_���VNULL (Yes:0, No:1)
      */
    int new_output_mem();

    /** @brief ����Ȧs��X�Ϥ����O����
      * @param none
      * @return R_output, G_output, B_output �O�_���VNULL (Yes:1, No:0)
      */
    int delete_output_mem();

    /** @brief �D R_origin, G_origin, B_origin �b���w�� x, y �y�ЩM window �j�p�U�� median
      * @param x, y: window center �y��
      * @param window_size: window �j�p(window_size * window_size)
      * @param median[3]: R, G, B channel �� median value
      * @return �ǤJ�� window_size �O�_�O�_�� (Yes:0, No:1)
      */
    int find_median(int x, int y, int window_size, int median[3]);

    /** @brief �D R_origin, G_origin, B_origin �b���w�� x, y �y�ЩM window �j�p�U�� median, max, min
      * @param x, y: window center �y��
      * @param window_size: window �j�p(window_size * window_size)
      * @param median[3]: R, G, B channel �� median value
      * @param max[3]: R, G, B channel �� max value
      * @param min[3]: R, G, B channel �� max value
      * @return �ǤJ�� window_size �O�_�O�_�� (Yes:0, No:1)
      */
    int find_median(int x, int y, int window_size, int median[3], int max[3], int min[3]);

    /** @brief �D���w�� channel, x, y �y�ЩM window �j�p�U�� median, max, min
      * @param x, y: window center �y��
      * @param window_size: window �j�p(window_size * window_size)
      * @param channel: ��� R_origin, G_origin, B_origin (R_origin: 0, G_origin: 1, B_origin: 2)
      * @param median: ���w channel �� median value
      * @param max: ���w channel �� max value
      * @param min: ���w channel �� max value
      * @return �ǤJ�� window_size �O�_�O�_�� (Yes:0, No:1)
      */
    int find_median_one_channel(int x, int y, int window_size, int channel, int& median, int& max, int& min);

    /** @brief �D R_origin, G_origin, B_origin �b���w�� x, y �y�ФU�� Adaptive Median Filtering value
      * @param x, y: Filtering center �y��
      * @param output[3]: R, G, B channel �� Adaptive median value
      * @return �{������O�_�X��
      */
    int find_AMF_output(int x, int y, int output[3]);

    /** @brief �� Perspective_Transformation �ɡA�H�s�Ϫ��y�� u, v ���M���Ϫ��y�� x, y
      * @param u, v: �s�Ϯy��
      * @param x, y: ��Ϯy��
      * @return none
      */
    void find_PT_xy(int u, int v, int& x, int& y);

    /** @brief ��� R_origin, G_origin, B_origin ��Ƕ�
      * @param result: �x�s��Ƕ��᪺��
      * @return none
      */
    void color_to_gray(vector<vector<double>>& result);

    /** @brief �ͦ� Gaussian kernel
      * @param Gaussian_kernel: �x�s�ͦ��� Gaussian kernel
      * @param Gaussian_kernel_size: �M�w Gaussian kernel ���j�p (Gaussian_kernel_size * Gaussian_kernel_size)
      * @param sigma: �M�w Gaussian kernel �� sigma
      * @return none
      */
    void make_GF_kernel(vector<vector<double>>& Gaussian_kernel, int Gaussian_kernel_size, double sigma);

    /** @brief Gaussian Filtering
      * @param Gaussian_kernel_size: �M�w Gaussian kernel ���j�p (Gaussian_kernel_size * Gaussian_kernel_size)
      * @param sigma: �M�w Gaussian kernel �� sigma
      * @param input: �Q�o�i�� 2D vector
      * @param result: �o�i�᪺���G
      * @return �ǤJ�� Gaussian_kernel_size �O�_�O�_�� (Yes:0, No:1)
      */
    int Gaussian_Filtering(int Gaussian_kernel_size, double sigma, vector<vector<double>>& input, vector<vector<double>>& result);

    /** @brief Sobel Edge detection
      * @param input: ��J�Ϥ�
      * @param gradient: ��X���
      * @param direction: ��X��V (0�X: 0, 45�X: 1, 90�X: 2, 135�X: 3)
      * @return none
      */
    void Sobel(vector<vector<double>>& input, vector<vector<double>>& gradient, vector<vector<int>>& direction);

    /** @brief Non maximum suppression
      * @param kernel_size: Non maximum suppression �� mask �j�p (kernel_size * kernel_size)
      * @param gradient: ��J���
      * @param direction: ��J��V (0�X: 0, 45�X: 1, 90�X: 2, 135�X: 3)
      * @param result: ��X suppress �᪺���
      * @return none
      */
    void Non_maximum_suppression(int kernel_size, vector<vector<double>>& gradient, vector<vector<int>>& direction, vector<vector<double>>& result);

    /** @brief Double Threshold
      * @param scan_times: ���y����
      * @param tL, tH: >tH: �@�w�O��t
                       <tL: �@�w���O��t
                  >tL, <tH: �Y���񦳨��I >tH ���I�A�h���I�]������t
      * @param input: ��J���
      * @param result: ��X���G�ȤƼv��
      * @return none
      */
    void Double_Threshold(int scan_times, int tL, int tH, vector<vector<double>>& input, vector<vector<int>>& result);

    /** @brief ��K�d�ݹϤ���פ�V
      * @param gradient: ��J���
      * @param direction: ��J��V (0�X: 0, 45�X: 1, 90�X: 2, 135�X: 3)
      * @return none
      */
    void Direction_visualization(vector<vector<double>>& gradient, vector<vector<int>>& direction);

    /** @brief �� input (2D vector) �� convolution
      * @param kernel: ��J kernel
      * @param input: �Q�o�i�� 2D vector
      * @param result: �o�i�᪺���G (2D vector)
      * @return none
      */
    void convolution(vector<vector<double>>& kernel, vector<vector<double>>& input, vector<vector<double>>& result);

    /** @brief �� R_origin, G_origin, B_origin �� convolution
      * @param kernel: ��J kernel
      * @param result: �o�i�᪺���G (3D vector)
      * @return none
      */
    void convolution(vector<vector<double>>& kernel, vector<vector<vector<double>>>& result);

    /** @brief �D R_origin, G_origin, B_origin �b���w�� x, y �y�ЩM kernel �U�� conv ��
      * @param kernel: ��J kernel
      * @param x, y: kernel center �y��
      * @param value[3]: R, G, B �b x, y �� conv value
      * @return none
      */
    void find_CONV(vector<vector<double>>& kernel, int x, int y, double value[3]);

    /** @brief �D input (2D vector) �b���w�� x, y �y�ЩM kernel �U�� conv ��
      * @param kernel: ��J kernel
      * @param input: �Q�o�i�� 2D vector
      * @param x, y: kernel center �y��
      * @param value: input �b x, y �� conv value
      * @return none
      */
    void find_CONV(vector<vector<double>>& kernel, vector<vector<double>>& input, int x, int y, double& value);

    /** @brief �D input (3D vector) �b���w�� x, y �y�ЩM kernel �U�� conv ��
      * @param kernel: ��J kernel
      * @param input: �Q�o�i�� 3D vector (R, G, B)
      * @param x, y: kernel center �y��
      * @param value[3]: input �b x, y �� conv value (R, G, B)
      * @return none
      */
    void find_CONV(vector<vector<double>>& kernel, vector<vector<vector<double>>>& input, int x, int y, double value[3]);
};

#include "My_Image.cpp"

#endif /* MY_IMAGE */