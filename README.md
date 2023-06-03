# LL1 parser
Input an LL(1) grammar into grammar.txt. The grammmar must include S as the start symbol, and each token must be seperated by a space. Epsilon is denoted with %, and $ is reserved for eof.
The > symbol does not need to be used any single character after the first token is ignored, meaning S > > and S . token are valid.
```
S > E
E > T E'
E' > + T E'
E' > - T E'
E' > %
T > F T'
T' > * F T'
T' > / F T'
T' > %
F > ( E )
F > num
```
Input the stream of tokens to be parsed in input.txt, whitespaces must be in between each token newlines are allowed.
```
num + 
( num * num )
```