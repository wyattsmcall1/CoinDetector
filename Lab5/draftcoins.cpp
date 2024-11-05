#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

int main(int argc, char** argv)
{
    Mat src, src_gray;
    
    /// Read the image
    src = imread( argv[1], 1 );
    
    if( !src.data )
    { return -1; }
    
    /// Convert it to gray
    cvtColor( src, src_gray, CV_BGR2GRAY );
    
    /// Reduce the noise so we avoid false circle detection
    GaussianBlur( src_gray, src_gray, Size(9, 9), 2, 2 );
    
    vector<Vec3f> circles;
    
    /// Apply the Hough Transform to find the circles
    HoughCircles( src_gray, circles, CV_HOUGH_GRADIENT, 1, src_gray.rows/8, 200, 100, 0, 0 );
    
    /// Draw the circles detected
    for( size_t i = 0; i < circles.size(); i++ )
    {
        Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        int radius = cvRound(circles[i][2]);
        // circle center
        circle( src, center, 3, Scalar(0,255,0), -1, 8, 0 );
        // circle outline
        circle( src, center, radius, Scalar(0,0,255), 3, 8, 0 );
    }
    
    int max_radius=max(radius);
    radius=radius/max_radius;
    
    int i=1;
    double d=0;
    double n=0;
    double q=0;
    double total_coins=0;
    double currency=0;
    
    while (i<length(radius)+1){
        if(vec(i)<0.68){
            
        }
        else if (vec(i)<0.82){
             d=d+1;
        }
        else if (vec(i)<0.94){
            n=n+1;
        }
        else if (vec(i)<1.01){
            q=q+1;
        }
        i=i+1;
    }
    total_coins=q+n+d;
    currency=(q*25.0+d*10.0+n*5.0)/100;
    cout<<"Dollars: $ "<<currency;
    
    /// Show your results
    namedWindow( "Hough Circle Transform Demo", CV_WINDOW_AUTOSIZE );
    imshow( "Hough Circle Transform Demo", src );
    
    waitKey(0);
    return 0;
}
