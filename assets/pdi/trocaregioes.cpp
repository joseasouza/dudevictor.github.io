#include <iostream>
#include <cv.h>
#include <highgui.h>

using namespace cv;
using namespace std;

void usage(int qtdArgs, char** args);
int main(int qtdArg, char** args) {
    usage(qtdArg, args);
    Mat image;

    image= imread(args[1], CV_LOAD_IMAGE_COLOR);
    int width = image.size().width;
    int height = image.size().height;


    Mat A(image, Rect(0, 0, width/2, height/2));
    Mat B(image, Rect(width/2, 0, width/2, height/2));
    Mat C(image, Rect(0, height/2, width/2, height/2));
    Mat D(image, Rect(width/2, height/2, width/2, height/2));

    Mat saida = Mat::zeros(image.size(), image.type());
    Mat aux;

    aux = saida.colRange(0, width/2).rowRange(0, height/2);
    D.clone().copyTo(aux);

    aux = saida.colRange(width/2, width).rowRange(0, height/2);
    C.copyTo(aux);

    aux = saida.colRange(0, width/2).rowRange(height/2, height);
    B.copyTo(aux);

    aux = saida.colRange(width/2, width).rowRange(height/2, height);
    A.copyTo(aux);

    if(!image.data)
        cout << "nao abriu a imagem" << endl;

    namedWindow("janela", WINDOW_AUTOSIZE);

    imshow("janela", saida);
    waitKey();
    return 0;

}

void usage(int qtdArgs, char** args) {

    if (qtdArgs < 2) {
        printf("Uso: ./trocaregioes <nome_arquivo>\n");
        exit(0);
    }

}
