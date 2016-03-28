---
layout:     project
title:      Programação Concorrente e Distribuída (2)
subject:    Programação Concorrente e Distribuída
date:       2016-03-04 02:23:00
summary:    Resolução da lista de exercícios do livro _An Introduction to Parallel Programming_
categories: ufrn
thumbnail: laptop
teacher:    Samuel Xavier de Souza
semester: 2015.2
use_math_jax: true
tags:
 - programação paralela
 - threads
 - MPI
 - OpenMP
 - DCA
 - UFRN
 - solucionário
 - exercises solutions
 - solutions
 - Parallel Programing
 - Peter Pacheco
 - Samuel Xavier de Souza
---
Esta página tem como objetivo apresentar a resolução das listas de exercícios do livro ["An Introduction to Parallel Programming"][1]
de Peter Pacheco. A resolução dessas listas foi utilizada durante o curso de Programação Paralela no DCA / UFRN.

Esta pagina ainda está sendo atualizada.

[1]: https://www.cs.usfca.edu/~peter/ipp/
{%include seccionContent.html%}
####Chapter 02 - Parallel Hardware and Parallel Software
>__2.1__ When we were discussing floating point addition, we made the simplifying
         assumption that each of the functional units took the same amount of time.
         Suppose that fetch and store each take 2 nanoseconds and the remaining
         operations each take 1 nanosecond.

>__a.__ How long does a floating point addition take with these assumptions?

As sete operações que são realizadas para a soma em ponto flutuante são:
Buscar operandos, Comparar expoentes, Mover um operando, Adicionar, Normalizar o resultado, Arredondar o resultado, Armazenar o resultado.
Supondo que as operações de busca e amazenamento demorem 2 nanosegundos e o resto das operações demore 1 nanosegundo, o total de tempo decorrido seria:
2+1+1+1+1+1+2 = 9 nanosegundos.	

>__b.__ How long will an unpipelined addition of 1000 pairs of floats take with
        these assumptions?
        
Uma soma de 1000 float points feita normalmente e em sequência demoraria 
1000 vezes o tempo de uma única soma: 9*1000=9000 nanosegundos.
        
>__c.__ How long will a pipelined addition of 1000 pairs of floats take with these 
assumptions?

Para a operação em pipeline sobre as operações da questão __2.1 a.__,
 são divididas em sete unidades funcionais. As unidades funcionam em sequência e a saída de uma unidade funcional é a entrada da próxima. Após realizar a primeira adição, que dura 9 nanosegundos, é produzido 1 resultado a cada 2 nanosegundos. Portanto o tempo total de execução do loop é de 2*999+9 = 2007 nanosegundos.
        
>__d.__ The time required for fetch and store may vary considerably if the operands/
        results are stored in different levels of the memory hierarchy. Suppose that
        a fetch from a level 1 cache takes two nanoseconds, while a fetch from a
        level 2 cache takes five nanoseconds, and a fetch from main memory takes
        fifty nanoseconds. What happens to the pipeline when there is a level 1
        cache miss on a fetch of one of the operands? What happens when there
        is a level 2 miss?
        
A ordem em que os dados são buscados na cache é do nível mais rápido ao mais lento.

Quando ocorre um level 1 cache miss:<br>
Operação de busca =  2 nanosegundos para buscar na cache level 1 + 5 nanosegundos para buscar na cache level 2 = 7 nanosegundos

Quando ocorre um level 2 cache miss:<br>
Operação de busca = 2 nanosegundos para buscar na cache level 1 + 5 nanosegundos para buscar na cache level 2 + 50 nanosegundos para buscar na memória principal = 57 nanosegundos.
        
        
>__2.2.__ Explain how a queue, implemented in hardware in the CPU, could be used to
improve the performance of a write-through cache.

A fila fará com que as escritas mais recentes armazenadas na cache fiquem no topo.
 Isso facilita a escrita contínua a dados que são utilizados com mais frequência,
  melhorando o tráfego no barramento e sua utilização.
        
>__2.3.__ Recall the example involving cache reads of a two-dimensional array
        (page 22). How does a larger matrix and a larger cache affect the performance
        of the two pairs of nested loops? What happens if MAX = 8 and the cache can
        store four lines? How many misses occur in the reads of A in the first pair of
        nested loops? How many misses occur in the second pair?
        
Se apenas o tamanho da matriz for aumentado mais dados deverão ser carregados na cache pois provavelmente uma linha de cache não conseguirá mais armazenar uma linha inteira da matriz, resultando assim em uma execução mais lenta tanto do par de loops 1 quanto do par de loops 2. Se apenas o tamanho da cache for aumentado os loops executariam mais rápido devido à menor quantidade de cache miss (mais dados da matriz seriam carregados na cache). Porém a cache seria mais lenta, pois para cada cache miss uma quantidade maior de dados teria que ser carregada na cache.

