---
layout:     project
title:      Processamento Digital de Imagens
subject:    Processameto Digital de Imagens
date:       2016-03-03 23:45:00
summary:    Algoritmos de processamento e tratamento de imagens com OpenCV
categories: ufrn
teacher:    Agostinho Brito Júnior
semester: 2016.1
tags:
 - floodfill
 - processamento digital
 - opencv
 - imagens
 - UFRN
 - DCA
---
### Introdução
Esta página tem como objetivo apresentar os resultados dos exercícios da disciplina de Processamento Digital de Imagens
lecionada pelo professor [{{page.teacher}}][1] no semestre {{ page.semester}}. Estes exercícios podem ser encontrados [aqui][2].
Para compilar os códigos apresentados neste tópico pode-se utilizar [este arquivo][7].

Esta página estará sendo atualizada conforme a disciplina for sendo encaminhada.

### Preenchendo Regiões com OpenCV

No programa [labeling.cpp][3] fornecido, cada objeto encontrado é rotulado com um valor no tom de cinza que varia entre `0` e `255`.
Portanto, quando há mais do que 255 objetos na cena, a rotulação fica comprometida pois não mais mais tons de cinza
que possam ser utilizados para rotular o restante dos objetos.

Uma possível solução para este problema seria resetar a contagem da variável `nobjects`, utilizando a função módulo.
O código modificado está apresentado a seguir:

```
/* ... Código antes */
  nobjects=0;
  for(int i=0; i<height; i++){
    for(int j=0; j<width; j++){
      if(image.at<uchar>(i,j) == 255){
		// achou um objeto
		nobjects++;
		p.x=j;
		p.y=i;
		floodFill(image,p,nobjects % 255);
	  }
	}
  }
/* ... Código Depois */
```

Dessa forma, quando o valor de `nobjects` passa a ser maior ou igual a 255, a escala de cinza passa a ser "zerada".
 
 O próximo passo é implementar uma solução que realize a contagem de objetos que possuem buracos
 e de objetos não possuem buracos, desconsiderando aqueles elementos que tocam a borda da imagem.
 Para isso, foi utilizado a seguinte imagem como teste:
 ![Imagem binária de bolhas][4]
 
 O primeiro passo a se fazer é remover todas os elementos que tocam a borda. Para isso, aplica-se o tom de 
 cinza branco (255) a todos os pixels da borda e então, realiza-se um _floodfill_ para o tom de cinza `0` igual ao do fundo da imagem.
 Depois disso, realiza-se uma contagem total de bolhas, rotulando todas elas inicialmente como "SEM BOLHAS".
 E por fim, realiza-se a rotulagem por aqueles elementos que possuem bolhas. Para fazer isso, percorre-se a matriz da imagem,
  e sempre que for encontrado um pixel com rótulo SEM BOLHA, armazena-se as coordenadas x e y desse elemento na variável `p`.
  O laço é continuado até encontrar um pixel de tom de cinza `0` (caso em que foi encontrada uma bolha), e nessa situação aplica-se o _floodfill_ 
  no último elemento `p` encontrado, rotulando este último agora como "COM BOLHA". 
  
O código completo em `C++` é apresentado abaixo. O código também pode ser baixado por [aqui][5].
 
```
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

  //Rotula como BRANCO todos os pontos na borda da imagem e aplica-se o floodfill para remover os elementos que tocam as bordas
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
```
O resultado está apresentado na imagem abaixo:
 ![Resultado Contagem por Labeling][6]


{%include tags.html%}

[1]: http://agostinhobritojr.github.io/
[2]: http://agostinhobritojr.github.io/tutoriais/pdi/
[3]: {{ site.baseurl }}/assets/pdi/labeling.cpp
[4]: {{site.baseurl}}/assets/pdi/bolhas.png
[5]: {{site.baseurl}}/assets/pdi/labeling_contagem.cpp
[6]: {{site.baseurl}}/assets/pdi/resultadoBolhas.png
[7]: {{site.baseurl}}/assets/pdi/Makefile


