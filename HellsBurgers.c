
#include <stdio.h>      // libreria estandar
#include <stdlib.h>     // para usar exit y funciones de la libreria standard
#include <string.h>
#include <pthread.h>    // para usar threads
#include <semaphore.h>  // para usar semaforos
#include <unistd.h>


#define LIMITE 50

//pthread_mutex_t mutex;

//creo estructura de semaforos 
struct semaforos {
    sem_t sem_cortar;
    sem_t sem_mezclar;
    sem_t sem_salar;
    sem_t sem_armar_medallones;
    sem_t sem_cocinar_medallones;
    sem_t sem_hornear_pan;
    sem_t sem_cortar_extras;
    sem_t sem_armar_hamburguesas;
    sem_t sem_ganador
	//poner demas semaforos aqui
};

//creo los pasos con los ingredientes
struct paso {
   char accion [LIMITE];
   char ingredientes[4][LIMITE];
   
};

//creo los parametros de los hilos 
struct parametro {
 int equipo_param;
 struct semaforos semaforos_param;
 struct paso pasos_param[8];
};

//funcion para imprimir las acciones y los ingredientes de la accion
void* imprimirAccion(void *data, char *accionIn) {
       
	struct parametro *mydata = data;
	//calculo la longitud del array de pasos 
	int sizeArray = (int)( sizeof(mydata->pasos_param) / sizeof(mydata->pasos_param[0]));
	//indice para recorrer array de pasos 
	int i;
	for(i = 0; i < sizeArray; i ++){
		//pregunto si la accion del array es igual a la pasada por parametro (si es igual la funcion strcmp devuelve cero)
		if(strcmp(mydata->pasos_param[i].accion, accionIn) == 0){
		printf("\tEquipo %d - accion %s \n " , mydata->equipo_param, mydata->pasos_param[i].accion);
		//calculo la longitud del array de ingredientes
		int sizeArrayIngredientes = (int)( sizeof(mydata->pasos_param[i].ingredientes) / sizeof(mydata->pasos_param[i].ingredientes[0]) );
		//indice para recorrer array de ingredientes
		int h;
		printf("\tEquipo %d -----------ingredientes : ----------\n",mydata->equipo_param); 
			for(h = 0; h < sizeArrayIngredientes; h++) {
				//consulto si la posicion tiene valor porque no se cuantos ingredientes tengo por accion 
				if(strlen(mydata->pasos_param[i].ingredientes[h]) != 0) {
							printf("\tEquipo %d ingrediente  %d : %s \n",mydata->equipo_param,h,mydata->pasos_param[i].ingredientes[h]);
				}
			}
		}
	}
}
pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex1=PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_pan=PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex0=PTHREAD_MUTEX_INITIALIZER;

//funcion para tomar de ejemplo
void* cortar(void *data) {
	//creo el nombre de la accion de la funcion 
	char *accion = "cortar";
	//creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero). 
	struct parametro *mydata = data;
	//llamo a la funcion imprimir le paso el struct y la accion de la funcion
	imprimirAccion(mydata,accion);
	//uso sleep para simular que pasa tiempo
	usleep( 1000000 );
	//doy la señal a la siguiente accion (cortar me habilita mezclar)
    sem_post(&mydata->semaforos_param.sem_mezclar);
  
    pthread_exit(NULL);
}
void* mezclar(void *data){
        struct parametro *mydata = data;
        sem_wait(&mydata->semaforos_param.sem_mezclar);
        char *accion = "mezclar";
       // struct parametro *mydata = data;
        imprimirAccion(mydata,accion);
        usleep(2000000);
        sem_post(&mydata->semaforos_param.sem_salar);
        pthread_exit(NULL);
 
}
void* salar(void *data){
       pthread_mutex_lock(&mutex);
       struct parametro *mydata = data;
       sem_wait(&mydata->semaforos_param.sem_salar);
       char *accion = "salar";
       imprimirAccion(mydata,accion);
       usleep(3000000);
       sem_post(&mydata->semaforos_param.sem_armar_medallones);
      // pthread_exit(NULL);
       pthread_mutex_unlock(&mutex);
       pthread_exit(NULL);
}

