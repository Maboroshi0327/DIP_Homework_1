#ifndef MY_IMAGE_CPP
#define MY_IMAGE_CPP

#include "My_Image.h"

template <typename type, size_t x_size, size_t y_size>
My_Image::My_Image(type(&R)[x_size][y_size], type(&G)[x_size][y_size], type(&B)[x_size][y_size], int w, int h)
{
    // 儲存 width height
    width_origin = w;
    height_origin = h;

    // 分配記憶體給 R G B
    R_origin = new int* [width_origin];
    G_origin = new int* [width_origin];
    B_origin = new int* [width_origin];
    for (int i = 0; i < width_origin; i++)
    {
        R_origin[i] = new int[height_origin];
        G_origin[i] = new int[height_origin];
        B_origin[i] = new int[height_origin];
    }

    // 儲存傳入的圖片
    for (int i = 0; i < width_origin; i++)
    {
        for (int j = 0; j < height_origin; j++)
        {
            R_origin[i][j] = (int)R[i][j];
            G_origin[i][j] = (int)G[i][j];
            B_origin[i][j] = (int)B[i][j];
        }
    }
}

My_Image::~My_Image()
{
    // 釋放 R G B 的記憶體
    for (int i = 0; i < width_origin; i++)
    {
        delete[] R_origin[i];
        delete[] G_origin[i];
        delete[] B_origin[i];
    }
    delete[] R_origin;
    delete[] G_origin;
    delete[] B_origin;
    delete_output_mem();
}

template<typename type, size_t x_size, size_t y_size>
void My_Image::change_image(type(&R)[x_size][y_size], type(&G)[x_size][y_size], type(&B)[x_size][y_size], int w, int h)
{
    delete_origin_mem();

    // 儲存 width height
    width_origin = w;
    height_origin = h;

    // 分配記憶體給 R G B
    R_origin = new int* [width_origin];
    G_origin = new int* [width_origin];
    B_origin = new int* [width_origin];
    for (int i = 0; i < width_origin; i++)
    {
        R_origin[i] = new int[height_origin];
        G_origin[i] = new int[height_origin];
        B_origin[i] = new int[height_origin];
    }

    // 儲存傳入的圖片
    for (int i = 0; i < width_origin; i++)
    {
        for (int j = 0; j < height_origin; j++)
        {
            R_origin[i][j] = (int)R[i][j];
            G_origin[i][j] = (int)G[i][j];
            B_origin[i][j] = (int)B[i][j];
        }
    }
}

template <typename type, size_t x_size, size_t y_size>
void My_Image::To_array(type(&R)[x_size][y_size], type(&G)[x_size][y_size], type(&B)[x_size][y_size])
{
    // 輸出圖片
    for (int i = 0; i < width_output; i++)
    {
        for (int j = 0; j < height_output; j++)
        {
            R[i][j] = R_output[i][j];
            G[i][j] = G_output[i][j];
            B[i][j] = B_output[i][j];
        }
    }
}

int My_Image::Median_Filtering(int window_size)
{
    // 分配輸出圖片的記憶體
    delete_output_mem();
    width_output = width_origin;
    height_output = height_origin;
    new_output_mem();

    // Median_Filtering
    for (int i = 0; i < width_origin; i++)
    {
        for (int j = 0; j < height_origin; j++)
        {
            int median[3];
            int flag = find_median(i, j, window_size, median);
            if (flag == 1)
            {
                cout << "Median_Filtering: window_size must be an odd integer." << endl;
                return 1;
            }
            R_output[i][j] = median[0];
            G_output[i][j] = median[1];
            B_output[i][j] = median[2];
        }
    }
    return 0;
}

int My_Image::Adaptive_Median_Filtering()
{
    // 分配輸出圖片的記憶體
    delete_output_mem();
    width_output = width_origin;
    height_output = height_origin;
    new_output_mem();

    // Adaptive_Median_Filtering
    for (int i = 0; i < width_origin; i++)
    {
        for (int j = 0; j < height_origin; j++)
        {
            int output[3];
            int flag = find_AMF_output(i, j, output);
            if (flag == 1)
            {
                cout << "Adaptive_Median_Filtering: window_size must be an odd integer." << endl;
                return 1;
            }
            R_output[i][j] = output[0];
            G_output[i][j] = output[1];
            B_output[i][j] = output[2];
        }
    }
    return 0;
}

