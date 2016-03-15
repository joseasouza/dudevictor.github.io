/*
  ~ Copyright (c) 2016-03-14 - José Victor - https://github.com/dudevictor/
  */
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv){
    Mat image;
    VideoCapture cap;
    vector<Mat> channels;

    image= imread(argv[1], CV_LOAD_IMAGE_COLOR);
    imshow("original", image);

    Mat imagemYCrCb;
    cvtColor(image, imagemYCrCb, CV_BGR2YCrCb);

    split(imagemYCrCb, channels);
    equalizeHist(channels[0], channels[0]);
    merge(channels, imagemYCrCb);
    Mat result;
    cvtColor(imagemYCrCb, result, CV_YCrCb2BGR);
    namedWindow("image", WINDOW_AUTOSIZE);
    imshow("image", result);
    waitKey();
    return 0;
}