void* armar_medallones(void *data){
       struct parametro *mydata = data;
       sem_wait(&mydata->semaforos_param.sem_armar_medallones);
       char *accion = "armar_medallones";
       imprimirAccion(mydata,accion);
       usleep(2000000);
       sem_post(&mydata->semaforos_param.sem_cocinar_medallones);
       pthread_exit(NULL);
}
void* cocinar_medallones(void *data){
       pthread_mutex_lock(&mutex1);
       struct parametro *mydata = data;
       sem_wait(&mydata->semaforos_param.sem_cocinar_medallones);
       char *accion = "cocinar_medallones";
       imprimirAccion(mydata,accion);
       usleep(2000000);
       sem_post(&mydata->semaforos_param.sem_hornear_pan);
       pthread_mutex_unlock(&mutex1);
       pthread_exit(NULL);
}
void* hornear_pan(void *data){
      pthread_mutex_lock(&mutex_pan);
      struct parametro *mydata = data;
      sem_wait(&mydata->semaforos_param.sem_hornear_pan);
      char *accion ="hornear_pan";
      imprimirAccion(mydata,accion);
      usleep(2000000);
      //sem_post(&mydata->semaforos_param.sem_armar_hamburguesas);
      sem_post(&mydata->semaforos_param.sem_cortar_extras);
      pthread_mutex_unlock(&mutex_pan);
      pthread_exit(NULL);
}
void* cortar_extras(void *data){
      struct parametro *mydata = data;
      sem_wait(&mydata->semaforos_param.sem_cortar_extras);
      char *accion = "cortar_extras";
      imprimirAccion(mydata,accion);
      usleep(2000000);
      sem_post(&mydata->semaforos_param.sem_armar_hamburguesas);
      pthread_exit(NULL);
}
void* armar_hamburguesas(void *data){
      struct parametro *mydata = data;
      sem_wait(&mydata->semaforos_param.sem_armar_hamburguesas);
      char *accion = "armar_hamburguesas";
      imprimirAccion(mydata,accion);
      sem_post(&mydata->semaforos_param.sem_ganador);
      usleep(2000000);
      pthread_exit(NULL);
}
void* ganador(void *data){
       struct parametro *mydata = data; 
       sem_wait(&mydata->semaforos_param.sem_ganador); 
       printf("El ganador es:\tEquipo %d \n",mydata->equipo_param); 
       exit(-1);
       pthread_exit(NULL); 

}

