# LL1 parser
Input an LL(1) grammar into grammar.txt. The grammmar must include S as the start symbol, nonterminals must be uppercase only and can include non alphabetic characters. Terminals are lowercase only and can also include special characters. Each token must be seperated by a space. Epsilon is denoted with %, rules are in form NONTERMINAL > terminals\NONTERMINALS. NONTERMINAL > > is allowed. 
S > E
E > T E'
E' > + T E'
E' > - T E'
T > F T'
T' > * F T'
T' > / F T'
F > ( E )
F > num

Input the stream of tokens to be parsed in input.txt, whitespaces must be in between each token newlines are allowed.

num + 
( num * num )