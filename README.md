# Зуев Даниил ИВ-921
# Вариант №11 - JavaScript

## Building requirements

- CMake 3.22
- ANTLR4 - 4.10.1
- GCC - 9.4.0
- Java - 11.0.15
- GNU Make - 4.2.1
- OS - Linux Ubuntu 20.04.1

#

## Build project
```./build.sh```

## Run examples
```./examples/run_fact.sh```
```./examples/run_math.sh```
```./examples/run_max.sh```
```./examples/run_nod.sh```

## Use parser
options:
  - --dump-ast
  - --dump-tokens
  - --dump-asm
  
```cd build/debug/bin```
```./js-parser [options] <filepath>```



#
## Задание 1
Дана грамматика. Постройте вывод заданной цепочки.
#
- a.
    ```
     S : T | T '+' S | T '-' S;
     T : F | F '*' T;
     F : 'a' | 'b';
     ```
    Цепочка:  ```a - b * a + b```.

**S** -> T - **S** -> T - T + **S** -> **T** - T + T -> F - **T** + T -> F - F * **T** + T -> F - F * F + **T** -> **F** - F * F + F -> a - **F** * F + F -> a - b * **F** + F -> a - b * a + **F** -> a - b * a + b

- b.
    ```
    S : 'a' S B C | 'ab' C;
    C B : B C;
    'b' B : 'bb';
    'b' C : 'bc' ;
    'c' C : 'cc';
    ```
    Цепочка: ```aaabbbccc```.

**S** -> a **S** B C -> a a **S** B C B C -> a a a b **C B** C B C -> a a a b B C **C B** C -> a a a b B **C B** C C -> a a a **b B** B C C C -> a a a b **b B** C C C -> a a a b b **b C** C C ->  a a a b b b **c C** C -> a a a b b b c **c C** -> a a a b b b c c 
#
## Задание 2
Построить грамматику, порождающую язык:\
![task 2](./misc/2.png)
#
a.\
G ({a, b, c}, {S, A, B, C}, P, S)\
P:\
    S : 'a' A;\
    A -> 'a' A | B;\
    B -> 'b' B | C;\
    C -> 'c' | 'c' C;

b.\
G ({0, 10}, {S, A, B}, P, S)\
P:\
    S : '0' | '0' A | B | ε;\
    A : '0' A | B | ε;\
    B : '10' | '10' B;

c.\
G ({0, 1}, {S, A}, P, S)\
P:\
    S : '0' A '0' | '1' A '1';\
    A : '0' A '0' | '1' A '1' | ε;\
#
## Задание 3
К какому типу по Хомскому относится грамматика с приведенными правилами? Аргументируйте ответ.
#
a.
```
S : '0' A '1' | '01';
'0' A : '00' A '1';
A : '01';
```
Тип грамматики - 1: контекстно зависимый, т.к. есть терминальные символы в левой части.

b.
```
S : A 'b';
A : A 'a' | 'ba';
```

Тип грамматики - 3: регулярный, т.к. в левой части нет терминальных символов, а в правой части есть только один не терминальный символ. Грамматика леволинейная.
#
## Задание 4
Построить КС-грамматику, эквивалентную грамматике с правилами:
```
S : A B | A B S;
A B : B A;
B A : A B;
A : 'a';
B : 'b';
```
#
S : 'ab' S | 'ba' S | 'a'S 'b'| 'b' S 'a' | 'ab' | 'ba';

#
## Задание 5
Построить регулярную грамматику, эквивалентную грамматике с правилами:
```
S : A '.' A;
A : B | B A;
B : '0' | '1';
```
#
S : '1' S | '0' S | '1' A | '0' A;\
A : '.' B;\
B : '1' B | '0' B | '0' | '1';
#
## Задание 6
Напишите регулярное выражение. В качестве ответа приведите:

1. Регулярные выражения

2. Тестовые данные для проверки

3. Ссылку на https://regexr.com/ с вашим решением
#
a. Для множества идентификаторов, где идентификатор – это последовательность букв или цифр, начинающаяся с буквы или _;
#
[Решение](https://regexr.com/6h3v3)

#
b. Для множества вещественных констант с плавающей точкой, состоящих из целой части, десятичной точки, дробной части, символа е или Е, целого показателя степени с необязательным знаком и необязательного суффикса типа – одной из букв f, F, l или L. Целая и дробная части состоят из последовательностей цифр. Может отсутствовать либо целая, либо дробная часть (но не обе сразу).
#
[Решение](https://regexr.com/6h67i)
#
## Задание 7
Для регулярных выражений из предыдущего задания постройте конечные автоматы. Изобразите их в виде графа. Для встраивания изображения в ответ воспользуйтесь сервисом http://gravizo.com/
#
S : '_' Q | '[a-zA-Z]' Q;\
Q : '[a-zA-Z]' Q | '[0-9]' Q | eps;

```
  digraph G {
    S -> Q [label="[a-zA-Z_]"];

    edge [color=black];
    Q -> Q [label="[a-zA-Z0-9]"];

    edge [color=red];
    Q -> ◉;
  }
```
<!-- <img src='https://g.gravizo.com/svg?
  digraph G {
    S -> Q [label="[a-zA-Z_]"];
    edge [color=black];
    Q -> Q [label="[a-zA-Z0-9]"];
    edge [color=red];
    Q -> ◉;
  }
'> -->
![task 7.1](./misc/g1.png)

#
S : '+' D | '-' D | D;\
D : [0-9] D | [0-9] | '.' D2 | [0-9] F ;\
D2 : [0-9] D2 | [0-9] | [0-9] F | 'e' E | 'E' E;\
E : '-' D3 | D3;\
D3 : [0-9] D3 | [0-9] | F;\
F : 'f' | 'F' | 'l' | 'L';

```
digraph G {
    S -> D [label="[+-]"];
    S -> D;

    D -> D [label="[0-9]"];
    edge [color=red];
    D -> ◉ [label="[0-9]"];
    edge [color=black];
    D -> F [label="[0-9]"];
    D -> D2 [label=" ."];

    D2 -> D2 [label="[0-9]"];
    edge [color=red];
    D2 -> ◉ [label="[0-9]"];
    edge [color=black];
    D2 -> F [label="[0-9]"];
    D2 -> E [label="[Ee]"];

    E -> D3 [label=" -"];
    E -> D3;

    D3 -> D3 [label="[0-9]"];
    edge [color=red];
    D3 -> ◉ [label="[0-9]"];
    edge [color=black];
    D3 -> F [label="[0-9]"];

    edge [color=red];
    F -> ◉ [label="[fFlL]"];
  }
```
<!-- <img src='https://g.gravizo.com/svg?
digraph G {
    S -> D [label="[+-]"];
    S -> D;
    D -> D [label="[0-9]"];
    edge [color=red];
    D -> ◉ [label="[0-9]"];
    edge [color=black];
    D -> F [label="[0-9]"];
    D -> D2 [label=" ."];
    D2 -> D2 [label="[0-9]"];
    edge [color=red];
    D2 -> ◉ [label="[0-9]"];
    edge [color=black];
    D2 -> F [label="[0-9]"];
    D2 -> E [label="[Ee]"];
    E -> D3 [label=" -"];
    E -> D3;
    D3 -> D3 [label="[0-9]"];
    edge [color=red];
    D3 -> ◉ [label="[0-9]"];
    edge [color=black];
    D3 -> F [label="[0-9]"];
    edge [color=red];
    F -> ◉ [label="[fFlL]"];
  }
'> -->
![task 7.2](./misc/g2.png)