Supondo MAX=8 e que a cache consegue armazenar 4 linhas com cada uma armazenando 4 elementos da matriz.
         O primeiro par de loops teria 16 cache misses, 2 para cada linha da matriz.
          O segundo par de loops teria 64 cache misses, pois mesmo com o aumento do número de 
          linhas que a cache pode armazenar, o segundo par de loops ainda teria 1 cache miss para cada 
          elemento lido da matriz.
          
![Imagem2.3]({{site.baseurl}}/assets/pcd/img2.3.png)

>__2.4.__ In Table 2.2, virtual addresses consist of a byte offset of 12 bits and a virtual
page number of 20 bits. How many pages can a program have if it’s run on a
system with this page size and this virtual address size?

Se temos um espaço de endereços virtual de N bits (neste caso, N=32), 
e temos um tamanho de página de 2^M (neste caso, M=12), então os primeiros N-M bits 
(neste caso, os primeiros 32-12 = 20 bits) compõem o número de páginas virtuais 
que que identificam unicamente cada página no espaço de endereços virtual. 
Os M bits nos dizem qual byte da página é o byte que está procurando, e é chamado de _offset_. 
Portanto o programa poderá ter até 2^20 = 1.048.576 páginas.

>__2.5.__ Does the addition of cache and virtual memory to a von Neumann system
change its designation as an SISD system? What about the addition of
pipelining? Multiple issue? Hardware multithreading?

Não. A adição de cache e memória virtual foram tentativas de superar o von Neumann bottleneck (gargalo), 
isto é, tentar evitar ao máximo o acesso a memória, visto que este acesso reduz a performance do sistema 
por ser mais custoso. Isto não muda sua característica de SISD.

Não. Pipelining pode ser claramente considerado hardware paralelo, já que unidades funcionais são replicadas. 
No entanto, esta forma de paralelismo não é comumente visível ao programador. 
De acordo com o livro-texto, hardware paralelo será limitado ao hardware que é visível ao programador, 
portanto pipelining é tratado como uma extensão do modelo básico de von Neumann.

Não. O mesmo conceito aplicado em pipelining também é aplicado em multiple issue.

Não. Hardware multithreading apenas providencia um meio para os sistemas continuarem executando 
trabalho útil quando uma task que está sendo executada for interrompida, por exemplo, se a task atual 
precisa esperar que dados sejam carregados da memória. Ao invés de procurar paralelismo na thread sendo executada,
 o sistema simplesmente carrega outra thread. Sistemas com hardware multithreading suportam switchings 
 muito rápidos entre threads para que isso seja possível, mas isto não modifica a característica SISD do sistema.

>__<div class='text-danger'>2.6.</div>__

<p></p>

>__2.7.__ Discuss the differences in how a GPU and a vector processor might execute
the following code: ![Imagem2.3]({{site.baseurl}}/assets/pcd/img2.7.png)

Vector Processor: Caso o tamanho do array seja igual ao tamanho do vector processor todo esse 
loop será executado em um único ciclo de load, add e store. Caso o tamanho do array seja maior, 
serão necessários mais ciclos para executar todo este bloco.

GPU: A GPU funciona de forma similar ao vector processor. 
A principal diferença é o overhead que o sistema terá para enviar os dados para a GPU e depois para coletá-los.

>__2.8.__ Explain why the performance of a hardware multithreaded processing core
might degrade if it had large caches and it ran many threads.

Múltiplas threads podem interferir umas nas outras quando compartilham recursos de hardware como caches ou translation 
lookaside buffers (TLB). Como resultado, tempos de execução de uma única thread não são melhorados mas degradados, mesmo 
quando apenas uma thread está executando, devido a baixas frequências ou estágios adicionais de pipelining que são necessários 
para acomodar thread-switching hardware.

>__2.9.__ In our discussion of parallel hardware, we used Flynn’s taxonomy to identify
three types of parallel systems: SISD, SIMD, and MIMD. None of our systems
were identified as multiple instruction, single data, or MISD. How would an
MISD system work? Give an example.

MISD (multiple instruction, single dada) é um tipo de arquitetura de computação paralela onde muitas 
unidades funcionais realizam diferentes operações no mesmo dado. Não existem muitas instâncias dessa arquitetura, 
já que MIMD e SIMD são frequentemente mais apropriadas para técnicas comuns de computação paralela. 
Especificamente, elas permitem melhor escalabilidade e uso de recursos computacionais do que a MISD. No entanto, 
um exemplo proeminente de computação MISD são os computadores de controle de vôo Space Shuttle. Fonte: Wikipedia.