void My_Image::Perspective_Transformation()
{
    // 分配輸出圖片的記憶體
    delete_output_mem();
    width_output = width_origin;
    height_output = height_origin;
    new_output_mem();

    int x, y;
    for (int i = 0; i < width_output; i++)
    {
        for (int j = 0; j < height_output; j++)
        {
            find_PT_xy(i, j, x, y);
            if (0 <= x && x < width_origin && 0 <= y && y < height_origin)
            {
                R_output[i][j] = R_origin[x][y];
                G_output[i][j] = G_origin[x][y];
                B_output[i][j] = B_origin[x][y];
            }
            else
            {
                R_output[i][j] = 0;
                G_output[i][j] = 0;
                B_output[i][j] = 0;
            }
        }
    }
}

int My_Image::Canny(int Gaussian_kernel_size, double sigma, int tH, int tL)
{
    if (Gaussian_kernel_size % 2 == 0)
    {
        cout << "Canny: Gaussian_kernel_size % 2 == 0" << endl;
        return 1;
    }

    // 分配輸出圖片的記憶體
    delete_output_mem();
    width_output = width_origin;
    height_output = height_origin;
    new_output_mem();

    // Color to Gray
    vector<vector<double>> Gray(width_origin, vector<double>(height_origin, 0));
    color_to_gray(Gray);

    // Gaussian Filtering
    vector<vector<double>> GF_result(width_origin, vector<double>(height_origin, 0));
    Gaussian_Filtering(Gaussian_kernel_size, sigma, Gray, GF_result);

    // Sobel
    vector<vector<double>> gradient(width_origin, vector<double>(height_origin, 0));
    vector<vector<int>> direction(width_origin, vector<int>(height_origin, 0));
    Sobel(GF_result, gradient, direction);

    // Non-maximum suppression
    vector<vector<double>> suppression(width_origin, vector<double>(height_origin, 0));
    Non_maximum_suppression(5, gradient, direction, suppression);

    // Double Threshold
    vector<vector<int>> binarization(width_origin, vector<int>(height_origin, 0));
    Double_Threshold(1, tL, tH, suppression, binarization);
    
    //Direction_visualization(gradient, direction);
    
    // output
    for (int i = 0; i < width_origin; i++)
    {
        for (int j = 0; j < height_origin; j++)
        {
            R_output[i][j] = G_output[i][j] = B_output[i][j] = binarization[i][j];
        }
    }

    return 0;
}

int My_Image::delete_origin_mem()
{
    // 釋放 R G B 的記憶體
    if (R_origin == NULL || G_origin == NULL || B_origin == NULL)
    {
        //cout << "delete_origin_mem: origin pointer is NULL." << endl;
        return 1;
    }
    for (int i = 0; i < width_origin; i++)
    {
        delete[] R_origin[i];
        delete[] G_origin[i];
        delete[] B_origin[i];
    }
    delete[] R_origin;
    delete[] G_origin;
    delete[] B_origin;
    R_origin = NULL;
    G_origin = NULL;
    B_origin = NULL;

    return 0;
}

int My_Image::new_output_mem()
{
    // 分配記憶體給 R G B
    if (R_output != NULL || G_output != NULL || B_output != NULL)
    {
        //cout << "new_output_mem: output pointer is not NULL." << endl;
        return 1;
    }
    R_output = new int* [width_output];
    G_output = new int* [width_output];
    B_output = new int* [width_output];
    for (int i = 0; i < width_output; i++)
    {
        R_output[i] = new int[height_output];
        G_output[i] = new int[height_output];
        B_output[i] = new int[height_output];
    }
    
    return 0;
}

int My_Image::delete_output_mem()
{
    // 釋放 R G B 的記憶體
    if (R_output == NULL || G_output == NULL || B_output == NULL)
    {
        //cout << "delete_output_mem: output pointer is NULL." << endl;
        return 1;
    }
    for (int i = 0; i < width_output; i++)
    {
        delete[] R_output[i];
        delete[] G_output[i];
        delete[] B_output[i];
    }
    delete[] R_output;
    delete[] G_output;
    delete[] B_output;
    R_output = NULL;
    G_output = NULL;
    B_output = NULL;

    return 0;
}

