
/**
* Trabajo para la asignatura de Criptografia y Teoria de Codigos
* Facultad de Informatica - Universidad Complutense de Madrid
* Junio de 2017
*
*	Autores:
*		Nacho Terriza		(github.com/nachoterriza)
*		Pablo Dominguez		(github.com/pablo)
*		Marcos Rafael		(github.com/marcos)
*		JuanJe Martos		(github.com/juanjeakabob)
*
*
*	Este codigo dispone de licencia GNU: (para más información www.gnu.org)
*
*		This program is free software: you can redistribute it and/or modify
*		it under the terms of the GNU General Public License as published by
*		the Free Software Foundation, either version 3 of the License, or
*		(at your option) any later version.
*
**/


#include <gmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_MENSAJE     4000
#define TAM_BLOQUE      80
#define MAX_BLOQUES     MAX_MENSAJE / TAM_BLOQUE

typedef struct Clave_publica
{
    mpz_t n; /* Módulo: n = p * q */
    mpz_t e; /* Exponente público */
} Clave_publica;

typedef struct Clave_privada
{
    mpz_t n; /* Módulo: n = p * q */
    mpz_t e; /* Exponente público */
    mpz_t d; /* Exponente privado: d * e = 1 (mod phi(n)) */
    mpz_t p; /* Primer primo: el de valor mayor p > q */
    mpz_t q; /* Segundo primo */
    mpz_t d_p; /* Primer exponente, para el TCR: d_p = d (mod p-1) */
    mpz_t d_q; /* Segundo exponente: d_q = d (mod q-1) */
    mpz_t q_inv; /* Coeficiente, para el TCR: q_inv = q ^ (-1) (mod p) */
} Clave_privada;

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


/** Funcion es_primo:
*	Entrada: num
*	Salida: 
*	1 si num es primo,
*	0 si no lo es.
*	Comportamiento: Comprueba si num es par
*	y si no lo es divide por los impares
*	hasta la raíz cuadrada de num.
**/

int es_primo(mpz_t num)
{
    mpz_t max;
    mpz_init(max);
    mpz_t tmp;
    mpz_init(tmp);
    mpz_sqrt(max, num);
    int k = 3;

    mpz_mod_ui(tmp, num, 2);
    if(!mpz_cmp_ui(tmp, 0))
        return !mpz_cmp_ui(num, 2);

    for(; mpz_cmp_ui(max, k) > 0; k += 2)
    {
        mpz_mod_ui(tmp, num, k);
        if(!mpz_cmp_ui(tmp, 0))
            return 0;
    }
    mpz_clear(max);
    mpz_clear(tmp);

    return 1;
}


/**Funcion genera_claves
*	Entrada: Memoria reservada para clave pública y clave privada
*	Salida: VOID
*	Comportamiento: Se piden dos números primos (p,q) al usuario, 
*	y se calculan los parametros de las claves.
*	n = p*q
*	phi = (p-1) * (q-1)
*	Tomamos e si podemos igual a 65537 que es igual a 2 ^16 + 1
*	e tiene que cumplir MCD(e, phi)= 1 BOOM!
*	La clave pública se compone de e y n
*	Para la clave privada calculamos más coeficientes que
*	nos ayudarán a descifrar
*
*
**/

