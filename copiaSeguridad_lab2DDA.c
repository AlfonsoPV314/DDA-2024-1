#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int *crear_dado(int arriba, int frente){
  int *dado = (int *)malloc(6*sizeof(int));
  dado[0] = arriba;
  dado[1] = 0;
  dado[2] = frente;
  dado[3] = 7-frente;
  dado[4] = 0;
  dado[5] = 7-arriba;
  return dado;
}

int calcular_faltante(int *dado){
  int state; 
  for(int n=1;n<7;n++){
      state = 0;
      for(int m=0;m<6;m++){
          if (dado[m] == n){
              state = 1;
          }
      }
      if (state == 0){
          return n;
      }
  }
}

int *completar_dado(int *dado,int indicador){ // dado[1] = 0, dado[4] = 0. (Completar con los numeros faltantes)
  int F1 = calcular_faltante(dado);
  int F2 = abs(7-F1);

  if (indicador == 1){
      if (F1>F2){
          dado[4] = F1;
          dado[1] = F2;
      }else{
          dado[4] = F2;
          dado[1] = F1;
      }
  }else{
      if (F1>F2){
          dado[4] = F2;
          dado[1] = F1;
      }else{
          dado[4] = F1;
          dado[1] = F2;
      }
  }
  return dado;
}

int *iniciar_dado(int* dado){ //// [Ar,Izq,Fr,At,Der,Ab]
  int diferencia = abs(dado[0] - dado[2]);
  if ((dado[0] == 1 || dado[0] == 6) && (diferencia == 2 || diferencia == 4)){
      return completar_dado(dado,1);
  }
  if ((dado[0] == 2 || dado[0] == 5) && (diferencia == 2 || diferencia == 1)){
      return completar_dado(dado,1);
  }
  if ((dado[0] == 3 || dado[0] == 4) && (diferencia == 3 || diferencia == 1)){
      return completar_dado(dado,1);
  }
  return completar_dado(dado,0);
}

void imprimir_dado(int *dado){
  for (int n=0;n<6;n++){
      printf("%d ",dado[n]);
  }
}

//Función para realizar el movimiento del dado
void moverDado(int* dado, int mov){
  int aux = dado[0];  //aux simboliza la cara de arriba
  if (mov == 1){  //dado hacia arriba
    dado[0] = dado[2];  //arriba = frente
    dado[2] = dado[5];  //frente = abajo
    dado[5] = dado[3];  //abajo = atras
    dado[3] = aux;  //atras = arriba
  }
  else if (mov == 2){  //dado hacia la derecha
    dado[0] = dado[1];  //arriba = izquierda
    dado[1] = dado[5];  //izquierda = abajo
    dado[5] = dado[4];  //abajo = derecha
    dado[4] = aux;  //derecha = arriba
  }
  else if (mov == 3){  //dado hacia abajo
    dado[0] = dado[3];  //arriba = atras
    dado[3] = dado[5];  //atras = abajo
    dado[5] = dado[2];  //abajo = frente
    dado[2] = aux;  //frente = arriba
  }
  else{  //dado hacia la izquierda
    dado[0] = dado[4];  //arriba = derecha
    dado[4] = dado[5];  //derecha = abajo
    dado[5] = dado[1];  //abajo = izquierda
    dado[1] = aux;  //izquierda = arriba
  }
}

//Función para crear el tablero
int** crearTablero(int filas, int columnas){
    int** tablero = (int**)malloc(filas*sizeof(int*));
    for (int i=0; i<filas; i++){
        tablero[i] = (int*)malloc(columnas*sizeof(int));
    }
    return tablero;
}

//TDA PILA:
typedef struct nodo{
    int fila;
    int columna;
    struct nodo* sig;
}Nodo;

typedef struct pila{
    Nodo* top;
}Pila;

Nodo* crearNodo(int fila, int columna){
    Nodo* nodo=(Nodo*)malloc(sizeof(Nodo));
    nodo->sig = NULL;
    nodo->fila = fila;
    nodo->columna = columna;
    return nodo;
}