int My_Image::find_median(int x, int y, int window_size, int median[3])
{
    if (window_size % 2 == 0)
    {
        cout << "find_median: window_size must be an odd integer." << endl;
        return 1;
    }
    vector<int> mask_R(window_size * window_size);
    vector<int> mask_G(window_size * window_size);
    vector<int> mask_B(window_size * window_size);
    for (int i = 0; i < window_size; i++)
    {
        for (int j = 0; j < window_size; j++)
        {
            int posision_x = x - (window_size / 2) + i;
            int position_y = y - (window_size / 2) + j;
            if (posision_x >= 0 && posision_x < width_origin && position_y >= 0 && position_y < height_origin)
            {
                mask_R[i * window_size + j] = R_origin[posision_x][position_y];
                mask_G[i * window_size + j] = G_origin[posision_x][position_y];
                mask_B[i * window_size + j] = B_origin[posision_x][position_y];
            }
            else
            {
                mask_R[i * window_size + j] = R_origin[x][y];
                mask_G[i * window_size + j] = G_origin[x][y];
                mask_B[i * window_size + j] = B_origin[x][y];
            }
        }
    }
    sort(mask_R.begin(), mask_R.end());
    sort(mask_G.begin(), mask_G.end());
    sort(mask_B.begin(), mask_B.end());
    median[0] = mask_R[window_size * window_size / 2];
    median[1] = mask_G[window_size * window_size / 2];
    median[2] = mask_B[window_size * window_size / 2];

    return 0;
}

int My_Image::find_median(int x, int y, int window_size, int median[3], int max[3], int min[3])
{
    if (window_size % 2 == 0)
    {
        cout << "find_median: window_size must be an odd integer." << endl;
        return 1;
    }
    vector<int> mask_R(window_size * window_size);
    vector<int> mask_G(window_size * window_size);
    vector<int> mask_B(window_size * window_size);
    for (int i = 0; i < window_size; i++)
    {
        for (int j = 0; j < window_size; j++)
        {
            int posision_x = x - (window_size / 2) + i;
            int position_y = y - (window_size / 2) + j;
            if (posision_x >= 0 && posision_x < width_origin && position_y >= 0 && position_y < height_origin)
            {
                mask_R[i * window_size + j] = R_origin[posision_x][position_y];
                mask_G[i * window_size + j] = G_origin[posision_x][position_y];
                mask_B[i * window_size + j] = B_origin[posision_x][position_y];
            }
            else
            {
                mask_R[i * window_size + j] = R_origin[x][y];
                mask_G[i * window_size + j] = G_origin[x][y];
                mask_B[i * window_size + j] = B_origin[x][y];
            }
        }
    }
    sort(mask_R.begin(), mask_R.end());
    sort(mask_G.begin(), mask_G.end());
    sort(mask_B.begin(), mask_B.end());
    median[0] = mask_R[window_size * window_size / 2];
    median[1] = mask_G[window_size * window_size / 2];
    median[2] = mask_B[window_size * window_size / 2];
    max[0] = mask_R.back();
    max[1] = mask_G.back();
    max[2] = mask_B.back();
    min[0] = mask_R.front();
    min[1] = mask_G.front();
    min[2] = mask_B.front();

    return 0;
}

int My_Image::find_median_one_channel(int x, int y, int window_size, int channel, int& median, int& max, int& min)
{
    if (window_size % 2 == 0)
    {
        cout << "find_median_one_channel: window_size must be an odd integer." << endl;
        return 1;
    }
    vector<int> mask(window_size * window_size);
    for (int i = 0; i < window_size; i++)
    {
        for (int j = 0; j < window_size; j++)
        {
            int posision_x = x - (window_size / 2) + i;
            int position_y = y - (window_size / 2) + j;
            if (posision_x >= 0 && posision_x < width_origin && position_y >= 0 && position_y < height_origin)
            {
                switch (channel)
                {
                // R channel
                case 0:
                    mask[i * window_size + j] = R_origin[posision_x][position_y];
                    break;

                // G channel
                case 1:
                    mask[i * window_size + j] = G_origin[posision_x][position_y];
                    break;

                // B channel
                case 2:
                    mask[i * window_size + j] = B_origin[posision_x][position_y];
                    break;
                }
            }
            else
            {
                switch (channel)
                {
                // R channel
                case 0:
                    mask[i * window_size + j] = R_origin[x][y];
                    break;

                // G channel
                case 1:
                    mask[i * window_size + j] = G_origin[x][y];
                    break;

                // B channel
                case 2:
                    mask[i * window_size + j] = B_origin[x][y];
                    break;
                }
            }
        }
    }
    sort(mask.begin(), mask.end());
    median = mask[window_size * window_size / 2];
    max = mask.back();
    min = mask.front();

    return 0;
}

