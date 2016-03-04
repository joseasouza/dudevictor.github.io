---
layout:     project
title:      Programação Concorrente e Distribuída
subject:    Programação Concorrente e Distribuída
date:       2016-03-04 02:23:00
summary:    Resolução da lista de exercícios do livro An Introduction to Parallel Programming
categories: ufrn
teacher:    Samuel Xavier de Souza
semester: 2015.2
tags:
 - programação paralela
 - threads
 - MPI
 - OpenMP
 - DCA
 - UFRN
---
Esta página tem como objetivo apresentar a resolução das listas de exercícios do livro ["An Introduction to Parallel Programming"][1]
de Peter Pacheco. A resolução dessas listas foi utilizada durante o curso de Programação Paralela no DCA / UFRN.

Esta pagina ainda está sendo atualizada.

[1]: https://www.cs.usfca.edu/~peter/ipp/

{%include newSeccion.html%}
##Chapter 01
>1.1 Devise formulas for the functions that calculate my first i and my last i in the global sum example.
 Remember that each core should be assigned roughly the same number of elements of computations in the loop.
  Hint: First consider the case when n is evenly divisible by p.

No raciocínio, foi considerado que o intervalo é fechado em my_first_i e aberto em my_last_i.
   A ocasião em que n é divisível por p é simples de formular, uma vez que o my_first_i é o index do núcleo multiplicado pela divisão entre n e p.
    As distribuições são iguais nessa situação, então o my_last_i será igual ao my_first_i somado a divisão n/p.
  No caso em que n não é divisível por p é necessário um maior tratamento.
  Nessa situação, o resto da divisão tem que ser distribuído entre os outros núcleos. No máximo n - 1 (maior resto possível) itens serão distribuídos.

>1.2 We’ve implicitly assumed that each call to Compute next value requires roughly the same amount of work as the other calls.
 How would you change your answer to the preceding question if call i = k requires k + 1 times as much work as the call with i = 0?
  So if the first call (i = 0) requires 2 milliseconds, the second call (i = 1) requires 4, the third (i = 2) requires 6, and so on.

  Nessa situação, nota-se que o custo de cada requisição do compute next cresce a uma constante multiplicada por 2i.
   Uma estratégia que poderia ser tomada nessa situação (talvez não a melhor) seria fazer com que a quantidade de elementos que cada núcleo executaria seria metade dos elementos faltantes. Por exemplo, para n = 20 e p = 4, o primeiro nucleo executaria os 10 primeiros, o segundo núcleo executaria os 5 seguintes, o terceiro núcleo os  3 seguintes e o último núcleo os 2 restantes. Nessa situação apresentada, considerando um custo inicial de 2, os custos totais tomados por cada núcleo seria: 110, 130, 102 e 78.
  	Considere agora n =5 e p = 2. Os custos seriam: 2 4 6 8 10, respectivamente.
  	Seguindo a lógica anterior, o primeiro núcleo executaria os índices: 0 , 1 e 2. E o segundo executaria os índices 3 e 4.
  	 Os custos totais seriam dessa situação seriam: 12 e 18, respectivamente, para cada núcleo.

>1.3 Try to write pseudo-code for the tree-structured global sum illustrated in Figure 1.1. Assume the number of cores is a power of two (1, 2, 4, 8, . . . ).
 Hints: Use a variable divisor to determine whether a core should send its sum or receive and add. The divisor should start with the value 2 and be doubled after each iteration. Also use a variable core difference to determine which core should be partnered with the current core. It should start with the value 1 and also be doubled after each iteration. For example, in the first iteration 0 % divisor = 0 and 1 % divisor = 1, so 0 receives and adds, while 1 sends. Also in the first iteration 0 + core difference = 1 and 1 − core difference = 0, so 0 and 1 are paired in the first iteration.

```
divisor = 2;
coreDifference = 1;
jaEnviou = false;
for (i = 0; i < log(p) base 2; i++ ) { // 3 iterações para p = 8
    if (indexCore % divisor == 0) {
         // receba e some de (indexCore + coreDifference);
    } else {
         if ( ! jaEnviou) { // Nao envie caso já tenha enviado numa iteração anterior
                // envie para (indexCore - coreDiffrence);
	   jaEnviou = true;
        }
    }
   divisor *= 2;
   coreDifference *= 2;
}
```

