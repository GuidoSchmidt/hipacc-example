#include <iostream>
#include "opencv2/opencv.hpp"
#include "hipacc.hpp"


using namespace cv;
using namespace std;
using namespace hipacc;

/**
 * @brief [brief description]
 * @details [long description]
 * 
 * @param inputAccessor [description]
 * @return [description]
 */
class SimpleFilter : public Kernel<uchar> {
    private:
        Accessor<uchar> &input;

    public:
        SimpleFilter(IterationSpace<uchar> &iterationSpace,
                     Accessor<uchar> &inputAccessor) :
        Kernel(iterationSpace),
        input(inputAccessor)
        {
            add_accessor(&input);
        }

        /**
         * @brief Kernel Function for simple mean calculation
         */
        void kernel()
        {
            int filter_width = 25;

            // Sobel operator in x direction
            float sum = 0;
            for(int x = -filter_width; x <= filter_width; x++)
                for(int y = -filter_width; y <= filter_width; y++)
                {
                    sum += ( input.pixel_at(input.x() + x, input.y() + y) );
                }
			sum /= (filter_width*2+1) * (filter_width*2+1);

            output() = (uchar) sum;
        }
};

/**
 * @brief Main function
 * 
 * @return exit status
 */
int main(int argc, const char **argv)
{
    // Read the input image from a given file 
    std::string fileName = "data/png/vhm.1001.dcm.png";
    Mat inputImage = imread(fileName.c_str(), CV_LOAD_IMAGE_GRAYSCALE);
    if (inputImage.empty())
    {
        std::cerr   << "Error reading image file '"
                    << fileName
                    << "'!"
                    << std::endl;
        return EXIT_FAILURE;
    }
    // Create an output image of the same size as the input file
    Mat outputImage = inputImage.clone();

    // HIPAcc DSL: create Image, Accessor and IterationSpace objects
    Image<uchar> imgIN(inputImage.cols, inputImage.rows, inputImage.data);
    Image<uchar> imgOUT(inputImage.cols, inputImage.rows);
    Accessor<uchar> imgAccessor(imgIN);
    IterationSpace<uchar> iter( imgOUT,
                                inputImage.rows,
                                inputImage.cols);

    // filter mask
    const uchar filter_mask[3][3] = {
        { (uchar)-1, (uchar)0, (uchar)1 },
        { (uchar)-1, (uchar)0, (uchar)1 },
        { (uchar)-1, (uchar)0, (uchar)1 }
    };
    Mask<uchar> mask(filter_mask);

    // HIPAcc DSL: create kernel instance
    // SimpleKernel derives from HIPAcc Kernel class
    // and was defined earlier
    SimpleFilter simpleFilter(iter, imgAccessor);
    simpleFilter.execute();



    /**
    // filter first image/inputImage
    BoundaryCondition<uchar> bound_img(img, mask, Boundary::CLAMP);
    Accessor<uchar> acc_img(bound_img);
    IterationSpace<uchar> iter_blur(filter_img);
    GaussianBlurFilter blur_img(iter_blur, acc_img, mask);

    // filter inputImage
    blur_img.execute();
    timing = hipacc_last_kernel_timing();
    fps_timing = timing;
    std::cerr << "HIPAcc blur filter: " << timing << " ms" << std::endl;

    unsigned char *output = 0;
    memcpy(inputImageRGB.data, (char*)&output, sizeof(output));
    */


    uchar *output = imgOUT.data();
    outputImage.data = output;

    // Show the laoded pixture
    imshow("Input Image", inputImage);
    imshow("Filter Example", outputImage);
    waitKey(0);

    return EXIT_SUCCESS;
}

