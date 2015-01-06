#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

int main( int argc, char* argv[] )
{
    // read an image
    cv::Mat image = cv::imread( "lena.jpg" );
    // create image window named "lena"
    cv::namedWindow( "lena" );
    // show the image on window
    cv::imshow( "lena", image );
    // show the size of the image
    std::cout << "The size of lena: " << std::endl;
    std::cout << image.size().height << std::endl;
    std::cout << image.size().width << std::endl;
    // wait key for 5000ms
    //cv::waitKey( 5000 );
    cv::waitKey( 0 );
    cv::imwrite( "lena_output.jpg", image );

    return 0;
}