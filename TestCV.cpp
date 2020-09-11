#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>//cout için gerekli
// C:\\Users\\rootx\\Downloads\\logos\\c2.jpg
//C:\\Users\\rootx\\Downloads\\logos\\???
using namespace std;
using namespace cv;

#include <string>

#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>

bool usingMask;
Mat input_image; 
Mat temp_image;
Mat mask_image;
Mat result,result2;

const string image_window = "Input Image";
const string result_window = "Flat Image";
const string result2_window = "Rotated Image";
int matching, matching2,trackbar=5;

void testMatchingProessing(int, void*);

int main(int argc, char** argv)
{
    namedWindow(image_window, WINDOW_AUTOSIZE);
    namedWindow(result_window, WINDOW_AUTOSIZE);
    //namedWindow(result2_window, WINDOW_AUTOSIZE);
    string file = "Small_area.png";
    string file2 = "Small_area_rotated.png";

    input_image = imread("StarMap.png", IMREAD_COLOR);
    temp_image = imread(file, IMREAD_COLOR);
    
    cvtColor(temp_image, mask_image, COLOR_BGR2GRAY);

    const string trackbar_label = "Method: \n 0: SQDIFF \n 1: SQDIFF NORMED \n 2: TM CCORR \n 3: TM CCORR NORMED \n 4: TM COEFF \n 5: TM COEFF NORMED";
    createTrackbar(trackbar_label, image_window, &matching, trackbar, testMatchingProessing);
    testMatchingProessing(0, 0);
    waitKey(0);
    return EXIT_SUCCESS;
}
void testMatchingProessing(int, void*)
{
    Mat display,display2;
    input_image.copyTo(display);
    //

    int result_cols = input_image.cols - temp_image.cols + 1;
    int result_rows = input_image.rows - temp_image.rows + 1;

    result.create(result_rows, result_cols, CV_32FC1);

    bool method_accepts_mask = (TM_SQDIFF == matching || matching == TM_CCORR_NORMED);

    if (usingMask && method_accepts_mask)
    {
        matchTemplate(input_image, temp_image, result, matching, mask_image);
    }
    else
    {
        matchTemplate(input_image, temp_image, result, matching);
    }
    
    normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());

   //matchTemplate(display2, smallarea_rotate, result2, matching2);
    //normalize(result2, result2, 0, 2, NORM_MINMAX, 1, Mat());

    double minVal, minVal2;
    double maxVal, maxVal2;
    Point minLoc, minLoc2;
    Point maxLoc, maxLoc2;
    Point matchLoc, matchLoc2;

    minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());

    if (matching == TM_SQDIFF || matching == TM_SQDIFF_NORMED)
    {
        matchLoc = minLoc;
    }
    else
    {
        matchLoc = maxLoc;
    }

    rectangle(display, matchLoc, Point(matchLoc.x + temp_image.cols, matchLoc.y + temp_image.rows), Scalar(0,250,0), 2, 8, 0);

    rectangle(result, matchLoc, Point(matchLoc.x + temp_image.cols, matchLoc.y + temp_image.rows), Scalar(0, 250, 0), 2, 8, 0);
    
    resize(display, result, Size(), 0.35, 0.35);
    resize(display, display, Size(), 0.5, 0.5);
    imshow(image_window, display);
    imshow(result_window, result);
    return;
}