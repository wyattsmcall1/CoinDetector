#include <jni.h>
#include <opencv2/core/core.hpp>
#include <android/log.h>

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <string.h>

using namespace std;
using namespace cv;

extern "C" {

// function declaration for manual color conversion
int convertYUVtoARGB(int, int, int);

JNIEXPORT void JNICALL Java_org_ece420_lab5_Sample4View_YUV2RGB(JNIEnv*, jobject, jlong addrYuv, jlong addrRgba)
{
	Mat* pYUV=(Mat*)addrYuv;
	Mat* pRGB=(Mat*)addrRgba;

	/*INSERT CODE TO CONVERT AN ENTIRE IMAGE FROM FROM YUV420sp TO ARGB*/
	int width = pYUV -> cols;
	int height = (pYUV -> rows)*2/3;
	int u, v, y1, y2, y3, y4;
	int index = 0;

	for(int j=0; j<height; j+=2)
	{
		for(int i=0; i<width; i+=2)
		{
			y1 = pYUV->at<uchar>(j,i)&0xff;
			y2 = pYUV->at<uchar>(j,i+1)&0xff;
			y3 = pYUV->at<uchar>(j+1,i)&0xff;
			y4 = pYUV->at<uchar>(j+1,i+1)&0xff;

			u = pYUV->at<uchar>(index/width+height,index%width)&0xff;
			v = pYUV->at<uchar>(index/width+height,index%width+1)&0xff;

			pRGB->at<int>(j,i) = convertYUVtoARGB(y1, u, v);
			pRGB->at<int>(j,i+1) = convertYUVtoARGB(y2, u, v);
			pRGB->at<int>(j+1,i) = convertYUVtoARGB(y3, u, v);
			pRGB->at<int>(j+1,i+1) = convertYUVtoARGB(y4, u, v);

			index+=2;
		}
	}
}

JNIEXPORT double JNICALL Java_org_ece420_lab5_Sample4View_HistEQ(JNIEnv* env, jobject thiz, jlong addrYuv, jlong addrRgba)
{
	Mat* pYUV=(Mat*)addrYuv;
	Mat* pRGB=(Mat*)addrRgba;

	Mat src_gray;

	int d=0;
	int n=0;
	int q=0;
	int total_coins=0;
	double currency=0;
	int threshold_value=50;
	int Max_Binary_Value=100;

	vector<Vec3f> circles;

    // convert to rgb and store
    Java_org_ece420_lab5_Sample4View_YUV2RGB(env, thiz, addrYuv, addrRgba);

	// Convert it to gray
	cvtColor(*pRGB, src_gray, CV_BGR2GRAY );

	//convert it to binary
	//threshold(src_gray,src_gray,threshold_value,Max_Binary_Value,0);

	GaussianBlur(src_gray,src_gray,Size(9,9),2,2);

	// Apply the Hough Transform to find the circles
	HoughCircles(src_gray, circles, CV_HOUGH_GRADIENT, 1, 50, 255, 30, 10, 200);
	double radius[(int) circles.size()];
	double relradius[(int) circles.size()];
	double max_radius;
    max_radius=0;

	for(size_t i = 0; i < circles.size(); i++ )
    {
        radius[i] = cvRound(circles[i][2]);
    	if(radius[i]>max_radius)
    	{
    		max_radius=radius[i];
        }
    }

	for(size_t i = 0; i < circles.size(); i++ )
    {
		relradius[i]=radius[i]/max_radius;
		// Draw the circles detected
		Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
		// circle center
		circle(*pRGB, center, 3, Scalar(0,255,0), -1, 8, 0);
		// circle outline
		circle(*pRGB, center, radius[i], Scalar(0,0,255), 3, 8, 0);

		if (relradius[i] < 0.82){
			d++;
		}
		else if (relradius[i] < 0.94){
			n++;
		}
		else if (relradius[i] < 1.01){
			q++;
		}
    }

   total_coins=q+n+d;
   currency=(q*25.0+d*10.0+n*5.0)/100.0;
   char text[200];
   //sprintf(text,"Size: %d %d",relradius[0], relradius[1]);
   sprintf(text,"$: %f, Q: %d, N: %d, D: %d",currency,q,n,d);
   int fontFace = FONT_HERSHEY_DUPLEX;
   double fontScale = 1.5;
   int thickness = 3;

   int baseline=0;
   Size textSize = getTextSize(text, fontFace,
                               fontScale, thickness, &baseline);
   baseline += thickness;

   // center the text
   Point textOrg((src_gray.cols - textSize.width)/2,
                 (textSize.height)*2);

   // draw the box
   rectangle(*pRGB, textOrg + Point(0, baseline),
             textOrg + Point(textSize.width, -textSize.height),
             Scalar(0,0,255));
   // ... and the baseline first
   line(*pRGB, textOrg + Point(0, thickness),
        textOrg + Point(textSize.width, thickness),
        Scalar(0, 0, 255));

   // then put the text itself
   putText(*pRGB, text, textOrg, fontFace, fontScale,
           Scalar(0, 0, 180), thickness, 8);

   return currency;
}

JNIEXPORT double JNICALL Java_org_ece420_lab5_Sample4View_HistEQ1(JNIEnv* env, jobject thiz, jlong addrYuv, jlong addrRgba)
{
	Mat* pYUV=(Mat*)addrYuv;
	Mat* pRGB=(Mat*)addrRgba;

	Mat src_gray;

	int uno=0;
	int dos=0;
	int cinco=0;
	int diez=0;
	int total_coins=0;
	int currency=0;
	int threshold_value=50;
	int Max_Binary_Value=100;

	vector<Vec3f> circles;

    // convert to rgb and store
    Java_org_ece420_lab5_Sample4View_YUV2RGB(env, thiz, addrYuv, addrRgba);

	// Convert it to gray
	cvtColor(*pRGB, src_gray, CV_BGR2GRAY );

	//convert it to binary
	//threshold(src_gray,src_gray,threshold_value,Max_Binary_Value,0);

	GaussianBlur(src_gray,src_gray,Size(9,9),2,2);

	// Apply the Hough Transform to find the circles
	HoughCircles(src_gray, circles, CV_HOUGH_GRADIENT, 1, 50, 255, 30, 10, 200);
	double radius[(int) circles.size()];
	double relradius[(int) circles.size()];
	double max_radius;
    max_radius=0;

	for(size_t i = 0; i < circles.size(); i++ )
    {
        radius[i] = cvRound(circles[i][2]);
    	if(radius[i]>max_radius)
    	{
    		max_radius=radius[i];
        }
    }

	for(size_t i = 0; i < circles.size(); i++ )
    {
		relradius[i]=radius[i]/max_radius;
		// Draw the circles detected
		Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
		// circle center
		circle(*pRGB, center, 3, Scalar(0,255,0), -1, 8, 0);
		// circle outline
		circle(*pRGB, center, radius[i], Scalar(0,0,255), 3, 8, 0);

		if (relradius[i] < 0.80){
			uno++;
		}
		else if (relradius[i] < 0.87){
			dos++;
		}
		else if (relradius[i] < 0.965){
					cinco++;
				}
		else if (relradius[i] < 1.01){
			diez++;
		}
    }

   total_coins=uno+dos+cinco+diez;
   currency=uno+dos*2+cinco*5+diez*10;
   char text[200];
   //sprintf(text,"Size: %d %d",relradius[0], relradius[1]);
   sprintf(text,"$: %d, U: %d, Do: %d, C: %d, Di: %d  ",currency, uno, dos, cinco, diez);
   int fontFace = FONT_HERSHEY_DUPLEX;
   double fontScale = 1.5;
   int thickness = 3;

   int baseline=0;
   Size textSize = getTextSize(text, fontFace,
                               fontScale, thickness, &baseline);
   baseline += thickness;

   // center the text
   Point textOrg((src_gray.cols - textSize.width)/2,
                 (textSize.height)*2);

   // draw the box
   rectangle(*pRGB, textOrg + Point(0, baseline),
             textOrg + Point(textSize.width, -textSize.height),
             Scalar(0,0,255));
   // ... and the baseline first
   line(*pRGB, textOrg + Point(0, thickness),
        textOrg + Point(textSize.width, thickness),
        Scalar(0, 0, 255));

   // then put the text itself
   putText(*pRGB, text, textOrg, fontFace, fontScale,
		   Scalar(180, 0, 0), thickness, 8);

   return currency;
}


int convertYUVtoARGB(int y, int u, int v) {
	/*INSERT CODE TO CONVERT A YUV PIXEL TO A 32-BIT INT REPRESENTING AN ARGB PIXEL*/
	int r,g,b;

	r = y + (int)(1.370705 * (v-128));
	g = y - (int)(0.698001 * (v-128)) - (0.337633 * (u-128));
	b = y + (int)(1.732446 * (u-128));

	r = r > 255 ? 255 : r < 0 ? 0 : r;
	g = g > 255 ? 255 : g < 0 ? 0 : g;
	b = b > 255 ? 255 : b < 0 ? 0 : b;

	return 0xff000000 | (r<<16) | (g<<8) | b;
}

}
