#include <iostream>
#include "opencv2/opencv.hpp"
#include "hipacc.hpp" // <-- include HIPAcc definitions

using namespace cv;
using namespace std;
using namespace hipacc;



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
        };

        /**
         * @brief Kernel Function for simple mean calculation
         */
        void kernel()
        {
            int filter_width = 30;
            float sum = 0;
            for(int x = -filter_width; x < filter_width; x++)
                for(int y = -filter_width; y < filter_width; y++)
                    sum += ( input.pixel_at(input.x() + x, input.y() + y) );

            sum /= (filter_width*2+1) * (filter_width*2+1);
            output() = (uchar) sum;
        }
};



int main(int argc, const char **argv)
{
    // Read the input image from a given file 
    std::string fileName = "data/jpg/Medical_X-Ray_imaging_WYJ07_nevit.jpg";
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
    IterationSpace<uchar> iter(imgOUT);

    // HIPAcc DSL: create kernel instance
    // SimpleKernel derives from HIPAcc Kernel class
    // and was defined earlier
    SimpleFilter simpleFilter(iter, imgAccessor);
    simpleFilter.execute();

    uchar *output = imgOUT.data();
    outputImage.data = output;

    // Show the pictures
    imshow("Input Image", inputImage);
    moveWindow("Filter Example", 50, 50);
    imshow("Filter Example", outputImage);
    moveWindow("Filter Example", 600, 50);
    waitKey(0);

    return EXIT_SUCCESS;
}

