# Delegates

Implementation of *delegates* in C ++

Use
```
int sum(int a, int b) {
    return a + b;
}

delegate <int, int, int> mDelegate;

mDelegate.bind(suma);

int result = mDelegate(2, 3);

```
## Licence
This project is licensed under the MIT License 
