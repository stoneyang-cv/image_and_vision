#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

int main( int argc, char* argv[] )
{
    // read an image
    cv::Mat src = cv::imread( "lena.jpg" );
    // create image window named "lena"
    cv::namedWindow( "lena" );
    // show the image on window
    cv::imshow( "lena", src );
    // show the size of the image
    std::cout << "The size of lena: " << std::endl;
    std::cout << src.size().height << std::endl;
    std::cout << src.size().width << std::endl;
    // flip the image...
    cv::Mat dst, dst2;

    cv::flip( src, dst, 1 );
    cv::namedWindow( "lena_flip" );
    cv::imshow( "lena_flip", dst );

    cv::flip( src, dst2, 0 );
    cv::namedWindow( "lena_flip2" );
    cv::imshow( "lena_flip2", dst2 );
    // wait key for 5000ms
    //cv::waitKey( 5000 );
    cv::waitKey( 0 );
    cv::imwrite( "lena_output.jpg", dst );
    cv::imwrite( "lena_output2.jpg", dst2 );

    return 0;
}