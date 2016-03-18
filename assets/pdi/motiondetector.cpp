/*
  ~ Copyright (c) 2016-03-14 - José Victor - https://github.com/dudevictor/
  */
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

double tolerancia = 0.995;
int COUNT_MAX = 3;
bool uniform = true;
bool acummulate = false;
int nbins = 64;
float range[] = {0, 256};
const float *histrange = { range };

void calcHistogram(Mat *out, Mat image) {


    vector<Mat> planes;
    split (image, planes);
    calcHist(&planes[0], 1, 0, Mat(), *out, 1,
             &nbins, &histrange,
             uniform, acummulate);
    normalize(*out, *out, 0, 1, NORM_MINMAX, -1, Mat());
}

int main(int argc, char** argv){
    Mat image;
    int width, height;
    VideoCapture cap;
    Mat histR;
    cap.open(0);

    if(!cap.isOpened()){
        cout << "cameras indisponiveis";
        return -1;
    }

    width  = cap.get(CV_CAP_PROP_FRAME_WIDTH);
    height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);

    cout << "largura = " << width << endl;
    cout << "altura  = " << height << endl;

    int histw = nbins, histh = nbins/2;
    Mat lastHist;
    cap >> lastHist;
    calcHistogram(&lastHist, lastHist);
    double correlacao = 1;
    int cont = 0;
    while(1){
        cap >> image;

        calcHistogram(&histR, image);

        correlacao = compareHist(histR, lastHist, CV_COMP_CORREL);
        lastHist = histR.clone();

        if (correlacao < tolerancia) {
            cont++;
            if (cont == COUNT_MAX) {
                circle(image, Point(16, 16), 15, Scalar(0, 0, 255), CV_FILLED);
            }
        } else {
            cont = 0;
        }
        printf("Correlacao: %f\n", correlacao);
        imshow("image", image);
        if(waitKey(30) >= 0) break;
    }
    return 0;
}