int My_Image::find_AMF_output(int x, int y, int output[3])
{
    int Wmax = 11;
    int window_size = 3;
    int median, max, min;
    int pixel_value[3] = { R_origin[x][y], G_origin[x][y], B_origin[x][y] };

    for (int channel = 0; channel < 3; channel++)
    {
#pragma region Stage A
        while (1)
        {
            int flag = find_median_one_channel(x, y, window_size, channel, median, max, min);
            if (flag == 1)
            {
                cout << "find_AMF_output: window_size must be an odd integer." << endl;
                return 1;
            }
            if (min < median && median < max)
            {
#pragma region Stage B
                if (min < pixel_value[channel] && pixel_value[channel] < max)
                {
                    output[channel] = pixel_value[channel];
                    break;
                }
                else
                {
                    output[channel] = median;
                    break;
                }
#pragma endregion Stage B
            }
            else
            {
                window_size = window_size + 2;
                if (window_size <= Wmax)
                {
                    continue;
                }
                else
                {
                    output[channel] = median;
                    break;
                }
            }
        }
#pragma endregion Stage A
    }

    return 0;
}

void My_Image::find_PT_xy(int u, int v, int& x, int& y)
{
    // 建立 Homography Matrix
    double a = 0.6158, b = 0.3301, c = 0, d = -0.0333, e = 0.8827, f = 30.0000, g = -0.0007, h = 0.0006;
    double hm[3][3] =
    {
        {a - g * u, d - g * v, 0},
        {b - h * u, e - h * v, 0},
        {c, f, 1}
    };
    mat H(&hm[0][0], 3, 3);
    mat output_xy({ (double)u, (double)v, 1 });

    // 求對映原圖的座標
    mat original_xy = H.i() * output_xy.t();
    x = (int)original_xy(0);
    y = (int)original_xy(1);
}

void My_Image::color_to_gray(vector<vector<double>>& input)
{
    for (int i = 0; i < width_origin; i++)
    {
        for (int j = 0; j < height_origin; j++)
        {
            input[i][j] = R_origin[i][j] * 0.299 + G_origin[i][j] * 0.587 + B_origin[i][j] * 0.114;
        }
    }
}

void My_Image::make_GF_kernel(vector<vector<double>>& Gaussian_kernel, int Gaussian_kernel_size, double sigma)
{
    double sum = 0;
    int half_kernel_size = Gaussian_kernel_size / 2;
    for (int i = -half_kernel_size; i <= half_kernel_size; i++)
    {
        for (int j = -half_kernel_size; j <= half_kernel_size; j++)
        {
            double power = -(i * i + j * j) / (2 * sigma * sigma);
            Gaussian_kernel[i + half_kernel_size][j + half_kernel_size] = exp(power);
            sum += Gaussian_kernel[i + half_kernel_size][j + half_kernel_size];
        }
    }
    for (int i = 0; i < Gaussian_kernel_size; i++)
    {
        for (int j = 0; j < Gaussian_kernel_size; j++)
        {
            Gaussian_kernel[i][j] /= sum;
        }
    }
}

int My_Image::Gaussian_Filtering(int Gaussian_kernel_size, double sigma, vector<vector<double>>& input, vector<vector<double>>& result)
{
    if (Gaussian_kernel_size % 2 == 0)
    {
        cout << "Gaussian_Filtering: Gaussian_kernel_size must be an odd integer." << endl;
        return 1;
    }
    vector<vector<double>> Gaussian_kernel(Gaussian_kernel_size, vector<double>(Gaussian_kernel_size, 0));
    make_GF_kernel(Gaussian_kernel, Gaussian_kernel_size, sigma);
    window_filtering(Gaussian_kernel, input, result);

    return 0;
}