Pila* crearPila(){
    Pila* P = (Pila*)malloc(sizeof(Pila));
    P->top = NULL;
    return P;
}

int esVacia(Pila* P){
    Nodo* aux = P->top;
    if (aux == NULL){
        return 1;
    }else{
        return 0;
    }
}

void push(Pila* P, int fila, int columna){
    Nodo* aux = P->top;
    Nodo* nuevoNodo = crearNodo(fila, columna);
    nuevoNodo->sig = aux;
    P->top = nuevoNodo;
}

void pop(Pila* P){
    if (esVacia(P) != 1){
        Nodo* aux = P->top;
        P->top = aux->sig;
    }
}

void moverPila(Pila* P1, Pila* P2){
    if (esVacia(P1) != 1){
        Nodo* aux = P1->top;
        int val[2] = {aux->fila, aux->columna};
        while (aux != NULL){
            push(P2, val[0], val[1]);
            pop(P1);
            aux = P1->top;
            if (aux != NULL){
                val[0] = aux->fila;
                val[1] = aux->columna;
            }
        }
    }
}

//SOLO PARA PROBAR EL PROGRAMA: Función que printea la pila
void mostrarPila(Pila* P){
    Nodo* aux = P->top;
    int val[2];
    if (esVacia(P) == 1){
        printf("No se ha podido mostrar la pila ya que es vacia\n");
    }else{
        Pila* Paux = crearPila();
        while (aux != NULL){
            val[0] = aux->fila;
            val[1] = aux->columna;

            moverPila(P, Paux);
            aux = aux->sig;
        }
        aux = Paux->top;
        while (aux != NULL){
            val[0] = aux->fila;
            val[1] = aux->columna;
            printf("(%d, %d) ", val[0], val[1]);
            moverPila(Paux, P);
            aux = aux->sig;
        }
    }
}

void escribirArchivo(Pila* P, FILE* fp){
    if (fp == NULL){
        printf("Error al abrir el archivo (Error de output).\n");
        return;
    }
    Nodo* aux = P->top;
    int val[2];
    if (esVacia(P) == 1){
        fprintf(fp, "No hay solucion posible\n");
        return;
    }
    Pila* Paux = crearPila();
    while (aux != NULL){
        val[0] = aux->fila;
        val[1] = aux->columna;
        fprintf(fp, "(%d, %d) ", val[0], val[1]);
        moverPila(P, Paux);
        aux = aux->sig;
    }
    fprintf(fp, "\n");
    aux = Paux->top;
    while (aux != NULL){
        val[0] = aux->fila;
        val[1] = aux->columna;
        moverPila(Paux, P);
        aux = aux->sig;
    }
}
//TERMINO DE TDA PILA

//Funcion para validar si el movimiento es valido
int esValido(int x, int** tablero, int fila, int columna, int cantFilas, int cantCol, int* dado, Pila* camino, int val){

    //Verifica que no se este devolviendo
    int devolver=0;
    if (val){
        int filaAux=camino->top->fila;
        int colAux=camino->top->columna;
        Pila* aux=crearPila();
        push(aux, filaAux, colAux);
        pop(camino);
        if (camino->top->fila==fila && camino->top->columna){
            devolver=1;
        }
        push(camino, aux->top->fila, aux->top->columna);
        pop(aux);
        free(aux);
    }
    

    //NOTA: Si la casilla a la que se quiere mover el dado es una casilla vacia (es 0), no cumplirá la primera condición del segundo if de los casos válidos.
    
    if (x == 1 && fila-1 >= 0){
        if (devolver!=1 && (tablero[fila-1][columna] == dado[0] || tablero[fila-1][columna] == -1)){
            return 1;
        }
        return 0;
    }
    else if (x == 2 && columna+1 < cantCol){
        if (devolver!=1 &&(tablero[fila][columna+1] == dado[0] || tablero[fila][columna+1] == -1)){
            return 1;
        }
        return 0;
    }
    else if (x == 3 && fila+1 < cantFilas){
        if (devolver!=1 &&(tablero[fila+1][columna] == dado[0] || tablero[fila+1][columna] == -1)){
            return 1;
        }
        return 0;
    }
    else if (x == 4 && columna-1 >= 0){
        if (devolver!=1 &&(tablero[fila][columna-1] == dado[0] || tablero[fila][columna-1] == -1)){
            return 1;
        }
        return 0;
    }
    else{
        return 0;
    }
}



