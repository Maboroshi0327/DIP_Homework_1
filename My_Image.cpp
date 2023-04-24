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
                mask_R[i * window_size + j] = 255;
                mask_G[i * window_size + j] = 255;
                mask_B[i * window_size + j] = 255;
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
                mask_R[i * window_size + j] = 255;
                mask_G[i * window_size + j] = 255;
                mask_B[i * window_size + j] = 255;
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
                mask[i * window_size + j] = 255;
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

#endif /* MY_IMAGE_CPP */