---
layout:     project
title:      Programação Concorrente e Distribuída (4)
subject:    Programação Concorrente e Distribuída
date:       2016-03-31 21:20:00
summary:    Resolução da lista de exercícios do livro _An Introduction to Parallel Programming_
categories: ufrn
chapter:    Chapter 04 - Shared-Memory Programming with Pthreads
thumbnail:  laptop
teacher:    Samuel Xavier de Souza
semester:   2015.2
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
 - manual solutions
 - Parallel Programing
 - Peter Pacheco
 - Samuel Xavier de Souza
---
Esta página tem como objetivo apresentar a resolução dos exercícios do livro ["An Introduction to Parallel Programming"][1]
de Peter Pacheco. A resolução dessas listas foi utilizada durante o curso de Programação Paralela no DCA / UFRN.

Respostas dos capítulos:

{% assign cap1 = site.projects | where: "id", "/projects/programacao-concorrente" | first %}
- [{{cap1.chapter}}]({{site.baseurl}}{{cap1.url}})
{% for post in site.programacao-concorrente %}
{% if post != page %}
- [{{post.chapter}}]({{site.baseurl}}{{post.url}})
{% else %}
- {{post.chapter}}
{% endif %}
{% endfor %}

[1]: https://www.cs.usfca.edu/~peter/ipp/
{%include seccionContent.html%}
####{{page.chapter}}

>__4.1.__ When we discussed matrix-vector multiplication we assumed that 
both m and n, the number of rows and the number of columns, respectively, 
were evenly divisible by t, the number of threads. How do the formulas for 
the assignments change if this is not the case?

Para que o cálculo também funcionasse para um número de linhas 
não divisível pelo número de threads (m % thread_count != 0), o resto dessa 
divisão foi distribuído entre as threads com my_rank menor que o resto. 
Além disso, as threads que não tiveram o resto distribuído, tiveram que fazer um deslocamento 
igual ao resto para compensar os local_m+1 das threads com resto distribuído. 
Código da função executada pelas threads e modificada do arquivo “pth_mat_vect.c” fornecido pelo livro 
texto está a seguir. O arquivo modificado pode ser baixado [aqui]({{site.baseurl}}/assets/codes-copyright/pth_mat_vect_4.1.c).

```
/*------------------------------------------------------------------
 * Function:       Pth_mat_vect
 * Purpose:        Multiply an mxn matrix by an nx1 column vector
 * In arg:         rank
 * Global in vars: A, x, m, n, thread_count
 * Global out var: y
 */
void *Pth_mat_vect(void* rank) {
   long my_rank = (long) rank;
   int i, j;
   int local_m = m/thread_count;
   int my_first_row;
   int my_last_row;
   if(my_rank < (m % thread_count)){	//Se minha thread atual tem um
      // my_rank menor que o resto da divisao de m pelo thread_count, então ela recebe o resto da divisão
	local_m++;
	my_first_row = my_rank*local_m;
        my_last_row = (my_rank+1)*local_m - 1;
   }else{ //Para as outras threads, adicionar o resto para compensar os local_m+1 das outras threads
        my_first_row = my_rank*local_m + (m % thread_count);
        my_last_row = (my_rank+1)*local_m - 1 + (m % thread_count);
   } 

   for (i = my_first_row; i <= my_last_row; i++) {
      y[i] = 0.0;
      for (j = 0; j < n; j++)
          y[i] += A[i*n+j]*x[j];
   }

   return NULL;
}  /* Pth_mat_vect */
```

>__4.2.__ If we decide to physically divide a data structure among the threads, that is, 
if we decide to make various members local to individual threads, we need to consider at least three issues:<br>
>__a.__ How are the members of the data structure used by the individual threads?<br>
>__b.__ Where and how is the data structure initialized?<br>
>__c.__ Where and how is the data structure used after its members are computed?<br>

Para que as linhas da matriz A e os blocos de memória do vetor y fossem divididos, 
foi necessário criar um local_y e um local_A para cada thread. Quanto de memória é alocado para essas 
estruturas de dados, é definido no cálculo do local_m e local_n. São feitas as atribuições do A para o local_A de 
cada thread, então o cálculo só é realizado com os local_A e local_y. No final, o resultado das threads é lançado 
dos local_y para o y. Já em relação ao tempo de execução, essa estratégia foi mais demorada. Isso ocorre porque 
cada thread deve realizar uma copia dos seus local_y e local_A, fazendo com que haja uma perda de tempo ao realizar 
esse processo. No caso de utilizar uma variável global torna-se o processo mais rápido, pois além de não perder 
tempo copiando, há a possibilidade dos dados já estarem na cache. O código pode ser acesado por 
[aqui]({{site.baseurl}}/assets/codes-copyright/pth_mat_vect_distribuido_4_2.c).

