# aSATar
aSATar: The Last SATbender. A SAT (and sudoku) solver written in C++.

Este proyecto contiene la implementación de un SAT-Solver y un traductor de problemas de Sudoku a instancias de SAT. Además se incluye un script que prueba dicho solver con varias instancias de Sudoku y compara su desempeño con el reconocido solver zChaff.

## Implementación
Para la implementación del Solver se uso un algoritmo basado en ***DPLL*** iterativo usando las técnicas de ***Two-Watched Literals*** y ***Unit Propagation*** explicadas [aquí](https://www.cs.cornell.edu/gomes/pdf/2008_gomes_knowledge_satisfiability.pdf). Además se usó de guía el solver [MiniSAT](https://github.com/niklasso/minisat).

Las variables del problema son representadas con un entero y se posee un `struct Lit` que representa un literal del problema. Por cada variable ***p*** tenemos dos literales ***p*** y ***~p***. Dicha estructura literal almacena únicamente un entero de tal forma que si tenemos una variable ***p*** representada por un entero ***x***, su literal positivo ***p*** esta representado por el entero ***2x*** y su literal negativo ***~p*** esta representado por el entero ***2x + 1***. Esto permite realizar operaciones con variables y literales de forma eficiente con operaciones bit a bit. Las cláusulas son representadas por arreglos dínamicos de literales y para esto se utiliza el tipo `std::vector` de C++.

El solver contiene un arreglo de enteros con las asignaciones de las variables actualmente. Dicho arreglo es inicializado en ***-1*** en todas sus posiciones que implica que dicha variable no esta asignada. Si contiene el valor ***0*** implica que se le asignó ***false*** a dicha variable y si tiene el valor ***1*** se le asignó ***true***. Además posee un arreglo de Cláusulas que representa la instancia especifica del problema de SAT, y los primeros dos literales de cada cláusla son los observadores de dicha cláusula. 

Para la técnica de ***Two Watched Literals*** se mantiene un arreglo de listas doblemente enlazadas (`std::list` en C++), una para cada literal. Cada lista contiene un entero por cada cláusula que esta siendo observada, y ese entero corresponde al índice de la cláusula observada en el arreglo de cláusulas. Se escogió usar listas doblemente enlazadas ya que permiten insertar y borrar elementos en ***O(1)***, y adémas se pueden recorrer en ***O(n)*** siendo ***n*** el tamaño de la lista.

Para la técnica de ***Unit Propagation*** se cuenta con una pila (`std::stack`) de enteros que corresponden a cláusulas unitarias al momento. Esto permite asignar las variables pertinentes para satisfacer dichas clausulas unitarias y encontrar conflictos si existen. 

Al momento de asignar un valor a una variable, se debe manejar la lista de cláusulas observadas por el literal que se hace ***false*** despues de la asignación. Esto se hace en tiempo ***O(sum|C|)*** donde ***sum(|C|)*** es la suma de los tamaños de las cláusulas observadas por dicho literal ya que para cada cláusula se debe recorrer la cláusula en busca de un sustituto que se convierta en el nuevo observador de la clausula. Sin embargo esta busqueda es omitida si el otro obsevador evalua a ***true***. Si no se encuentra ningún observador sustituto, la cláusula se inserta en la pila de cláusulas unitarias. Además la ventaja de los observadores es que a la hora de hacer backtracking y deshacer una asignación, no añade ningún costo adicional porque no se puede dañar el invariante de los observadores, ya que ningún literal se hace falso.

Para realizar el algoritmo ***DPLL*** iterativo se siguió el esqueleto en el artículo mencionado anteriormente. Para realizar el backtracking se mantienen dos pilas de variables, una con las variables de decisión asignadas y otra que contiene a todas las variables asignadas (de decisión e implicadas). Las variables implicadas son aquellas que se definen en el proceso de ***Unit Propagation***. Cuando se desea hacer un backtracking se van deshaciendo las asignaciones de las variables en la pila que contiene a todas las variables asignada hasta que los topes de las pilas sean iguales. Esto se puede hacer siempre que haya una variable de decisión asignada ya que dicha variable se encuentra en ambas pilas. Si no existe dicha variable de decisión y se desea hacer un backtrack, significa que la instancia del problema es insatisfacible. Si existe dicha variable, cuando es su turno de ser deshacer su asignación, se le asigna el valor contrario al que tenía y se convierte en una variable implicada (no vuelve a entrar en la pila de decisión).

Por último, el solver contiene un *timer* para terminar la ejecución si se está tardando demasiado en llegar a un veredicto.

## Uso
Para el uso de nuestro programa será necesario tener C++ y Python3, así como la librería de python matplotlib, usada para la obtención de graficas. Para instalar esta librería se usa el comando:

	`python -m pip install -U matplotlib`

Luego de esto, para correr cada programa se usaran dos archivos bash que se encuentran en el directorio principal. `bash run_zchaff.sh` correrá el programa zchaff para la resolución del problema SAT, mientras que
`bash run_aSatar.sh` correrá la implementación propia de un resolvedor de problemas SAT. En ambos casos se debe pasar como segundo argumento la dirección del archivo con los sudokus que se pretenden resolver, y se le puede pasar como tercer argumento el tiempo en segundos que, en caso de que el programa no haya sido completado, detenga la corrida. Los resultados de ambas corridas quedarán en la carpeta `files`, en los archivos `zchaff.txt` y `aSatar.txt`, respectivamente. En la misma carpeta se encontrarán las imagenes `zchaff_plot.png` y `aSatar_plot.png`, que graficarán el tiempo tomado por el programa respectivo para cada distinto problema. Finalmente, dentro de la carpeta `files` se podrán encontrar otras dos carpetas `zchaff_sudokus` y `aSatar_sudokus` que tendrán dentro imagenes con cada uno de los sudokus resueltos por el programa respectivo.

En caso de querer hacer una corrida rápida, donde se usen ambos solvers y se obtenga una gráfica comparando ambos tiempos está la opción de usar `bash run_both.sh`, igualmente con la dirección del archivo entre comillas simples y opcionalmente el límite de tiempo. Esta corrida devolverá dentro de la carpeta `files` dos archivos, `zchaff.txt` y `aSatar.txt`, con los resultados y tiempos de cada uno de los programas, así como una imagen `times_plot.png` comparando los tiempos de ambas corridas. Este script no devuelve los sudokus representados como imagenes, con el objetivo de reducir el tiempo de corrida del script.

## Análisis

![Alt text](files/example_plot.png?raw=true)

Como podemos ver en la gráfica, aSATar resuelve la mayoría de las instancias en aproximadamente un microsegundo, mientras que zchaff tarda menos en la mayoría, existen algunas instancias en las que aSATar halla una solución más rápido que zchaff. Esto suponemos que es causado porque Zchaff implementa una serie de optimizaciones que conllevan un trade-off para hacer la corrida promedio más rápida, sin embargo como estamos lidiando con instancias muy específicas, la propagación unitaria de aSATar logra un muy buen trabajo y no tiene más estructuras que mantener, causando un mejor desempeño en algunas instancias. 

### Agradecimientos
Gracias a Vicmary por darnos este nombre tan malandro para el proyecto.