void* ejecutarReceta(void *i) {

	
	//variables semaforos
	sem_t sem_mezclar;
        sem_t sem_cortar;
        sem_t sem_salar;
        sem_t sem_armar_medallones;
        sem_t sem_cocinar_medallones;
        sem_t sem_hornear_pan;
        sem_t sem_cortar_extras;
        sem_t sem_armar_hamburguesas;
        sem_t sem_ganador;
	//crear variables semaforos aqui
	
	//variables hilos
	pthread_t p1;
        pthread_t p2;
        pthread_t p3;
        pthread_t p4; 
        pthread_t p5; 
        pthread_t p6; 
        pthread_t p7; 
        pthread_t p8; 
    
	//crear variables hilos aqui
	
	//numero del equipo (casteo el puntero a un int)
	int p = *((int *) i);
	
	printf("Ejecutando equipo %d \n", p);

	//reservo memoria para el struct
	struct parametro *pthread_data = malloc(sizeof(struct parametro));

	//seteo los valores al struct
	
	//seteo numero de grupo
	pthread_data->equipo_param = p;

	//seteo semaforos
	pthread_data->semaforos_param.sem_mezclar = sem_mezclar;
	//setear demas semaforos al struct aqui
        pthread_data->semaforos_param.sem_cortar = sem_cortar;
        pthread_data->semaforos_param.sem_salar = sem_salar;
        pthread_data->semaforos_param.sem_armar_medallones = sem_armar_medallones;
        pthread_data->semaforos_param.sem_cocinar_medallones = sem_cocinar_medallones;
	pthread_data->semaforos_param.sem_hornear_pan = sem_hornear_pan;
	pthread_data->semaforos_param.sem_cortar_extras = sem_cortar_extras;
	pthread_data->semaforos_param.sem_armar_hamburguesas = sem_armar_hamburguesas;
	pthread_data->semaforos_param.sem_ganador = sem_ganador;

	//seteo las acciones y los ingredientes (Faltan acciones e ingredientes) ¿Se ve hardcodeado no? ¿Les parece bien?


     	strcpy(pthread_data->pasos_param[0].accion, "cortar");
	strcpy(pthread_data->pasos_param[0].ingredientes[0], "ajo");
        strcpy(pthread_data->pasos_param[0].ingredientes[1], "perejil");
 	strcpy(pthread_data->pasos_param[0].ingredientes[2], "cebolla");

	strcpy(pthread_data->pasos_param[1].accion, "mezclar");
	strcpy(pthread_data->pasos_param[1].ingredientes[0], "ajo");
        strcpy(pthread_data->pasos_param[1].ingredientes[1], "perejil");
 	strcpy(pthread_data->pasos_param[1].ingredientes[2], "cebolla");
	strcpy(pthread_data->pasos_param[1].ingredientes[3], "carne picada");


	strcpy(pthread_data->pasos_param[2].accion, "salar");
	strcpy(pthread_data->pasos_param[2].ingredientes[0], "ajo");
	strcpy(pthread_data->pasos_param[2].ingredientes[1], "perejil");
	strcpy(pthread_data->pasos_param[2].ingredientes[2], "cebolla");
	strcpy(pthread_data->pasos_param[2].ingredientes[3], "carne picada");

        strcpy(pthread_data->pasos_param[3].accion, "armar_medallones");
        strcpy(pthread_data->pasos_param[3].ingredientes[0], "ajo");
	strcpy(pthread_data->pasos_param[3].ingredientes[1], "perejil");
	strcpy(pthread_data->pasos_param[3].ingredientes[2], "cebolla");
	strcpy(pthread_data->pasos_param[3].ingredientes[3], "carne picada");
 
        strcpy(pthread_data->pasos_param[4].accion, "cocinar_medallones");
	strcpy(pthread_data->pasos_param[4].ingredientes[0], "ajo");
	strcpy(pthread_data->pasos_param[4].ingredientes[1], "perejil");
	strcpy(pthread_data->pasos_param[4].ingredientes[2], "cebolla");
	strcpy(pthread_data->pasos_param[4].ingredientes[3], "carne picada");

        strcpy(pthread_data->pasos_param[5].accion, "hornear_pan");
        strcpy(pthread_data->pasos_param[6].accion, "cortar_extras");
	strcpy(pthread_data->pasos_param[6].ingredientes[0], "lechuga");
	strcpy(pthread_data->pasos_param[6].ingredientes[1], "tomate");

        strcpy(pthread_data->pasos_param[7].accion, "armar_hamburguesas");
  	strcpy(pthread_data->pasos_param[7].ingredientes[0], "medallon");
	strcpy(pthread_data->pasos_param[7].ingredientes[1], "lechuga");
	strcpy(pthread_data->pasos_param[7].ingredientes[2], "tomate");	
	strcpy(pthread_data->pasos_param[7].ingredientes[3], "pan");


        

	
	//inicializo los semaforos
	sem_init(&(pthread_data->semaforos_param.sem_mezclar),0,0);
	//inicializar demas semaforos aqui
        sem_init(&(pthread_data->semaforos_param.sem_cortar),0,0);
        sem_init(&(pthread_data->semaforos_param.sem_salar),0,0);
        sem_init(&(pthread_data->semaforos_param.sem_armar_medallones),0,0);
        sem_init(&(pthread_data->semaforos_param.sem_cocinar_medallones),0,0);
        sem_init(&(pthread_data->semaforos_param.sem_hornear_pan),0,0);
        sem_init(&(pthread_data->semaforos_param.sem_cortar_extras),0,0);
        sem_init(&(pthread_data->semaforos_param.sem_armar_hamburguesas),0,0);
        sem_init(&(pthread_data->semaforos_param.sem_ganador),0,0);


	//creo los hilos a todos les paso el struct creado (el mismo a todos los hilos) ya que todos comparten los semaforos 
    int rc;
    rc = pthread_create(&p1,                           //identificador unico
                            NULL,                          //atributos del thread
                                cortar,             //funcion a ejecutar
                                pthread_data);                     //parametros de la funcion a ejecutar, pasado por referencia
	//crear demas hilos aqui
    rc = pthread_create(&p1,NULL,mezclar,pthread_data);	
    rc = pthread_create(&p2,NULL,salar,pthread_data);
    rc = pthread_create(&p3,NULL,armar_medallones,pthread_data);
    rc = pthread_create(&p4,NULL,cocinar_medallones,pthread_data);	
    rc = pthread_create(&p5,NULL,hornear_pan,pthread_data);	
    rc = pthread_create(&p6,NULL,cortar_extras,pthread_data);	
    rc = pthread_create(&p7,NULL,armar_hamburguesas,pthread_data);	
    rc = pthread_create(&p8,NULL,ganador,pthread_data);	




	//join de todos los hilos
	pthread_join (p1,NULL);
	//crear join de demas hilos
        pthread_join (p2,NULL);
        pthread_join (p3,NULL);
        pthread_join (p4,NULL);
        pthread_join (p5,NULL);
        pthread_join (p6,NULL);
        pthread_join (p7,NULL);
        pthread_join (p8,NULL);

       


	//valido que el hilo se alla creado bien 
    if (rc){
       printf("Error:unable to create thread, %d \n", rc);
       exit(-1);
     }

	 
	//destruccion de los semaforos 
	sem_destroy(&sem_mezclar);
	//destruir demas semaforos 
        sem_destroy(&sem_cortar);
        sem_destroy(&sem_salar);
        sem_destroy(&sem_armar_medallones);
        sem_destroy(&sem_cocinar_medallones);
        sem_destroy(&sem_hornear_pan);
        sem_destroy(&sem_cortar_extras);
        sem_destroy(&sem_armar_hamburguesas);
        sem_destroy(&sem_ganador);
	
	//salida del hilo
	 pthread_exit(NULL);
}