//FUNCION DE BACKTRACKING (Awebo.txt implementado): Realiza el camino y retorna la pila con los espacios visitados. En caso de que no haya solución, retorna una pila vacía.
Pila* realizarCamino(int* dado, int fila, int columna, int** tablero, int posIni[2], Pila* camino, int cantFilas, int cantCol, int val){
    if (fila == posIni[0] && columna == posIni[1]&&val){    //Si llega a la casilla de inicio, retorna la pila con los espacios visitados
        return camino;
    }
    int i = 1;
    while (i < 5){
        //TODO LO QUE NO DICE "esto si va" NO DEBE DE SER IMPLEMENTADO, es solo para probar el programa
        if (i == 1){    //hacia arriba
            if (esValido(i, tablero, fila, columna, cantFilas, cantCol, dado, camino, val) == 1){
                printf("La pila antes de moverse es: ");
                mostrarPila(camino);
                printf("\n");
                printf("El dado antes de moverse es: ");
                imprimir_dado(dado);
                printf("\n");
                printf("Me estoy moviendo hacia arriba\n");
                push(camino, fila-1, columna);    //esto si va
                moverDado(dado, i);    //esto si va
                printf("La pila después de moverse es: ");
                mostrarPila(camino);
                printf("\n");
                printf("El dado después de moverse es: ");
                imprimir_dado(dado);
                printf("\n\n");
                realizarCamino(dado, fila-1, columna, tablero, posIni, camino, cantFilas, cantCol,1); //esto si va
            }
        }
        else if (i == 2){    //hacia la derecha
            if (esValido(i, tablero, fila, columna, cantFilas, cantCol, dado, camino, val) == 1){
                printf("La pila antes de moverse es: ");
                mostrarPila(camino);
                printf("\n");
                printf("El dado antes de moverse es: ");
                imprimir_dado(dado);
                printf("\n");
                printf("Me estoy moviendo hacia la derecha\n");
                push(camino, fila, columna+1);    //esto si va
                moverDado(dado, i);    //esto si va
                printf("La pila después de moverse es: ");
                mostrarPila(camino);
                printf("\n");
                printf("El dado después de moverse es: ");
                imprimir_dado(dado);
                printf("\n\n");
                realizarCamino(dado, fila, columna+1, tablero, posIni, camino, cantFilas, cantCol,1); //esto si va
            }
        }
        else if (i == 3){    //hacia abajo
            if (esValido(i, tablero, fila, columna, cantFilas, cantCol, dado, camino, val) == 1){
                printf("La pila antes de moverse es: ");
                mostrarPila(camino);
                printf("\n");
                printf("El dado antes de moverse es: ");
                imprimir_dado(dado);
                printf("\n");
                printf("Me estoy moviendo hacia abajo\n");
                push(camino, fila+1, columna);    //esto si va
                moverDado(dado, i);    //esto si va
                printf("La pila después de moverse es: ");
                mostrarPila(camino);
                printf("\n");
                printf("El dado después de moverse es: ");
                imprimir_dado(dado);
                printf("\n\n");
                realizarCamino(dado, fila+1, columna, tablero, posIni, camino, cantFilas, cantCol,1); //esto si va
            }
        }
        else if (i == 4){    //hacia la izquierda
            if (esValido(i, tablero, fila, columna, cantFilas, cantCol, dado, camino, val) == 1){
                printf("La pila antes de moverse es: ");
                mostrarPila(camino);
                printf("\n");
                printf("El dado antes de moverse es: ");
                imprimir_dado(dado);
                printf("\n");
                printf("Me estoy moviendo hacia la izquierda\n");
                push(camino, fila, columna-1);    //esto si va
                moverDado(dado, i);    //esto si va
                printf("La pila después de moverse es: ");
                mostrarPila(camino);
                printf("\n");
                printf("El dado después de moverse es: ");
                imprimir_dado(dado);
                printf("\n\n");
                realizarCamino(dado, fila, columna-1, tablero, posIni, camino, cantFilas, cantCol,1); //esto si va
            }
        }
        else{    //i = 5: No hay caminos posibles para el camino elegido
            printf("No hay caminos posibles para el camino elegido\n");
            pop(camino);    //Vuelve a la casilla anterior
            printf("La pila después de moverse es: ");
            mostrarPila(camino);
            printf("\n");
            //ERROR: no actualiza el dado cuando encuentra un camino sin salida
            //IDEA: poner los dados en una pila, y que en este caso, si no hay caminos posibles, se saca el dado de la pila y se actualiza el dado con el top de la pila
            
            //ERROR 2: no confirma si se está devolviendo
            //IDEA: añadirle una condición al 2do if de la función esValido para ver si no se está moviendo a una posición anterior a la actual (hacer pop al top, guardar en variable auxiliar, y comparar el nuevo top con la posición a la que se quiere mover; luego, pushear la variable auxiliar al top de la pila de nuevo)
        }
        i++;
    }
    if (esVacia(camino) == 1){    //Si no hay caminos posibles, retorna una pila vacía
        return camino;
    }
}