void My_Image::Sobel(vector<vector<double>>& input, vector<vector<double>>& gradient, vector<vector<int>>& direction)
{
    // All gradient = 0
    for (auto& i : gradient)
        for (auto& j : i)
            j = 0;

    vector<vector<double>> Sobel_x(3, vector<double>(3, 0));
    vector<vector<double>> Sobel_y(3, vector<double>(3, 0));
    Sobel_x[0][2] = -1; Sobel_x[1][2] = 0; Sobel_x[2][2] = 1;
    Sobel_x[0][1] = -2; Sobel_x[1][1] = 0; Sobel_x[2][1] = 2;
    Sobel_x[0][0] = -1; Sobel_x[1][0] = 0; Sobel_x[2][0] = 1;
    Sobel_y[0][2] =  1; Sobel_y[1][2] =  2; Sobel_y[2][2] =  1;
    Sobel_y[0][1] =  0; Sobel_y[1][1] =  0; Sobel_y[2][1] =  0;
    Sobel_y[0][0] = -1; Sobel_y[1][0] = -2; Sobel_y[2][0] = -1;

    int width = (int)input.size();
    int height = (int)input[0].size();
    for (int i = 1; i < width - 1; i++)
    {
        for (int j = 1; j < height - 1; j++)
        {
            double Gx{}, Gy{}, G, angle;
            find_WF(Sobel_x, input, i, j, Gx);
            find_WF(Sobel_y, input, i, j, Gy);
            G = sqrt(Gx * Gx + Gy * Gy);
            angle = atan(Gy / Gx) * 180 / 3.14159265;
            angle = angle < 0 ? angle + 180 : angle;

            gradient[i][j] = G;
            if ((0 <= angle && angle < 22.5) || (157.5 <= angle))
                direction[i][j] = 0;
            else if (22.5 <= angle && angle < 67.5)
                direction[i][j] = 1;
            else if (67.5 <= angle && angle < 112.5)
                direction[i][j] = 2;
            else if (112.5 <= angle && angle < 157.5)
                direction[i][j] = 3;
        }
    }
}

void My_Image::Non_maximum_suppression(int kernel_size, vector<vector<double>>& gradient, vector<vector<int>>& direction, vector<vector<double>>& result)
{
    int half_kernel_size = kernel_size / 2;

    int width = (int)gradient.size();
    int height = (int)gradient[0].size();
    for (int i = 0; i < width; i++)
    {
        result[i].assign(gradient[i].begin(), gradient[i].end());
    }

    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            switch (direction[i][j])
            {
            case 0:
                for (int k = -half_kernel_size; k < half_kernel_size; k++)
                {
                    if (0 <= i + k && i + k < width)
                    {
                        if (k != 0 && result[i][j] < result[i + k][j])
                        {
                            result[i][j] = 0;
                            break;
                        }
                    }
                }
                break;

            case 1:
                for (int k = -half_kernel_size; k < half_kernel_size; k++)
                {
                    if (0 <= i + k && i + k < width && 0 <= j + k && j + k < height)
                    {
                        if (k != 0 && result[i][j] < result[i + k][j + k])
                        {
                            result[i][j] = 0;
                            break;
                        }
                    }
                }
                break;

            case 2:
                for (int k = -half_kernel_size; k < half_kernel_size; k++)
                {
                    if (0 <= j + k && j + k < height)
                    {
                        if (k != 0 && result[i][j] < result[i][j + k])
                        {
                            result[i][j] = 0;
                            break;
                        }
                    }
                }
                break;

            case 3:
                for (int k = -half_kernel_size; k < half_kernel_size; k++)
                {
                    if (0 <= i + k && i + k < width && 0 <= j - k && j - k < height)
                    {
                        if (k != 0 && result[i][j] < result[i + k][j - k])
                        {
                            result[i][j] = 0;
                            break;
                        }
                    }
                }
                break;
            }
        }
    }
}

void My_Image::Double_Threshold(int scan_times, int tL, int tH, vector<vector<double>>& input, vector<vector<int>>& result)
{
    int width = (int)input.size();
    int height = (int)input[0].size();
    for (int i = 1; i < width - 1; i++)
    {
        for (int j = 1; j < height - 1; j++)
        {
            if (input[i][j] < tL)
            {
                result[i][j] = 0;
            }
            else if (tL < input[i][j] && input[i][j] < tH)
            {
                result[i][j] = 1;
            }
            else
            {
                result[i][j] = 2;
            }
        }
    }
    for (int scan = 0; scan < scan_times; scan++)
    {
        for (int i = 1; i < width - 1; i++)
        {
            for (int j = 1; j < height - 1; j++)
            {
                if (result[i][j] == 1)
                {
                    for (int dx = -1; dx <= 1; dx++)
                        for (int dy = -1; dy <= 1; dy++)
                            if (result[i + dx][j + dy] == 2)
                            {
                                result[i][j] = 2;
                                dx = 2;
                                dy = 2;
                            }

                }
            }
        }
    }
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            if (result[i][j] == 2)
                result[i][j] = 255;
            else
                result[i][j] = 0;
        }
    }
}