int main ()
{
       

	//creo los nombres de los equipos 
	int rc;
	int *equipoNombre1 =malloc(sizeof(*equipoNombre1));
	int *equipoNombre2 =malloc(sizeof(*equipoNombre2));
	int *equipoNombre3 =malloc(sizeof(*equipoNombre3));
	*equipoNombre1 = 1;
	*equipoNombre2 = 2;
	*equipoNombre3 = 3;

	//creo las variables los hilos de los equipos
	pthread_t equipo1; 
	pthread_t equipo2;
	pthread_t equipo3;
	//inicializo los hilos de los equipos
    rc = pthread_create(&equipo1,                           //identificador unico
                            NULL,                          //atributos del thread
                                ejecutarReceta,             //funcion a ejecutar
                                equipoNombre1); 

    rc = pthread_create(&equipo2,                           //identificador unico
                            NULL,                          //atributos del thread
                                ejecutarReceta,             //funcion a ejecutar
                                equipoNombre2);

    rc = pthread_create(&equipo3,                           //identificador unico
                            NULL,                          //atributos del thread
                                ejecutarReceta,             //funcion a ejecutar
                                equipoNombre3);

   if (rc){
       printf("Error:unable to create thread, %d \n", rc);
       exit(-1);
     } 

	//join de todos los hilos
	pthread_join (equipo1,NULL);
	pthread_join (equipo2,NULL);
	pthread_join (equipo3,NULL);


    pthread_exit(NULL);
}


//Para compilar:   gcc HellsBurgers.c -o ejecutable -lpthread
//Para ejecutar:   ./ejecutable
