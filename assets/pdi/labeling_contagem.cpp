/*
  ~ Copyright (c) 2016 - José Victor Alves de Souza - https://github.com/dudevictor/
  */
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;

int main(int argc, char** argv){
  Mat image, mask;
  int width, height;
  int nobjects;
  int BRANCO = 255;
  int PRETO = 0;
  int BACKGROUND = 1;
  int COM_BOLHA = 67;
  int SEM_BOLHA = 196;
  CvPoint p;
  p.x = 0;
  p.y = 0;
  image = imread(argv[1],CV_LOAD_IMAGE_GRAYSCALE);
  
  if(!image.data){
    std::cout << "imagem nao carregou corretamente\n";
    return(-1);
  }
  width=image.size().width;
  height=image.size().height;

  //Aplica 255 em todos os pontos na borda da
  // imagem e aplica-se o floodfill para remover os elementos que tocam as bordas
  for (int i = 0; i < width; i++) {
    image.at<uchar>(i, height -1) = BRANCO;
    image.at<uchar>(i, 0) = BRANCO;
  }

  for (int i = 0; i < height; i++) {
    image.at<uchar>(0, i) = BRANCO;
    image.at<uchar>(width -1, i) = BRANCO;
  }
  //Aplica-se o floodfill no ponto (0, 0) removendo todos os elementos da borda
  floodFill(image, p, PRETO);
  //Aplica-se o floodfill em todo todo o background da imagem
  floodFill(image, p, BACKGROUND);

  //Procurando por Elementos, julgando inicialmente que nenhum tem bolhas
  int qtdTotal = 0;
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      if (image.at<uchar>(i, j) == BRANCO) {
        p.x = j;
        p.y = i;
        floodFill(image, p, SEM_BOLHA);
        qtdTotal++;
      }
    }
  }

  //Procurando por Elementos que possuem bolhas
  int qtdComBolhas = 0;
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      if (image.at<uchar>(i,j) == SEM_BOLHA) {
        p.x = j;
        p.y = i;
      } else if (image.at<uchar>(i,j) == PRETO) {
        floodFill(image, p, COM_BOLHA);
        p.x = j;
        p.y = i;
        floodFill(image, p, COM_BOLHA);
        qtdComBolhas++;
      }
    }
  }

  imshow("image", image);
  imwrite("labeling.png", image);
  printf("Quantidade de sem bolhas: %d, com bolhas: %d\n", qtdTotal - qtdComBolhas, qtdComBolhas);
  waitKey();
  return 0;
}
