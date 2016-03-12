/*
  ~ Copyright (c) 2016 - José Victor - https://github.com/dudevictor/
  */
#include <iostream>
#include <cv.h>
#include <highgui.h>

using namespace cv;
using namespace std;

void usage(int qtdArgs, char** args);

int main(int qtdArg, char** args){
    usage(qtdArg, args);
    Mat image;

    image= imread(args[1], CV_LOAD_IMAGE_COLOR);

    int p1x = atoi(args[2]);
    int p1y = atoi(args[3]);
    int p2x = atoi(args[4]);
    int p2y = atoi(args[5]);

    if(!image.data)
        cout << "nao abriu a imagem" << endl;

    namedWindow("janela", WINDOW_AUTOSIZE);

    for(int i=p1x;i< p2x;i++) {
        for(int j=p1y; j< p2y;j++){
            Vec3b valor = image.at<Vec3b>(i,j);
            valor[0] = 255 - valor[0];
            valor[1] = 255 - valor[1];
            valor[2] = 255 - valor[2];
            image.at<Vec3b>(i,j) = valor;
        }
    }

    imshow("janela", image);
    waitKey();
    return 0;

}

void usage(int qtdArgs, char** args) {

    if (qtdArgs < 6) {
        printf("Uso: ./regions <nome_arquivo> X1 Y1 X2 Y2\n");
        exit(0);
    }

}