## MPI

### Lista
**1.** Escreva um programa que transmita um dado do processo zero para todos os outros processos, da seguinte maneira: o processo i deve receber o dado do processo i-1, somar seu número de identificação e enviar o dado ao processo i+1, até que o último processo seja atingido.
Assuma que o dado consiste de um número inteiro. O processo zero deve ler o dado especificado pelo usuário na linha de comando. Todos os processos devem enviar o valor calculado para o processo zero. Assim, o processo zero deve escrever no arquivo de saída o número de identificação do processo e o valor por ele calculado. Por exemplo, suponha que o valor passado inicialmente é igual a 12, o arquivo de saída seria:
Identificação = 0 valor 12
Identificação = 1 valor 13 
Identificação = 2 valor 15 
Identificação = 3 valor 18 e assim por diante. 

2.
