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
    /** @brief 建構函式，傳入 R, G, B 2D array.
      * @param R, G, B: input image array
      * @param w: input image's width
      * @param h: input image's height
      * @return none
      */
    template <typename type, size_t x_size, size_t y_size>
    My_Image(type(&R)[x_size][y_size], type(&G)[x_size][y_size], type(&B)[x_size][y_size], int w, int h);

    /** @brief 解構函式，釋放 RGB_origin 和 RGB_output 的記憶體
      * @param none
      * @return none
      */
    ~My_Image();

    /** @brief 改變 R_origin, G_origin, B_origin 儲存的圖片
      * @param R, G, B: input image array
      * @param w: input image's width
      * @param h: input image's height
      * @return none
      */
    template <typename type, size_t x_size, size_t y_size>
    void change_image(type(&R)[x_size][y_size], type(&G)[x_size][y_size], type(&B)[x_size][y_size], int w, int h);

    /** @brief 輸出 R_output, G_output, B_output 至 R, G, B array
      * @param R, G, B: output image array
      * @return none
      */
    template <typename type, size_t x_size, size_t y_size>
    void To_array(type(&R)[x_size][y_size], type(&G)[x_size][y_size], type(&B)[x_size][y_size]);

    /** @brief 對 R_origin, G_origin, B_origin 做 Median Filtering
      * @param window_size: window 大小(window_size * window_size)
      * @return 傳入的 window_size 是否是奇數 (Yes:0, No:1)
      */
    int Median_Filtering(int window_size);

    /** @brief 對 R_origin, G_origin, B_origin 做 Adaptive_Median Filtering
      * @param none
      * @return 程式執行是否出錯
      */
    int Adaptive_Median_Filtering();

    /** @brief 對 R_origin, G_origin, B_origin 做 Perspective Transformation
      * @param none
      * @return none
      */
    void Perspective_Transformation();

    /** @brief 對 R_origin, G_origin, B_origin 做 Canny Edge detection
      * @param Gaussian_kernel_size: 決定 Gaussian kernel 的大小 (Gaussian_kernel_size * Gaussian_kernel_size)
      * @param sigma: 決定 Gaussian kernel 的 sigma
      * @param param tL, tH: 做 Double Threshold 時所需的參數
                             >tH : 一定是邊緣
                             <tL : 一定不是邊緣
                        >tL, <tH : 若附近有兩點 >tH 的點，則此點也視為邊緣
      * @return 傳入的 Gaussian_kernel_size 是否是奇數 (Yes:0, No:1)
      */
    int Canny(int Gaussian_kernel_size, double sigma, int tH, int tL);