void genera_claves(Clave_publica * kp, Clave_privada * ks)
{
    mpz_t phi;
    mpz_init(phi);
    mpz_t p_1;
    mpz_init(p_1);
    mpz_t q_1;
    mpz_init(q_1);
    
    printf("Necesitamos dos números primos:\n");
    if(mpz_inp_str(ks->p, stdin, 10) == 0)
        salida("Error escaneando entero");
    //if(!es_primo(ks->p))
    if(!mpz_probab_prime_p(ks->p, 38))
        salida("Entrada incorrecta: P no es primo");
    if(mpz_inp_str(ks->q, stdin, 10) == 0)
        salida("Error escaneando entero");
    //if(!es_primo(ks->q))
    if(!mpz_probab_prime_p(ks->q, 38))
        salida("Entrada incorrecta: Q no es primo");
    if(mpz_cmp(ks->p, ks->q) < 0)
        mpz_swap(ks->p, ks->q);

    else if(mpz_cmp(ks->p, ks->q) == 0)
        salida("Error: P y Q deben ser distintos");

    mpz_mul(ks->n, ks->p, ks->q);
    mpz_set(kp->n, ks->n);

    mpz_sub_ui(p_1, ks->p, 1);
    mpz_sub_ui(q_1, ks->q, 1);
    mpz_mul(phi, p_1, q_1);

    // Primero probamos si es factible el valor clásico, 2^16 + 1 = 65537
    mpz_set_ui(ks->e, 65537);
    if(!mpz_invert(ks->d, ks->e, phi))
    {
        /* Probar con otros valores, e = 3, etc... */
    }
    else
    {
        mpz_mod(ks->d_p, ks->d, p_1);
        mpz_mod(ks->d_q, ks->d, q_1);
        mpz_invert(ks->q_inv, ks->q, ks->p);
        mpz_set(kp->e, ks->e);
    }

    printf("Claves generadas\n");
    gmp_printf("\n\tClave pública:\n\n\t\tMódulo: %Zd (%#Zx)\n\t\tExponente: %Zd (%#Zx)\n", kp->n, kp->n, kp->e, kp->e);
    gmp_printf("\n\tClave privada:\n\n\t\tMódulo: %Zd (%#Zx)\n\t\tExponente público: %Zd (%#Zx)\n\t\tExponente privado: %Zd (%#Zx)\n\t\tPrimer primo P: %Zd (%#Zx)\n\t\tSegundo primo Q: %Zd (%#Zx)\n\t\tPrimer exponente d_p: %Zd (%#Zx)\n\t\tSegundo exponente d_q: %Zd (%#Zx)\n\t\tCoeficiente q_inverso: %Zd (%#Zx)\n\n",ks->n, ks->n, ks->e, ks->e, ks->d, ks->d, ks->p, ks->p, ks->q, ks->q, ks->d_p, ks->d_p, ks->d_q, ks->d_q, ks->q_inv, ks->q_inv);

    mpz_clear(p_1);
    mpz_clear(q_1);
    mpz_clear(phi);

}

int numero_de_digitos(mpz_t num, int base)
{
    int k = 0;
    mpz_t entero;
    mpz_init(entero);
    mpz_set(entero, num);

    for(; mpz_cmp_ui(entero, 0) > 0; k++)
        mpz_tdiv_q_ui(entero, entero, base);

    mpz_clear(entero);
    return k;
}

void mensaje_a_entero(char * mensaje, mpz_t rop, int tam)
{
    mpz_set_ui(rop, 0);
    int k = 0;

    for(; k < tam; k++)
    {
        mpz_mul_ui(rop, rop, 256);
        mpz_add_ui(rop, rop, (unsigned int) mensaje[k]);
        if(mensaje[k] == '\n')
            for(int l=1; l<TAM_BLOQUE; l++)
                mensaje[k+l] = '\0';
    }

    return;
}

void entero_a_mensaje(mpz_t entero, char * destino)
{
    int k = numero_de_digitos(entero, 256);
    mpz_t tmp;
    mpz_init(tmp);
    destino[k] = '\0';
    k--;

    for(; mpz_cmp_ui(entero, 0) > 0; k--)
    {
        mpz_mod_ui(tmp, entero, 256);
        destino[k] = (char) mpz_get_ui(tmp);
        mpz_tdiv_q_ui(entero, entero, 256);
    }

    mpz_clear(tmp);

    return;
}

void cifra(mpz_t texto, Clave_publica clave, mpz_t cifrado)
{
    gmp_printf("Me mandan: %Zd\n", texto);
    mpz_powm(cifrado, texto, clave.e, clave.n);
    return;
}

