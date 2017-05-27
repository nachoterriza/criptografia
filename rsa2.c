#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <errno.h>

#define MAX_CLAVES      10
#define MAX_MENSAJE     100

typedef struct Clave_publica {
    uint64_t n;
    uint64_t e;
} Clave_publica;

typedef struct Clave_privada {
    uint64_t n;
    uint64_t d;
} Clave_privada;

typedef struct Claves {
    Clave_publica publica;
    Clave_privada privada;
} Claves;

Claves genera_claves();
int es_primo(uint64_t num);
void salida(char * mensaje);
uint64_t mcd(uint64_t a, uint64_t b);
uint64_t cifra(uint64_t mensaje, Clave_publica clave);
uint64_t descifra(uint64_t mensaje, Clave_privada clave);
uint64_t exponente_modular(uint64_t base, uint64_t exp, uint64_t mod);

uint64_t mensaje_a_entero(char * mensaje);
char * entero_a_mensaje(uint64_t entero);
int numero_de_digitos(uint64_t num, int base);
uint64_t inverso(uint64_t a, uint64_t base);

/* Cálculo del inverso por el algoritmo
 * extendido de Euclides.
 * @a: clase a invertir
 * @base: base del anillo
 * */
uint64_t inverso(uint64_t a, uint64_t b)
{
    uint64_t inv, a1, a3, b1, b3, t1, t3, q;
    int64_t iter;
    a1 = 1;
    a3 = a;
    b1 = 0;
    b3 = b;
    iter = 1;

    while(b3 != 0){
        q = a3/b3;
        t3 = a3 % b3;
        t1 = a1 + q*b1;
        a1 = b1;
        b1 = t1;
        a3 = b3;
        b3 = t3;
        iter = -iter;
    }

    if(a3 != 1)
        return 0;

    if(iter<0)
        inv = b - a1;
    else
        inv = a1;

    return inv;
}

/** Funcion mcd: máximo común divisor
 *  Entradas: a y b, dos números enteros
 *  Salida: mcd(a,b).
 */
uint64_t mcd(uint64_t a, uint64_t b) {
    while(a!=b) {
        if(a > b)
            a-=b;
        else
            b-=a;
    }
    return a;
}

/** Funcion exponente_modular: potencia de un número en un anillo
 *  Entradas: base, exponente y módulo
 *  Salida: (base elevado a exp) módulo mod
 */
