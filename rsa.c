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
        return num==2;
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
    Claves *ret = malloc(sizeof(Claves));
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
            //printf("%ld y %ld son coprimos\n", i, n);
            ret[j].publica.e=i;
            ret[j].publica.n=n;
            ret[j].privada.n=n;
            ret[j].privada.d=encuentra_d(i,phi);
            j++;
        }
    }

    printf("Claves generadas.\n");
    for(i=0; i<j; i++) {
        printf("La clave pública número %ld es (e,n) = (%ld,%ld)\n", i, ret[i].publica.e, ret[i].publica.n);
        printf("La clave privada número %ld es (d,n) = (%ld,%ld)\n", i, ret[i].privada.d, ret[i].privada.n);
    }

    while(respuesta!='s' && respuesta!='n') {
        printf("\nPor defecto escogeremos la primera (0).\n¿Quiere cambiar (s/n)?\n");
        //scanf(" %c", respuesta);
        getchar();
        respuesta = getchar();
    }

    if(respuesta=='s') {
        //printf("Indique el número de la nueva clave: 1,...,%ld:\n", i);
        //scanf("%ld", eleccion);
        eleccion = getchar();
        while((eleccion-'0')<1 || (eleccion-'0')>i) {
            printf("Por favor escoja un número de clave válido: 1,...,%ld:\n", i);
            //scanf("%ld", eleccion);
            eleccion = getchar();
        }
        printf("Paso1\n");
        Claves tmp = ret[(eleccion-'0')];
        ret[(eleccion-'0')] = ret[0];
        ret[0] = tmp;
        printf("Paso2\n");
    }
    printf("\n\nClave elegida:\n\n\tkp = ( %ld, %ld )\n\n\tks = ( %ld, %ld )\n\n", ret[0].publica.e, ret[0].publica.n, ret[0].privada.d, ret[0].privada.n );

    return ret;
}

char * cifra(char * mensaje, Clave_publica clave) {
    char texto_cifrado[MAX_MENSAJE];
    int k=0;
    long int texto_plano[MAX_MENSAJE], temp[MAX_MENSAJE], enc;

    for(k=0; mensaje[k]!=NULL; k++) {
        texto_plano[k] = mensaje[k];
        printf("%ld, ", texto_plano[k]);
    }
    texto_plano[k]=NULL;
    printf("\n");

    k=0;

    while(texto_plano[k] != NULL) {
       enc = texto_plano[k];
       temp[k] = ( ((long int) pow(enc, clave.e)) % clave.n);
       texto_cifrado[k] = temp[k]+'a';
       printf("Int transformado: %ld\n", temp[k]);
       printf("Char transformado: %c\n", texto_cifrado[k]);
       k++;
    }
    texto_cifrado[k] = NULL;

    return texto_cifrado;
}

int main() {
   Claves * mis_claves = genera_claves();
   char mensaje[MAX_MENSAJE];
   char* cifrado;
   printf("Escriba su mensaje\n");
   fflush(stdin);
   scanf("%s", mensaje);
   cifrado=cifra(mensaje, mis_claves[0].publica);
   printf("El mensaje cifrado es:\n%s\n", cifrado);
   return 0;
}

