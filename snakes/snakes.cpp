#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/background_segm.hpp>

int main( int argc, char** argv )
{
    if ( argc < 2 ) {
        std::cerr << "Missing Parameter! " << std::endl;
        std::cerr << argv[0] << " inputVideo" << std::endl;
        return 1;
    }
    // open video file
    cv::VideoCapture capture( argv[1] );

    // check whether the video is opened
    if ( !capture.isOpened() )
        return 1;

    // obtain frame rate
    double rate = capture.get( CV_CAP_PROP_FPS );
    bool stop = false;
    cv::Mat frame; // current frame
    cv::namedWindow( "Extracted Frame" );
    // delay on each frame
    // corresponding to the rate
    int delay = 1000 / rate;

    // declare variables for grayscale conversion
    cv::Mat grayscaleFrame;
    cv::namedWindow( "Grayscale Frame" );

    // declare variables for binary thresholding
    cv::Mat thFrame;
    cv::namedWindow( "Thresholded Frame" );

    // declare variables for background subtractor
    cv::Mat bgFgFrame, bgBgFrame;
    //cv::BackgroundSubtractorMOG bgSubtractor( 20, 10, 0.5, false); 
    cv::BackgroundSubtractorMOG2 bgSubtractor( 20, 16, true );
    cv::namedWindow( "Foreground Frame" );
    cv::namedWindow( "Background Frame" );

    // declare variables for erosion and dilation
    cv::Mat erodeFrame, dilateFrame;
    cv::namedWindow( "Eroded Frame" );
    cv::namedWindow( "Dilated Frame" );

    // ergo each frame
    while ( !stop ) {
        // try to read the next frame
        if ( !capture.read( frame ) )
            break;

        cvtColor( frame, grayscaleFrame, CV_RGB2GRAY );
 
        bgSubtractor( frame, bgFgFrame, 0.001);
        bgSubtractor.getBackgroundImage( bgBgFrame );
        
        //cv::threshold( bgFgFrame, thFrame, 240, 255, CV_THRESH_BINARY );
        //cv::erode( bgFgFrame, erodeFrame, cv::Mat() );
        cv::dilate( bgFgFrame, dilateFrame, cv::Mat() );
        
        cv::imshow( "Extracted Frame", frame );
        cv::imshow( "Grayscale Frame", grayscaleFrame );
        //cv::imshow( "Thresholded Frame", thFrame );
        cv::imshow( "Foreground Frame", bgFgFrame );
        cv::imshow( "Background Frame", bgBgFrame );
        //cv::imshow( "Eroded Frame", erodeFrame );
        cv::imshow( "Dilated Frame", dilateFrame );
        // introduce the delay
        // also can be stopped by clicking the keyboard
        if ( cv::waitKey( delay ) >= 0 )
            stop = true;
    }
    // close the video
    // called by the destructor, unnecessary to do this explicitly
    capture.release();
}