uint64_t exponente_modular(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t k=0;
    uint64_t ret=1;
    for(;k<exp;k++) {
        ret*=base;
        ret%=mod;
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

int es_primo(uint64_t num) {
    double max=sqrt(num);
    if(num%2==0)
        return num==2;
    for(int k=3; k<=max; k+=2) {
        if(num%k==0)
            return 0;
    }
    return 1;
}

Claves genera_claves() {
    uint64_t p, q, n, phi;
    Claves ret[MAX_CLAVES];
    char respuesta;
    int indice=0;

    printf("Necesitamos dos números primos:\n");
    scanf("%i", &p);
    if(!es_primo(p))
        salida("Entrada incorrecta. P no es primo");
    scanf("%i", &q);
    if(!es_primo(q))
        salida("Entrada incorrecta. Q no es primo");
    if(p==q)
        salida("Entrada incorrecta. P es igual a Q");

    n = p * q; 
    printf("N = %" PRIu64 ", p = %" PRIu64 ", q = %" PRIu64 "\n", n, p, q);
    phi = (p-1) * (q-1); 
    printf("Phi(n) = %" PRIu64 "\n", phi);

    /* Elegir e tal que 1 < e < phi(n), y tal que
     * e y n sean coprimos. */
    uint64_t i=2,j=0;
    for(; i<phi && j<MAX_CLAVES; i++) {
        if((i!=p) && (i!=q) && (mcd(i,phi)==1)){ // e y n son coprimos, y e y phi(n) también
            ret[j].publica.e=i;
            ret[j].publica.n=n;
            ret[j].privada.n=n;
            ret[j].privada.d=inverso(i,phi);
            printf("Creado: %" PRIu64 " - %" PRIu64 "\n", i, ret[j].privada.d);
            j++;
           /* if(numero_de_digitos(i,10)==numero_de_digitos(ret[j].privada.d,10))
                j++;*/
        }
    }

    printf("Claves generadas.\n");
    for(i=0; i<j; i++) {
        printf("La clave pública número %i es (e,n) = (%" PRIu64 ",%" PRIu64 ")\n", i, ret[i].publica.e, ret[i].publica.n);
        printf("La clave privada número %i es (d,n) = (%" PRIu64 ",%" PRIu64 ")\n", i, ret[i].privada.d, ret[i].privada.n);
    }

    while(respuesta!='s' && respuesta!='n') {
        printf("\nPor defecto escogeremos la primera (0).\n¿Quiere cambiar (s/n)?\n");
        getchar();
        respuesta = getchar();
    }

    if(respuesta=='s') {
        getchar();
        while(indice<1 || indice>i) {
            printf("Por favor escoja un número de clave válido: 1,...,%i:\n", i-1);
            scanf("%i", &indice);
            printf("Elegido: %i\n", indice);
        }
        Claves tmp = ret[indice];
        ret[indice] = ret[0];
        ret[0] = tmp;
    }
    printf("\n\nClave elegida:\n\n\tkp = ( %i, %i )\n\n\tks = ( %i, %i )\n\n", ret[0].publica.e, ret[0].publica.n, ret[0].privada.d, ret[0].privada.n );

    return ret[0];
}

uint64_t cifra(uint64_t mensaje, Clave_publica clave) {
    return exponente_modular(mensaje, clave.e, clave.n);
}

uint64_t descifra(uint64_t mensaje, Clave_privada clave) {
    return exponente_modular(mensaje, clave.d, clave.n);
}

uint64_t mensaje_a_entero(char * mensaje) {
    uint64_t ret=0;
    uint64_t k=0;

    for(; mensaje[k] != '\n'; k++) {
        ret *= 256;
        ret += mensaje[k];
    }
    printf("\n");

    return ret;
}

char * entero_a_mensaje(uint64_t entero) {

    char *ret = malloc(sizeof(char) * 100);
    int k=numero_de_digitos(entero, 256);
    ret[k]='\n';
    k--;
    for(; entero>0; k--) {
       ret[k] = entero;
       entero /= 256;
    }
    printf("\n");

    return ret;
}

int numero_de_digitos(uint64_t num, int base) {

    int k=0;

    for(; num>0; k++)
        num /= base;

    return k;
}

int main() {
   Claves mis_claves = genera_claves();
   char *mensaje = malloc(sizeof(char) * MAX_MENSAJE);
   char *plano = malloc(sizeof(char) * MAX_MENSAJE);
   char *comando = malloc(sizeof(char) * MAX_MENSAJE);
   uint64_t texto, cifrado, descifrado;
   int k=0;
   printf("Escriba su mensaje:\n");
   fflush(stdin);
   getchar();
   fgets(mensaje, MAX_MENSAJE, stdin);
   texto = mensaje_a_entero(mensaje);
   printf("Mensaje a entero: %" PRIu64 "\n", texto);
   cifrado=cifra(texto, mis_claves.publica);
   printf("Mensaje: %" PRIu64 " -> cifrado: %" PRIu64 "\n", texto, cifrado);
   sprintf(comando, "echo -n %" PRIu64 " | openssl enc -base64", cifrado);
   printf("Texto cifrado:\n");
   system(comando);

   descifrado = descifra(cifrado, mis_claves.privada);
   printf("Cifrado: %" PRIu64 " -> mensaje: %" PRIu64 "\n", cifrado, descifrado);
   
   plano = entero_a_mensaje(descifrado);
   printf("Mensaje descifrado: %s\n", plano);
   free(comando);
   free(plano);
   free(mensaje);
   return 0;
}
