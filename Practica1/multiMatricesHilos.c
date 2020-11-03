
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include<time.h>
#include<pthread.h>
#include <string.h>
#include <time.h> 
 


// estructura  para las matrices
typedef struct Matrix
{
    /* data */
    int n;//filas
    int m;//columnas
    int ** contenido; //apuntador a la matirz
    char  nombre; //nombre de la matriz
}Matrix;


//estructura para los parametros  a los hilos
typedef struct dato{ //parametros  para el hilo
    int initFila;
    int noFilas;
    int analizado;   
} ParametroComplejo ;

//crea la memoria  decaurdo a los valores que se le indiqeu en n y m
// y se le  asigna un numerio deacurdeo al numero 
int** crearMemoria(Matrix x){
    int** content;
	content = (int**) malloc(x.n*sizeof(int*));	
	for(int i = 0; i < x.n; i ++){
		content[i] = (int*) malloc(x.m*sizeof(int));
        for(int j=0; j<x.m; j++){
            content[i][j]=rand() % (5 + 1); //0-5
        }  
    }
    return content;
}

//crear memoria llena de 0 para la matriz resultados
int** crearMemoriaVacia(Matrix x){
    int** content;
	content = (int**) malloc(x.n*sizeof(int*));	
	for(int i = 0; i < x.n; i ++){
		content[i] = (int*) malloc(x.m*sizeof(int));
         for(int j=0; j<x.m; j++){
            content[i][j]=0;
        }   
    }
    return content;
}


// muestra el nombre, datos  y contenido de una matriz 
void mostrarMatrix(Matrix* matrix){
    printf("\nMostrando matriz %c (%d x %d) : \n",matrix->nombre, matrix->n, matrix->m);
	int i, j;
	for(i = 0; i < matrix -> n; i++){
		for(j = 0; j < matrix -> m; j++)
			printf(" %d,", matrix -> contenido[i][j]);
		printf("\n");
	}
}
 // matrices a ocupar
Matrix matrixA;
Matrix matrixB;
Matrix matrixC;


// pide y crea los datos para una matriz
void leerMatriz(Matrix* m,char nombre){
    m->nombre=nombre; 
    printf("\n        Leyendo la matriz: %c \n",m->nombre);
    printf("ingrese numero de filas:");
    scanf("%d", &(m -> n));

    printf("ingrese numero de columnas:");
    scanf("%d", &(m -> m));

     m->contenido= crearMemoria(*m);
}

// funcion que ejecutara cada hilo donde se le pasa las matrices y el numero de hilos
void * thread_routine(void * arg){

   ParametroComplejo *argComplejo=  (  (ParametroComplejo *) arg  ); // convertimos el parametro a tipo ParametroComplejo *

    if(argComplejo->noFilas==0){ // no hoy filas para analizar salir 
             argComplejo->analizado=1;   
            pthread_exit((void *) argComplejo);            
    }


//realizar la multiplicacion para los renglones asignados

   int filaActual = argComplejo->initFila;
   int noFilas= argComplejo->noFilas;


   for(int h=0;h<noFilas; h++){ // para cada una de las filas a analizar
    for (int i = 0; i < matrixA.m ; i++){      
        for (int j = 0; j < matrixB.m ; j++){
            matrixC.contenido[ filaActual ][j ] +=matrixA.contenido[ filaActual ][i] *matrixB.contenido[ i ][j];
        }
    }
    filaActual++; // pasar  a la sigueinte fila a analizar
   }
   argComplejo->analizado=1;   // marcar que este hilo za hizo su trabajo
   pthread_exit((void *) argComplejo); // retornar el parametro

}



int main(int argc, char *argv[]){
    int noHilos;  // se le asignara el numero de hilos a crear
    int noFilas4Hilo; // cuantas filas tendra cada hilo
    int restantes=0; //  residuo de filas, se asignaran uno a  los primeros hillos
    pthread_t  *threadsArr; // apuntador a los hilos
    ParametroComplejo* paramArray;   // apuntador a lso parametros para cada hilo
    long double  t0, t1; // contadores para el tiempo de ejecucion


    t0=clock(); // empesar a contar el tiempo
    leerMatriz(&matrixA,'A'); //creae las matrices
    leerMatriz(&matrixB,'B'); 

    mostrarMatrix(&matrixA); // mostrar las matrices
    mostrarMatrix(&matrixB);

    if(matrixA.m == matrixB.n){ //verificando  si es posible hacer la multiplicacion

    //     //creando la matriz C
        matrixC.nombre='C';
        
        matrixC.n= matrixA.n;
        matrixC.m=matrixB.m;
        matrixC.contenido = crearMemoriaVacia(matrixC);

        // preguntar el numero de hilos 
        printf("Ingrese el numero de hilos:");
        scanf("%d",&noHilos);
        //repartir el numero de filas 

       //   asignar numero  de filas por hilo

       noFilas4Hilo =   matrixC.n / noHilos;
        restantes =  matrixC.n % noHilos; // filas restantes

        threadsArr = (pthread_t*) malloc(noHilos*sizeof(pthread_t)); // creae el espacio de memoria para  los apuntadores a  hilos
        paramArray = (ParametroComplejo *) malloc( noHilos* sizeof(ParametroComplejo)  ); // creae el espacio de memoria para los  parametros a los hilos       

        for(int i=0; i<noHilos;i++){
            // AConfigurar parametro

           
            if(noFilas4Hilo >0 ){ //si hay mas o igual filas que hilos
                paramArray[i].initFila=noFilas4Hilo *i; //donde inicia a hacer la fila
                paramArray[i].noFilas=noFilas4Hilo; //cuantas filas realizar
                //si hay acarreo agregar uno por hilo hasta que se acaben los sobrantes
                if(restantes>0  ){ //si hay residuos


                    if (i<restantes) // asignar uno a los primeros hilos
                    {
                         paramArray[i].noFilas++; 
                         paramArray[i].initFila+=i;
                    }else //recorrer los restantes  hilos con los hilos que se asignaron
                    {
                         paramArray[i].initFila+=restantes;
                    }                   

                }


            }else{ // si haymas hilos  que  filas
              //  printf("\n mas filas que hilos \n");
                if(i<matrixC.n){  //los que alcanzaron
                    paramArray[i].initFila=i; //donde inicia a hacer la fila
                    paramArray[i].noFilas=1;   
                }else{
                      paramArray[i].initFila=0; //donde inicia a hacer la fila
                      paramArray[i].noFilas=0;   
                }
            }
            
            paramArray[i].analizado=0; // no se ha analizado
            if( 0 != pthread_create(&threadsArr[i], NULL, thread_routine, &(paramArray[i]) ) ){  //lanzar los hilos
             printf("no se pudo crear hilo \n");
              return -1;
            }

        }
        for (int i = 0; i < noHilos; i++)  // esperar por los  hilos lanzados
        {
                ParametroComplejo *salida; // resibir los resultados
                pthread_join(threadsArr[i], (void **) &salida  );
                //printf("salida : %d  - %d numero \n",salida->analizado, salida->initFila );
        }   

        mostrarMatrix(&matrixC);    // mostrar resuldao de la  multiplicacion
    }else{
        printf("No se peude multiplicar");
        printf("\n");

    }
    t1=clock(); // calcular el tiempo en ejecucion
    long double time = (long double) ((t1-t0)/CLOCKS_PER_SEC); 
    printf("\n tiempo de ejecucion %Lf \n",time);

}