```
/*------------------------------------------------------------------
 * Function:       Pth_mat_vect
 * Purpose:        Multiply an mxn matrix by an nx1 column vector
 * In arg:         rank
 * Global in vars: A, x, m, n, thread_count
 * Global out var: y
 */
void *Pth_mat_vect(void* rank) {
   long my_rank = (long) rank;
   int i, j;
   int local_m = m/thread_count; //Divisão das linhas de A
   int local_n = n/thread_count; //Divisão do vetor y
   int my_first_row;
   int my_last_row; 
   int my_first_column;
   int my_last_column;
   double* local_y; //Vetor produto local
   double* local_A; //Matriz A local
      
   //Distribuição das linhas da matriz A
   if(my_rank < (m % thread_count)){	//Se minha thread atual
      // tem um my_rank menor que o resto da divisao de m pelo thread_count,
      // então ela recebe o resto da divisão
	local_m++;
	my_first_row = my_rank*local_m;
        my_last_row = (my_rank+1)*local_m - 1;
   }else{
      //Para as outras threads, adicionar o resto para compensaros local_m+1 das outras threads
        my_first_row = my_rank*local_m + (m % thread_count);
        my_last_row = (my_rank+1)*local_m - 1 + (m % thread_count);
   }

   //Distribuição do vetor y
   if(my_rank < (n % thread_count)){
      //Se minha thread atual tem um my_rank menor que
      // o resto da divisao de n pelo thread_count, então ela recebe o resto da divisão
	local_n++;
	my_first_column = my_rank*local_n;
        my_last_column = (my_rank+1)*local_n - 1;
   }else{ //Para as outras threads, adicionar o resto para compensar os local_m+1 das outras threads
        my_first_column = my_rank*local_n + (n % thread_count);
        my_last_column = (my_rank+1)*local_n - 1 + (n % thread_count);
   }

   //Aloca memória
   local_A = malloc(local_m*n*sizeof(double));
   local_y = malloc(local_n*sizeof(double));

   //Realiza as atribuições do local_A
   for(i=0;i<local_m;i++){
      for (j = 0; j < n; j++){
	local_A[i*n + j] = A[my_first_row*n+j];
      }
      my_first_row++; 
   }

   //Realiza o cálculo local
   for (i = 0; i < local_m; i++) {
      local_y[i] = 0.0;
      for (j = 0; j < local_n; j++)
          local_y[i] += local_A[i*n+j]*x[j];
   }

   //Junta o resultado novamente
   for (i = 0; i < local_n; i++) {
	y[my_first_column] = local_y[i];
        my_first_column++;
   }

   return NULL;
}  /* Pth_mat_vect */
```

(Primeira execução é a nova implementação)

![Imagem4.2]({{site.baseurl}}/assets/pcd/img4.2.png)

><div class="text-danger"><strong>4.3</strong>

<p></p>

>__4.4.__ The performance of the \\(\pi\\) calculation program that uses mutexes 
remains roughly constant once we increase the number of threads beyond 
the number of available CPUs. What does this suggest about how the threads are scheduled on the 
available processors?

O tempo permanece constante pois as threads não vão estar executando em paralelo, 
mas sim, disputando para poderem ser executadas nas CPUs disponíveis. 
A melhor forma de escalonar essas tarefas seria distribuindo as threads igualmente 
para cada CPU e cada  thread executaria em uma quantidade fixa de tempo, sem necessidade 
de determinar prioridade entre elas.

>__4.5.__ Modify the mutex version of the \\(\pi\\) calculation program so that the critical section is 
in the for loop. How does the performance of this version compare to the performance of the original 
busy-wait version? How might we explain this?

[Mutex modificado]({{site.baseurl}}/assets/codes-copyright/pth_pi_mutex_modificado_4.5.c):

```
for (i = my_first_i; i < my_last_i; i++, factor = -factor) {
      pthread_mutex_lock(&mutex);
      sum += factor/(2*i+1);
      pthread_mutex_unlock(&mutex);
}
```

[Busy-waiting]({{site.baseurl}}/assets/codes-copyright/pth_pi_busy_4.5.c): 

```
for (i = my_first_i; i < my_last_i; i++, factor = -factor) {
      while (flag != my_rank);
      sum += factor/(2*i+1);  
      flag = (flag+1) % thread_count;
}
```

Busy:
![Imagem4.5]({{site.baseurl}}/assets/pcd/img4.5.png)

Mutex:
![Imagem4.5_a]({{site.baseurl}}/assets/pcd/img4.5_a.png)

A performance é similar no mutex modificado, já que é feita uma operação de 
lock e unlock em cada iteração das thread para cálculo do \\(\pi\\). 
Como a espera ocupada é bastante baixa, não há muito prejuízo em comparação ao mutex, 
que coloca a thread para o estado de espera.

>__4.6.__ Modify the mutex version of the calculation program so that it uses a 
semaphore instead of a mutex. How does the performance of this version compare with the mutex version?

[Código Implementado]({{site.baseurl}}/assets/codes-copyright/pth_pi_mutex_modificado_4.6.c)

Semáforo:

```
for (i = my_first_i; i < my_last_i; i++, factor = -factor) {
      sem_wait(&mutex);
      sum += factor/(2*i+1);
      sem_post(&mutex);
}
```

Mutex:
![Imagem4.6]({{site.baseurl}}/assets/pcd/img4.6.png)

Semáforo:
![Imagem4.6_a]({{site.baseurl}}/assets/pcd/img4.6_a.png)

A implementação com semáfoto obteve resultados parecidos com a implementação em mutex.
 Como somente uma thread pode executar a funcionalidade por vez, 
o desempenho torna-se parecido com o desempenho do mutex.

>__4.7.__ Although producer-consumer synchronization is easy to implement with semaphores,
it’s also possible to implement it with mutexes. The basic idea is to have the producer
and the consumer share a mutex. A flag variable that’s initialized to false by the main thread
indicates whether there’s anything to consume. With two threads we’d execute something like this:

