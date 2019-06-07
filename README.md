# Delegates

Implementación de delegados en C++


Example
```c
int suma(int a, int b) {
    return a + b;
}

delegate <int, int, int> miDelegado;

miDelegado.bind(suma);

int result = miDelegado(2, 3);

```
