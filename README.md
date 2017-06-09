#Cifrado RSA
Trabajo sobre el cifrado RSA para la asignatura Criptografía y Teoría de Códigos.
Este repositorio contiene:
* rsa.c
* rsa2.c
* encode.c

##rsa.c
Muestra una versión básica del cifrado en la que se cifra caracter a caracter. Esta versión es susceptible a ataques de análisis de frecuencias ya que un caracter producirá siempre el mismo caracter de salida (se prueba a descifrar los caracteres más repetidos como 'e', 'a', ' '...). En esta versión las claves utilizan enteros **unsigned int**.

##rsa2.c
Muestra una versión más avanzada, reemplazando **unsigned int** por **unsigned long long int** o **uint64_t**. Aún son enteros demasiado pequeños como para trabajar con claves seguras. El mensaje se trata de cifrar todo a la vez, convirtiendo la cadena de caracteres en un número entero (cada caracter es un dígito en base 256). Si este número entero es mayor que el módulo de nuestra clave el descifrado no tendrá éxito: necesitaremos partir el mensaje en bloques (o hacer la clave de mayor tamaño).

##encode.c
Para solucionar los problemas anteriores utilizamos la librería GNU de precisión múltiple, gmp [link](https://gmplib.org) con la que podemos tener en las claves el tamaño que queramos. Además dividimos el mensaje en bloques para poder que ninguno tenga una codificación mayor que el módulo, y así poder cifrar y descifrar mensajes de tamaño arbitrario sin problema.

---
#Compilación y Uso

##rsa.c
```
make clean; make rsa
./rsa
```

##rsa2.c
```
make clean; make rsa2
./rsa2
```

##encode.c
```
make clean; make
./encode
```

#Nota
Para utilizar números primos muy grandes hemos utilizado la herramienta openssl:
```
openssl prime -generate -bits <numBits> -safe
```