//Función para leer el archivo: Esta función realiza todo el trabajo, llamando a las demás funciones
void leerArchivo(char* nombreArchivo){
  FILE* fp = fopen(nombreArchivo, "r");
    
    if (fp == NULL){
        printf("Error al abrir el archivo (Error en input).\n");
        return; 
    }
        
    else{    //si el archivo se pudo abrir
        char nombreLaberinto[20];
        int filas, columnas, filaIni, colIni, arribaIni, frenteIni;
        
        while(fscanf(fp, "%s", nombreLaberinto) == 1 && strcmp(nombreLaberinto, "FIN") != 0){    //lee el nombre del tablero, y mientras no haya alcanzado el fin del archivo (denotado por un string "FIN"):

            fscanf(fp, "%d%d%d%d%d%d", &filas, &columnas, &filaIni, &colIni, &arribaIni, &frenteIni); //escanear los datos del tablero

            int** tablero = crearTablero(filas, columnas); //crea el tablero vacío
            
            for (int i=0; i<filas; i++){    //pobla el tablero con los datos del archivo
                for (int j=0; j<columnas; j++){
                    fscanf(fp, "%d", &tablero[i][j]);
                }
            }
            Pila* camino = crearPila();    //crea la pila de camino
            push(camino, filaIni - 1, colIni - 1);    //pobla la pila con la posición inicial en el tablero
            
            int* dado = iniciar_dado(crear_dado(arribaIni, frenteIni));    //inicia el dado con los valores iniciales del archivo
            
            int posIni[2] = {filaIni - 1, colIni - 1};    //crea un arreglo con la posición inicial en el tablero
            
            FILE* output = fopen("output.stdout", "w");    //crea el archivo de salida
            fprintf(output, "%s\n", nombreLaberinto);    //escribe el nombre del tablero en el archivo de salida
            escribirArchivo(realizarCamino(dado, filaIni - 1, colIni - 1, tablero, posIni, camino, filas, columnas, 0), output);    //escribe el camino en el archivo de salida
            free(tablero);
            free(camino);
            free(dado);
        }
    }
}

int main(){
    int** tablero = crearTablero(3, 3);
    tablero[0][0] = -1;
    tablero[0][1] = 2;
    tablero[0][2] = 4;
    tablero[1][0] = 5;
    tablero[1][1] = 5;
    tablero[1][2] = 6;
    tablero[2][0] = 6;
    tablero[2][1] = -1;
    tablero[2][2] = -1;
    Pila* camino = crearPila();
    push(camino, 0, 1);
    int* dado = iniciar_dado(crear_dado(5, 1));
    int posIni[2] = {0, 1};
    //moverDado funciona
    //esValido funciona
    
    mostrarPila(realizarCamino(dado, 0, 1, tablero, posIni, camino, 3, 3,0));
    return 0;
}