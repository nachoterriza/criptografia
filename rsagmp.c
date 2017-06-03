#include <gmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MODULO          512
#define MAX_MENSAJE     100

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
    if(!es_primo(ks->p))
        salida("Entrada incorrecta: P no es primo");
    if(mpz_inp_str(ks->q, stdin, 10) == 0)
        salida("Error escaneando entero");
    if(!es_primo(ks->q))
        salida("Entrada incorrecta: Q no es primo");
    if(mpz_cmp(ks->p, ks->q) < 0)
        mpz_swap(ks->p, ks->q);
    /*
    {
        
        mpz_t temp;
        mpz_init(temp);
        mpz_set_ui(temp, mpz_get_ui(ks->p));
        mpz_set_ui(ks->p, mpz_get_ui(ks->q));
        mpz_set_ui(ks->q, mpz_get_ui(temp));
        mpz_clear(temp);
        
    }*/

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

    return k;
}

void mensaje_a_entero(char * mensaje, mpz_t rop)
{
    mpz_set_ui(rop, 0);
    int k = 0;

    for(; mensaje[k] != '\n'; k++)
    {
        mpz_mul_ui(rop, rop, 256);
        mpz_add_ui(rop, rop, (unsigned int) mensaje[k]);
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
    mpz_t texto;
    mpz_init(texto);
    mpz_t cifrado;
    mpz_init(cifrado);
    mpz_t descifrado;
    mpz_init(descifrado);

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

    mensaje_a_entero(mensaje, texto);
    gmp_printf("Mensaje a entero: %Zd\n", texto);

    cifra(texto, kp, cifrado);
    gmp_printf("Entero cifrado: %Zd\n", cifrado);

    descifra(cifrado, ks, descifrado);
    gmp_printf("Entero descifrado: %Zd\n", descifrado);

    entero_a_mensaje(descifrado,mensaje2);
    printf("Mensaje descifrado: %s\n", mensaje2);

    free(mensaje);
    free(mensaje2);
    return 0;
}
