//Lab 2 Diseño de Algoritmos
//Nombres: Alfonso Palacios, Sofia Rojas

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

void imprimirCamino(Pila* P,char nombre[]){
    Nodo* aux = P->top;
    int val[2];
    printf("\n%s\n", nombre);
    if (P->top->sig==NULL){
        printf("No hay solucion posible\n");
    }else{
        Pila* Paux = crearPila();
        moverPila(P, Paux);
        int i=0;
        while (!esVacia(Paux)){
            aux = Paux->top;
            val[0] = aux->fila;
            val[1] = aux->columna;
            printf("(%d,%d)", val[0]+1, val[1]+1);
            push(P, val[0], val[1]);
            pop(Paux);
            i+=1;
            if (!esVacia(Paux)){
                printf(",");
                if(i==9){
                    printf("\n");
                    i=0;
                }
            }
        }
        free(Paux);
    }
}
//TERMINO DE TDA PILA

int devolver(Pila* camino, int fila, int columna, int val){
    //Revisa si se esta regresando por el mismo camino
    int devolver=1;
    if (val){
        int filaAux=camino->top->fila;
        int colAux=camino->top->columna;
        Pila* aux=crearPila();
        push(aux, filaAux, colAux);
        pop(camino);
        if (camino->top->fila==fila && camino->top->columna==columna){
            devolver=0;
        }
        push(camino, aux->top->fila, aux->top->columna);
        pop(aux);
        free(aux);
    }
    return devolver;
}

//Funcion para validar si el movimiento es valido
int esValido(int x, int** tablero, int fila, int columna, int cantFilas, int cantCol, int* dado, Pila* camino, int val){
    //NOTA: Si la casilla a la que se quiere mover el dado es una casilla vacia (es 0), no cumplirá la primera condición del segundo if de los casos válidos.
    if (x == 1 && fila-1 >= 0){
        if (devolver(camino, fila-1, columna, val) && (tablero[fila-1][columna] == -1 || tablero[fila-1][columna] == dado[0])){
            return 1;
        }
        return 0;
    }
    else if (x == 2 && columna+1 < cantCol){
        if (devolver(camino, fila, columna+1, val) && (tablero[fila][columna+1] == -1 || tablero[fila][columna+1] == dado[0])){
            return 1;
        }
        return 0;
    }
    else if (x == 3 && fila+1 < cantFilas){
        if (devolver(camino, fila+1, columna, val) && (tablero[fila+1][columna] == -1 || tablero[fila+1][columna] == dado[0])){
            return 1;
        }
        return 0;
    }
    else if (x == 4 && columna-1 >= 0){
        if (devolver(camino, fila, columna-1, val) && (tablero[fila][columna-1] == -1 || tablero[fila][columna-1] == dado[0])){
            return 1;
        }
        return 0;
    }
    else{
        return 0;
    }
}

int meta(int** tablero, int* dado, Pila* camino, int fila, int columna, int cantFilas, int cantCol, int posIni[2], int val){
    if (val){
        if(posIni[0]==fila){
            if (posIni[1]==columna+1 && esValido(2, tablero, fila, columna, cantFilas, cantCol, dado, camino, val)){
                return 2;
            }
            else if (posIni[1]==columna-1 && esValido(4, tablero, fila, columna, cantFilas, cantCol, dado, camino, val)){
                return 4;
            }
        }else if (posIni[1]==columna){
            if (posIni[0]==fila+1 && esValido(3, tablero, fila, columna, cantFilas, cantCol, dado, camino, val)){
                return 3;
            }
            else if (posIni[0]==fila-1 && esValido(1, tablero, fila, columna, cantFilas, cantCol, dado, camino, val)){
                return 1;
            }
        }
    }
    return 0;
}

void regresarDado(Pila* camino, int* dado){
    //arriba=mov[0]=1
    //derecha=mov[1]=1
    //abajo=mov[0]=-1
    //izquierda=mov[1]=-1
    Pila* aux=crearPila();
    int filaAux=camino->top->fila;
    int colAux=camino->top->columna;
    push(aux, filaAux, colAux);
    pop(camino);
    int mov[2]={filaAux-camino->top->fila, colAux-camino->top->columna};
    pop(aux);
    free(aux);
    if (mov[0]==1){
        moverDado(dado, 1);
    }
    else if (mov[1]==1){
        moverDado(dado, 4);
    }
    else if (mov[0]==-1){
        moverDado(dado, 3);
    }
    else{
        moverDado(dado, 2);
    }
}

