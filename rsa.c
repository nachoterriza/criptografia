#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <errno.h>

#define MAX_CLAVES      50
#define MAX_MENSAJE     100

typedef struct Clave_publica {
    unsigned long int n;
    unsigned long int e;
} Clave_publica;

typedef struct Clave_privada {
    unsigned long int n;
    unsigned long int d;
} Clave_privada;

typedef struct Claves {
    Clave_publica publica;
    Clave_privada privada;
} Claves;

Claves genera_claves();
int es_primo(unsigned long int num);
unsigned long int encuentra_d(unsigned long int e, unsigned long int phi);
void salida(char * mensaje);
unsigned long int mcd(unsigned long int a, unsigned long int b);
char * cifra(char * mensaje, Clave_publica clave);
char * descifra(char * mensaje, Clave_privada clave);
long int exponente_modular(long int base, long int exp, long int mod);

/** Funcion mcd: máximo común divisor
 *  Entradas: a y b, dos números enteros
 *  Salida: mcd(a,b).
 */
unsigned long int mcd(unsigned long int a, unsigned long int b) {
    while(a!=b) {
        if(a > b)
            a-=b;
        else
            b-=a;
    }
    return a;
}

long int exponente_modular(long int base, long int exp, long int mod) {
    int k=0;
    int ret=1;
    for(k;k<exp;k++) {
        ret*=base;
        ret = ret%mod;
    }

    return ret;
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

int es_primo(unsigned long int num) {
    double max=sqrt(num);
    if(num%2==0)
        return num==2;
    for(int k=3; k<=max; k+=2) {
        if(num%k==0)
            return 0;
    }
    return 1;
}

unsigned long int encuentra_d(unsigned long int e, unsigned long int phi) {
    
    // Encuentra d tal que (d * e) % phi(n) = 1
    unsigned long int d=1;
    while(1) {
        if(((d*e) % phi) == 1)
            return d;
        d++;
    }

}


Claves genera_claves() {
    unsigned long int p, q, n, phi;
    //Claves *ret = malloc(sizeof(Claves));
    Claves ret[MAX_CLAVES];
    char respuesta, *eleccion;
    int indice=0;

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
        while(indice<1 || indice>i) {
            printf("Por favor escoja un número de clave válido: 1,...,%ld:\n", i-1);
            scanf("%d", indice);
            //scanf("%s", eleccion);
            //gets(eleccion);
            //indice = atoi(eleccion);
            //eleccion = getchar();
            printf("Elegido: %i\n", indice);
        }
        Claves tmp = ret[indice];
        ret[indice] = ret[0];
        ret[0] = tmp;
    }
    printf("\n\nClave elegida:\n\n\tkp = ( %ld, %ld )\n\n\tks = ( %ld, %ld )\n\n", ret[0].publica.e, ret[0].publica.n, ret[0].privada.d, ret[0].privada.n );

    return ret[0];
}

char * cifra(char * mensaje, Clave_publica clave) {
    char *texto_cifrado = malloc(sizeof(char));
    int k=0;
    unsigned long int texto_plano[MAX_MENSAJE], temp[MAX_MENSAJE], enc;

    for(k=0; mensaje[k]!=NULL; k++) {
        texto_plano[k] = mensaje[k];
        printf("%ld, ", texto_plano[k]);
    }
    texto_plano[k]=NULL;
    printf("\n");

    printf("Clave->e: %ld, Clave->n: %ld\n", clave.e, clave.n);
    k=0;

    while(texto_plano[k] != NULL) {
       enc = texto_plano[k];
       //temp[k] = ( ((unsigned long long int) pow(enc, clave.e)) % clave.n);
       temp[k] = exponente_modular(enc, clave.e, clave.n);
       texto_cifrado[k] = temp[k]+'a';
       printf("Int transformado: %ld\n", temp[k]);
       printf("Char transformado: %c\n", texto_cifrado[k]);
       k++;
    }
    texto_cifrado[k] = NULL;

    return texto_cifrado;
}

int main() {
   Claves mis_claves = genera_claves();
   char mensaje[MAX_MENSAJE];
   char* cifrado;
   printf("Escriba su mensaje\n");
   fflush(stdin);
   scanf("%s", mensaje);
   cifrado=cifra(mensaje, mis_claves.publica);
   printf("El mensaje cifrado es:\n%s\n", cifrado);
   return 0;
}

