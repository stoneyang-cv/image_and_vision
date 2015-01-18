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
    //int delay = 1000 / rate;

    // declare variables for smoothing
    cv::Mat blurFrame;
    cv::namedWindow( "Smoothed Frame" );

    // declare variables for grayscale conversion
    //cv::Mat grayscaleFrame;
    //cv::namedWindow( "Grayscale Frame" );

    // declare variables for binary thresholding
    //cv::Mat thFrame;
    //cv::namedWindow( "Thresholded Frame" );

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

    // declare variables for bounding box
    cv::RNG rng( 12345 );
    std::vector< std::vector< cv::Point > > contours;
    std::vector< cv::Vec4i > hierarchy;
    cv::namedWindow( "Contours Frame" );

    // ergo each frame
    while ( !stop ) {
        // try to read the next frame
        if ( !capture.read( frame ) )
            break;
/*
        for ( int i = 1; i < 7; i = i + 2 ) { 
            cv::GaussianBlur( frame, blurFrame, cv::Size( i, i ), 0, 0 );
        }
*/
        //cv::GaussianBlur( frame, blurFrame, cv::Size( 5, 5 ), 0, 0 );
        cv::medianBlur( frame, blurFrame, 3 );
        //cv::bilateralFilter( frame, blurFrame, 3, 2, 5 );
        //cv::adaptiveBilateralFilter( frame, blurFrame, cv::Size( 3, 3 ), 5 );

        //cvtColor( frame, grayscaleFrame, CV_RGB2GRAY );
 
        bgSubtractor( blurFrame, bgFgFrame, 0.001);
        bgSubtractor.getBackgroundImage( bgBgFrame );
        
        //cv::threshold( bgFgFrame, thFrame, 240, 255, CV_THRESH_BINARY );
        cv::erode( bgFgFrame, erodeFrame, cv::Mat() );
        cv::dilate( erodeFrame, dilateFrame, cv::Mat() );

        cv::findContours( dilateFrame, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point( 0, 0 ) );

        /// Approximate contours to polygons + get bounding rects and circles
        std::vector< std::vector< cv::Point > > contours_poly( contours.size() );
        std::vector< cv::Rect > boundRect( contours.size() );
        std::vector< cv::Point2f > center( contours.size() );
        std::vector< float > radius( contours.size() );

        for ( int i = 0; i < contours.size(); i++ ) { 
            cv::approxPolyDP( cv::Mat( contours[i] ), contours_poly[i], 3, true );
            boundRect[i] = cv::boundingRect( cv::Mat( contours_poly[i] ) );
            cv::minEnclosingCircle( (cv::Mat) contours_poly[i], center[i], radius[i] );
        }

        /// Draw polygonal contour + bonding rects + circles
        cv::Mat drawing = cv::Mat::zeros( dilateFrame.size(), CV_8UC3 );
        for ( int i = 0; i < contours.size(); i++ ) {
            cv::Scalar color = cv::Scalar( rng.uniform( 0, 255 ), rng.uniform( 0, 255 ), rng.uniform( 0, 255 ) );
            //cv::drawContours( drawing, contours_poly, i, color, 1, 8, std::vector< cv::Vec4i >(), 0, cv::Point() );
            cv::rectangle( drawing, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0 );
            //cv::circle( drawing, center[i], ( int ) radius[i], color, 2, 8, 0 );
        }
        
        cv::imshow( "Extracted Frame", frame );
        cv::imshow( "Smoothed Frame", blurFrame );
        //cv::imshow( "Grayscale Frame", grayscaleFrame );
        //cv::imshow( "Thresholded Frame", thFrame );
        cv::imshow( "Foreground Frame", bgFgFrame );
        cv::imshow( "Background Frame", bgBgFrame );
        cv::imshow( "Eroded Frame", erodeFrame );
        cv::imshow( "Dilated Frame", dilateFrame );
        cv::imshow( "Contours Frame", drawing );
        // introduce the delay
        // also can be stopped by clicking the keyboard
        //if ( cv::waitKey( delay ) >= 0 )
        //    stop = true;
        char keyPress = cv::waitKey( 20 );
        if ( keyPress == 27 )
            stop = true;
    }
    // close the video
    // called by the destructor, unnecessary to do this explicitly
    capture.release();
}