private:
    int** R_origin, ** G_origin, ** B_origin;
    int width_origin, height_origin;

    int** R_output{}, ** G_output{}, ** B_output{};
    int width_output{}, height_output{};

    /** @brief 釋放暫存原始圖片的記憶體.
      * @param none
      * @return R_origin, G_origin, B_origin 是否指向NULL (Yes:1, No:0)
      */
    int delete_origin_mem();

    /** @brief 分配暫存輸出圖片的記憶體
      * @param none
      * @return R_output, G_output, B_output 是否指向NULL (Yes:0, No:1)
      */
    int new_output_mem();

    /** @brief 釋放暫存輸出圖片的記憶體
      * @param none
      * @return R_output, G_output, B_output 是否指向NULL (Yes:1, No:0)
      */
    int delete_output_mem();

    /** @brief 求 R_origin, G_origin, B_origin 在給定的 x, y 座標和 window 大小下的 median
      * @param x, y: window center 座標
      * @param window_size: window 大小(window_size * window_size)
      * @param median[3]: R, G, B channel 的 median value
      * @return 傳入的 window_size 是否是奇數 (Yes:0, No:1)
      */
    int find_median(int x, int y, int window_size, int median[3]);

    /** @brief 求 R_origin, G_origin, B_origin 在給定的 x, y 座標和 window 大小下的 median, max, min
      * @param x, y: window center 座標
      * @param window_size: window 大小(window_size * window_size)
      * @param median[3]: R, G, B channel 的 median value
      * @param max[3]: R, G, B channel 的 max value
      * @param min[3]: R, G, B channel 的 max value
      * @return 傳入的 window_size 是否是奇數 (Yes:0, No:1)
      */
    int find_median(int x, int y, int window_size, int median[3], int max[3], int min[3]);

    /** @brief 求給定的 channel, x, y 座標和 window 大小下的 median, max, min
      * @param x, y: window center 座標
      * @param window_size: window 大小(window_size * window_size)
      * @param channel: 選擇 R_origin, G_origin, B_origin (R_origin: 0, G_origin: 1, B_origin: 2)
      * @param median: 給定 channel 的 median value
      * @param max: 給定 channel 的 max value
      * @param min: 給定 channel 的 max value
      * @return 傳入的 window_size 是否是奇數 (Yes:0, No:1)
      */
    int find_median_one_channel(int x, int y, int window_size, int channel, int& median, int& max, int& min);

    /** @brief 求 R_origin, G_origin, B_origin 在給定的 x, y 座標下的 Adaptive Median Filtering value
      * @param x, y: Filtering center 座標
      * @param output[3]: R, G, B channel 的 Adaptive median value
      * @return 程式執行是否出錯
      */
    int find_AMF_output(int x, int y, int output[3]);

    /** @brief 做 Perspective_Transformation 時，以新圖的座標 u, v 找對映到原圖的座標 x, y
      * @param u, v: 新圖座標
      * @param x, y: 原圖座標
      * @return none
      */
    void find_PT_xy(int u, int v, int& x, int& y);

    /** @brief 原圖 R_origin, G_origin, B_origin 轉灰階
      * @param result: 儲存轉灰階後的值
      * @return none
      */
    void color_to_gray(vector<vector<double>>& result);

    /** @brief 生成 Gaussian kernel
      * @param Gaussian_kernel: 儲存生成的 Gaussian kernel
      * @param Gaussian_kernel_size: 決定 Gaussian kernel 的大小 (Gaussian_kernel_size * Gaussian_kernel_size)
      * @param sigma: 決定 Gaussian kernel 的 sigma
      * @return none
      */
    void make_GF_kernel(vector<vector<double>>& Gaussian_kernel, int Gaussian_kernel_size, double sigma);

    /** @brief Gaussian Filtering
      * @param Gaussian_kernel_size: 決定 Gaussian kernel 的大小 (Gaussian_kernel_size * Gaussian_kernel_size)
      * @param sigma: 決定 Gaussian kernel 的 sigma
      * @param input: 被濾波的 2D vector
      * @param result: 濾波後的結果
      * @return 傳入的 Gaussian_kernel_size 是否是奇數 (Yes:0, No:1)
      */
    int Gaussian_Filtering(int Gaussian_kernel_size, double sigma, vector<vector<double>>& input, vector<vector<double>>& result);

    /** @brief Sobel Edge detection
      * @param input: 輸入圖片
      * @param gradient: 輸出梯度
      * @param direction: 輸出方向 (0°: 0, 45°: 1, 90°: 2, 135°: 3)
      * @return none
      */
    void Sobel(vector<vector<double>>& input, vector<vector<double>>& gradient, vector<vector<int>>& direction);

    /** @brief Non maximum suppression
      * @param kernel_size: Non maximum suppression 的 mask 大小 (kernel_size * kernel_size)
      * @param gradient: 輸入梯度
      * @param direction: 輸入方向 (0°: 0, 45°: 1, 90°: 2, 135°: 3)
      * @param result: 輸出 suppress 後的梯度
      * @return none
      */
    void Non_maximum_suppression(int kernel_size, vector<vector<double>>& gradient, vector<vector<int>>& direction, vector<vector<double>>& result);

    /** @brief Double Threshold
      * @param scan_times: 掃描次數
      * @param tL, tH: >tH: 一定是邊緣
                       <tL: 一定不是邊緣
                  >tL, <tH: 若附近有兩點 >tH 的點，則此點也視為邊緣
      * @param input: 輸入梯度
      * @param result: 輸出的二值化影像
      * @return none
      */
    void Double_Threshold(int scan_times, int tL, int tH, vector<vector<double>>& input, vector<vector<int>>& result);

    /** @brief 方便查看圖片梯度方向
      * @param gradient: 輸入梯度
      * @param direction: 輸入方向 (0°: 0, 45°: 1, 90°: 2, 135°: 3)
      * @return none
      */
    void Direction_visualization(vector<vector<double>>& gradient, vector<vector<int>>& direction);

    /** @brief 對 input (2D vector) 做 convolution
      * @param kernel: 輸入 kernel
      * @param input: 被濾波的 2D vector
      * @param result: 濾波後的結果 (2D vector)
      * @return none
      */
    void convolution(vector<vector<double>>& kernel, vector<vector<double>>& input, vector<vector<double>>& result);

    /** @brief 對 R_origin, G_origin, B_origin 做 convolution
      * @param kernel: 輸入 kernel
      * @param result: 濾波後的結果 (3D vector)
      * @return none
      */
    void convolution(vector<vector<double>>& kernel, vector<vector<vector<double>>>& result);

    /** @brief 求 R_origin, G_origin, B_origin 在給定的 x, y 座標和 kernel 下的 conv 值
      * @param kernel: 輸入 kernel
      * @param x, y: kernel center 座標
      * @param value[3]: R, G, B 在 x, y 的 conv value
      * @return none
      */
    void find_CONV(vector<vector<double>>& kernel, int x, int y, double value[3]);

    /** @brief 求 input (2D vector) 在給定的 x, y 座標和 kernel 下的 conv 值
      * @param kernel: 輸入 kernel
      * @param input: 被濾波的 2D vector
      * @param x, y: kernel center 座標
      * @param value: input 在 x, y 的 conv value
      * @return none
      */
    void find_CONV(vector<vector<double>>& kernel, vector<vector<double>>& input, int x, int y, double& value);

    /** @brief 求 input (3D vector) 在給定的 x, y 座標和 kernel 下的 conv 值
      * @param kernel: 輸入 kernel
      * @param input: 被濾波的 3D vector (R, G, B)
      * @param x, y: kernel center 座標
      * @param value[3]: input 在 x, y 的 conv value (R, G, B)
      * @return none
      */
    void find_CONV(vector<vector<double>>& kernel, vector<vector<vector<double>>>& input, int x, int y, double value[3]);
};

#include "My_Image.cpp"

#endif /* MY_IMAGE */