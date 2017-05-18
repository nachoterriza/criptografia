#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <errno.h>

#define MAX_CLAVES      50
#define MAX_MENSAJE     100

typedef struct Clave_publica {
    long int n;
    long int e;
} Clave_publica;

typedef struct Clave_privada {
    long int n;
    long int d;
} Clave_privada;

typedef struct Claves {
    Clave_publica publica;
    Clave_privada privada;
} Claves;

Claves * genera_claves();
int es_primo(long int num);
long int encuentra_d(long int e, long int phi);
void salida(char * mensaje);
long int mcd(long int a, long int b);
char * cifra(char * mensaje, Clave_publica clave);
char * descifra(char * mensaje, Clave_privada clave);

/** Funcion mcd: máximo común divisor
 *  Entradas: a y b, dos números enteros
 *  Salida: mcd(a,b).
 */
long int mcd(long int a, long int b) {
    while(a!=b) {
        if(a > b)
            a-=b;
        else
            b-=a;
    }
    return a;
}

/** Función salida:
 *  Entrada: Cadena mensaje a imprimir
 *  Salida: El mensaje impreso y fin de la 
 *  ejecución del programa con código de 
 *  salida 1.
 */
void salida(char * mensaje) {
    fprintf(stderr, "%s\n", mensaje);
    exit(1);
}

int es_primo(long int num) {
    double max=sqrt(num);
    if(num%2==0)
        return 0;
    for(int k=3; k<=max; k+=2) {
        if(num%k==0)
            return 0;
    }
    return 1;
}

long int encuentra_d(long int e, long int phi) {
    
    // Encuentra d tal que (d * e) % phi(n) = 1
    long int d=1;
    while(1) {
        if(((d*e) % phi) == 1)
            return d;
        d++;
    }

}


Claves * genera_claves() {
    long int p, q, n, phi;
    Claves ret[MAX_CLAVES];
    char respuesta, eleccion;

    printf("Necesitamos dos números primos:\n");
    scanf("%ld", &p);
    if(!es_primo(p))
        salida("Entrada incorrecta. P no es primo");
    scanf("%ld", &q);
    if(!es_primo(q))
        salida("Entrada incorrecta. Q no es primo");
    if(p==q)
        salida("Entrada incorrecta. P es igual a Q");

    n = p * q; 
    printf("N = %ld, p = %ld, q = %ld\n", n, p, q);
    phi = (p-1) * (q-1); 
    printf("Phi(n) = %ld\n", phi);
    /* Elegir e tal que 1 < e < phi(n), y tal que
     * e y n sean coprimos. */
    long int i=2,j=0;
    for(i; i<phi; i++) {
        if((mcd(i,n)==1) && (mcd(i,phi)==1)){ // e y n son coprimos, y e y phi(n) también
            printf("%ld y %ld son coprimos\n", i, n);
            ret[j].publica.e=i;
            ret[j].publica.n=n;
            ret[j].privada.n=n;
            ret[j].privada.d=encuentra_d(i,phi);
            j++;
        }
        printf("Vuelta %ld completada\n", i);
    }

    printf("Claves generadas.\n");
    for(i=0; i<j; i++) {
        printf("La clave pública número %ld es (e,n) = (%ld,%ld)\n", i, ret[i].publica.e, ret[i].publica.n);
        printf("La clave privada número %ld es (d,n) = (%ld,%ld)\n", i, ret[i].privada.d, ret[i].privada.n);
    }

    while(respuesta!='s' || respuesta!='n') {
        printf("\nPor defecto escogeremos la primera (0).\n¿Quiere cambiar?\n");
        scanf("%c", respuesta);
    }

    if(respuesta=='s') {
        printf("Indique el número de la nueva clave: 1,...,%ld", i);
        scanf("%ld", eleccion);
        while(eleccion<1 || eleccion>i) {
            printf("Por favor escoja un número de clave válido: 1,...,%ld", i);
            scanf("%ld", eleccion);
        }
        Claves tmp = ret[eleccion];
        ret[eleccion] = ret[0];
        ret[0] = eleccion;
    }

    return ret;
}

int main() {
   Claves * mis_claves = genera_claves();
   char mensaje[MAX_MENSAJE], cifrado*;
   printf("Escriba su mensaje\n");
   //fflush(stdin);
   scanf("%s", mensaje);
   cifrado=cifra(mensaje, mis_claves[0]);
   printf("El mensaje cifrado es:\n%s\n", cifrado);
   //printf("Elige un par de claves: 0 ... %i\n", 
   return 0;
}