void descifra(mpz_t cifrado, Clave_privada clave, mpz_t descifrado)
{
    char * mensaje = malloc(sizeof(char) * MAX_MENSAJE);
    mpz_t m1;
    mpz_init(m1);
    mpz_t m2;
    mpz_init(m2);
    mpz_t h;
    mpz_init(h);
    mpz_t m;
    mpz_init(m);

    mpz_powm(m1, cifrado, clave.d_p, clave.p);
    mpz_powm(m2, cifrado, clave.d_q, clave.q);

    if(mpz_cmp(m1, m2) < 0)
    {
        mpz_add(m, m1, clave.p);
        mpz_sub(m, m, m2);
    }
    else
    {
        mpz_sub(m, m1, m2);
    }

    mpz_mul(h, clave.q_inv, m);
    mpz_mod(h, h, clave.p);
    mpz_mul(h, h, clave.q);
    mpz_add(m, m2, h);
    mpz_set(descifrado, m);
    mpz_clear(m1);
    mpz_clear(m2);
    mpz_clear(h);
    mpz_clear(m);
    return;
}

int main()
{
    Clave_publica kp;
    Clave_privada ks;
    char * mensaje = malloc(sizeof(char) * MAX_MENSAJE);
    char * mensaje2 = malloc(sizeof(char) * MAX_MENSAJE);
    char * mensaje3 = malloc(sizeof(char) * MAX_MENSAJE);
    char bloque[MAX_BLOQUES][TAM_BLOQUE];
    mpz_t * texto = malloc(sizeof(mpz_t) * MAX_BLOQUES);
    mpz_t * cifrado = malloc(sizeof(mpz_t) * MAX_BLOQUES);
    mpz_t * descifrado = malloc(sizeof(mpz_t) * MAX_BLOQUES);

    mpz_init(kp.n);
    mpz_init(kp.e);

    mpz_init(ks.n);
    mpz_init(ks.e);
    mpz_init(ks.d);
    mpz_init(ks.p);
    mpz_init(ks.q);
    mpz_init(ks.d_p);
    mpz_init(ks.d_q);
    mpz_init(ks.q_inv);

    genera_claves(&kp, &ks);

    printf("Escriba el mensaje a cifrar:\n");
    fflush(stdin);
    getchar();
    fgets(mensaje, MAX_MENSAJE, stdin);

    int i = 0;
    for(; mensaje[i] != '\n'; i++)
    {
       bloque[i/TAM_BLOQUE][i%TAM_BLOQUE] = mensaje[i];
    }
    bloque[i/TAM_BLOQUE][i%TAM_BLOQUE] = '\n';

    for(int k=0; k<=i/TAM_BLOQUE; k++)
    {
        mpz_init(texto[k]);
        mensaje_a_entero(bloque[k], texto[k], TAM_BLOQUE);
        printf("Bloque %i: [", k);
        for(int m=0; m<TAM_BLOQUE; m++)
            printf("%c", bloque[k][m]);
        gmp_printf("] -> %Zd\n", texto[k]);
    }

    for(int k=0; k<=i/TAM_BLOQUE; k++)
    {
        mpz_init(cifrado[k]);
        cifra(texto[k], kp, cifrado[k]);
        gmp_printf("Entero cifrado: %Zd\n", cifrado[k]);
    }

    for(int k=0; k<=i/TAM_BLOQUE; k++)
    {
        mpz_init(descifrado[k]);
        descifra(cifrado[k], ks, descifrado[k]);
        gmp_printf("Entero descifrado: %Zd\n", descifrado[k]);
    }

    for(int k=0; k<=i/TAM_BLOQUE; k++)
    {
        entero_a_mensaje(descifrado[k],mensaje2);
        strcat(mensaje3, mensaje2);
    }

    printf("Mensaje descifrado: %s\n", mensaje3);

    free(mensaje);
    free(mensaje2);
    free(mensaje3);
    return 0;
}