int realizarMovimiento(int i, int* dado, int fila, int columna, int** tablero, int posIni[2], Pila* camino, int cantFilas, int cantCol, int val, int *exito){
	int aux;
	if (i == 1){
		push(camino, fila-1, columna);
		moverDado(dado, i);
		aux = fila-1;
		return aux;
	}
	else if (i == 2){
		push(camino, fila, columna+1);
		moverDado(dado, i);
		aux = columna+1;
		return aux;
	}
	else if (i == 3){
		push(camino, fila+1, columna);
        moverDado(dado, i);
		aux = fila+1;
		return aux;
	}
	else{
		push(camino, fila, columna-1);
        moverDado(dado, i);
		aux = columna-1;
		return aux;
	}
}

//FUNCION DE BACKTRACKING: Realiza el camino y retorna la pila con los espacios visitados. En caso de que no haya solución, retorna una pila vacía.
Pila* realizarCamino(int* dado, int fila, int columna, int** tablero, int posIni[2], Pila* camino, int cantFilas, int cantCol, int val, int *exito){
    int mov;
    if (camino->top->fila== posIni[0] && camino->top->columna == posIni[1] && val){    //Si llega a la casilla de inicio, retorna la pila con los espacios visitados
        *exito=1;
        return camino;
    }
    int i = 1;
    while (i <= 5){
        if (*exito==1){
            return camino;
        }
        mov = meta(tablero, dado, camino, fila, columna, cantFilas, cantCol, posIni, val);
        if (mov!=0){
            i=mov;
        }
        if (i == 1){    //hacia arriba
            if (esValido(i, tablero, fila, columna, cantFilas, cantCol, dado, camino, val) == 1){
                realizarCamino(dado, realizarMovimiento(i, dado, fila, columna, tablero, posIni, camino, cantFilas, cantCol, 1, exito), columna, tablero, posIni, camino, cantFilas, cantCol,1,exito);
            }
        }
        else if (i == 2){    //hacia la derecha
            if (esValido(i, tablero, fila, columna, cantFilas, cantCol, dado, camino, val) == 1){
                realizarCamino(dado, fila, realizarMovimiento(i, dado, fila, columna, tablero, posIni, camino, cantFilas, cantCol, 1, exito), tablero, posIni, camino, cantFilas, cantCol,1, exito);
            }
        }
        else if (i == 3){    //hacia abajo
            if (esValido(i, tablero, fila, columna, cantFilas, cantCol, dado, camino, val) == 1){
                realizarCamino(dado, realizarMovimiento(i, dado, fila, columna, tablero, posIni, camino, cantFilas, cantCol, 1, exito), columna, tablero, posIni, camino, cantFilas, cantCol,1, exito);
            }
        }
        else if (i == 4){    //hacia la izquierda
            if (esValido(i, tablero, fila, columna, cantFilas, cantCol, dado, camino, val) == 1){
                realizarCamino(dado, fila, realizarMovimiento(i, dado, fila, columna, tablero, posIni, camino, cantFilas, cantCol, 1, exito), tablero, posIni, camino, cantFilas, cantCol,1, exito);
            }
        }
        else{    //i = 5: No hay caminos posibles para el camino elegido
            if (val){
                regresarDado(camino, dado); //Vuelve a la casilla anterior
            }
            if (camino->top->fila==posIni[0]&&camino->top->columna==posIni[1]){    //Si no hay caminos posibles, retorna una pila vacía
                *exito=1;
                return camino;
            }
        }
        i++;
    }
}



//Función para leer el archivo: Esta función realiza todo el trabajo, llamando a las demás funciones
void leerArchivo(){
  FILE* fp = stdin;

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

            int exito=0;
            imprimirCamino(realizarCamino(dado, filaIni - 1, colIni - 1, tablero, posIni, camino, filas, columnas, 0, &exito), nombreLaberinto);    //escribe el camino en el archivo de salida
            free(tablero);
            free(camino);
            free(dado);
        }
    }
}

int main(){
    leerArchivo();
    return 0;
}