void My_Image::Direction_visualization(vector<vector<double>>& gradient, vector<vector<int>>& direction)
{
    for (int i = 0; i < width_output; i++)
    {
        for (int j = 0; j < height_output; j++)
        {
            if (gradient[i][j] > 50)
            {
                if (direction[i][j] == 0)
                {
                    R_output[i][j] = 255;
                    G_output[i][j] = 0;
                    B_output[i][j] = 0;
                }
                else if (direction[i][j] == 1)
                {
                    R_output[i][j] = 0;
                    G_output[i][j] = 255;
                    B_output[i][j] = 0;
                }
                else if (direction[i][j] == 2)
                {
                    R_output[i][j] = 0;
                    G_output[i][j] = 0;
                    B_output[i][j] = 255;
                }
                else if (direction[i][j] == 3)
                {
                    R_output[i][j] = 255;
                    G_output[i][j] = 255;
                    B_output[i][j] = 0;
                }
            }
            else
            {
                R_output[i][j] = 0;
                G_output[i][j] = 0;
                B_output[i][j] = 0;
            }
        }
    }
}

void My_Image::window_filtering(vector<vector<double>>& window, vector<vector<double>>& input, vector<vector<double>>& result)
{
    int width = (int)result.size();
    int height = (int)result[0].size();
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            double value{};
            find_WF(window, input, i, j, value);
            result[i][j] = value;
        }
    }
}

void My_Image::window_filtering(vector<vector<double>>& window, vector<vector<vector<double>>>& result)
{
    int width = (int)result[0].size();
    int height = (int)result[0][0].size();
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            double value[3];
            find_WF(window, i, j, value);
            result[0][i][j] = value[0];
            result[1][i][j] = value[1];
            result[2][i][j] = value[2];
        }
    }
}

void My_Image::find_WF(vector<vector<double>>& window, int x, int y, double value[3])
{
    int window_size = (int)window.size();
    value[0] = value[1] = value[2] = 0;
    for (int i = 0; i < window_size; i++)
    {
        for (int j = 0; j < window_size; j++)
        {
            int posision_x = x - (window_size / 2) + i;
            int position_y = y - (window_size / 2) + j;
            if (posision_x >= 0 && posision_x < width_origin && position_y >= 0 && position_y < height_origin)
            {
                value[0] += window[i][j] * R_origin[posision_x][position_y];
                value[1] += window[i][j] * G_origin[posision_x][position_y];
                value[2] += window[i][j] * B_origin[posision_x][position_y];
            }
            else
            {
                value[0] += window[i][j] * R_origin[x][y];
                value[1] += window[i][j] * G_origin[x][y];
                value[2] += window[i][j] * B_origin[x][y];
            }
        }
    }
}

void My_Image::find_WF(vector<vector<double>>& window, vector<vector<double>>& input, int x, int y, double& value)
{
    int window_size = (int)window.size();
    int width = (int)input.size();
    int height = (int)input[0].size();
    value = 0;
    for (int i = 0; i < window_size; i++)
    {
        for (int j = 0; j < window_size; j++)
        {
            int posision_x = x - (window_size / 2) + i;
            int position_y = y - (window_size / 2) + j;
            if (posision_x >= 0 && posision_x < width && position_y >= 0 && position_y < height)
            {
                value += window[i][j] * input[posision_x][position_y];
            }
            else
            {
                value += window[i][j] * input[x][y];
            }
        }
    }
}

void My_Image::find_WF(vector<vector<double>>& window, vector<vector<vector<double>>>& input, int x, int y, double value[3])
{
    int window_size = (int)window.size();
    int width = (int)input[0].size();
    int height = (int)input[0][0].size();
    value[0] = value[1] = value[2] = 0;
    for (int i = 0; i < window_size; i++)
    {
        for (int j = 0; j < window_size; j++)
        {
            int posision_x = x - (window_size / 2) + i;
            int position_y = y - (window_size / 2) + j;
            if (posision_x >= 0 && posision_x < width && position_y >= 0 && position_y < height)
            {
                value[0] += window[i][j] * input[0][posision_x][position_y];
                value[1] += window[i][j] * input[1][posision_x][position_y];
                value[2] += window[i][j] * input[2][posision_x][position_y];
            }
            else
            {
                value[0] += window[i][j] * input[0][x][y];
                value[1] += window[i][j] * input[1][x][y];
                value[2] += window[i][j] * input[2][x][y];
            }
        }
    }
}

#endif /* MY_IMAGE_CPP */