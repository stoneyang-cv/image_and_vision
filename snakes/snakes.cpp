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

    bool stop = false;
    cv::Mat frame; // current frame
    cv::Mat resizedFrame;

    // declare variables for smoothing
    cv::Mat medianFrame;

    // declare variables for background subtractor
    cv::Mat bgFgFrame, bgBgFrame;
    //cv::BackgroundSubtractorMOG bgSubtractor( 20, 10, 0.5, false); 
    cv::BackgroundSubtractorMOG2 bgSubtractor( 20, 16, true );
    
    cv::Mat thresFrame;
    
    cv::Mat medianFrame2;
    
    // declare variables for erosion and dilation
    cv::Mat erodeFrame, dilateFrame;
    int morph_elem = 0;
    int dilate_size = 3;
    int erode_size = 1;
    cv::Mat dilateElement = cv::getStructuringElement( morph_elem, cv::Size( 2 * dilate_size + 1, 2 * dilate_size + 1 ), cv::Point( dilate_size, dilate_size ) );
    cv::Mat erodeElement = cv::getStructuringElement( morph_elem, cv::Size( 2 * erode_size + 1, 2 * erode_size + 1 ), cv::Point( erode_size, erode_size ) );
    cv::Mat copiedFrame;  

    // declare variables for bounding box
    cv::RNG rng( 12345 );
    std::vector< std::vector< cv::Point > > contours;
    std::vector< cv::Vec4i > hierarchy;
        
    // declare variables for overlay contours and raw frames
    cv::Mat overlayFrame;
    
    // ergo each frame
    while ( !stop ) {
        // try to read the next frame
        if ( !capture.read( frame ) )
            break;
        cv::imshow( "Extracted Frame", frame );

        cv::resize( frame, resizedFrame, cv::Size( 352, 288 ) );
        cv::imshow( "Resized Frame", resizedFrame );

        cv::medianBlur( resizedFrame, medianFrame, 3 );
        cv::imshow( "Median Frame", medianFrame );

        bgSubtractor( medianFrame, bgFgFrame, 0.001);
        bgSubtractor.getBackgroundImage( bgBgFrame );
        cv::imshow( "Foreground Frame", bgFgFrame );
        cv::imshow( "Background Frame", bgBgFrame );

        cv::threshold( bgFgFrame, thresFrame, 128, 255, CV_THRESH_BINARY );
        cv::imshow( "Thresholded Frame", thresFrame ); 

        cv::medianBlur( thresFrame, medianFrame2, 5 );
        cv::imshow( "Median Frame 2", medianFrame2 ); 

        cv::erode( medianFrame2, erodeFrame, erodeElement );
        cv::dilate( erodeFrame, dilateFrame, dilateElement );
        cv::imshow( "Eroded Frame", erodeFrame );         
        cv::imshow( "Dilated Frame", dilateFrame );
     
        dilateFrame.copyTo( copiedFrame );
        //erodedresize.copyTo( copiedFrame );

        cv::Mat copiedResize = cv::Mat::zeros( frame.size(), CV_8UC3 );
        cv::resize( copiedFrame, copiedResize, frame.size() );
        cv::findContours( copiedResize, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point( 0, 0 ) );

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
        cv::Mat drawing = cv::Mat::zeros( frame.size(), CV_8UC3 );     
        for ( int i = 0; i < contours.size(); i++ ) {
            //cv::Scalar color = cv::Scalar( rng.uniform( 0, 255 ), rng.uniform( 0, 255 ), rng.uniform( 0, 255 ) );
            cv::Scalar color( 0, 200, 0 );
            //cv::drawContours( drawing, contours_poly, i, color, 1, 8, std::vector< cv::Vec4i >(), 0, cv::Point() );
            cv::rectangle( drawing, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0 );
            //cv::circle( drawing, center[i], ( int ) radius[i], color, 2, 8, 0 );
        }
        cv::imshow( "Contours Frame", drawing );

        // overlay
        cv::addWeighted( frame, 1.0, drawing, 1.0, 0.9, overlayFrame );
        cv::imshow( "Overlayed Frame", overlayFrame );
        
        char keyPress = cv::waitKey( 20 );
        if ( keyPress == 27 )
            stop = true;
    }
    // close the video
    // called by the destructor, unnecessary to do this explicitly
    capture.release();
}