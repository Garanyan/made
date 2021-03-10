```
start ->  variable '=' sum
```

```
sum -> product (('+' product) | ('-' product))*
```

```
product -> factor (('*' factor) | ('/' factor))*
```
```
factor -> number | parentheses | variable
```

```
parentheses -> '(' sum ')'
```