grammar Js;

program
   : statement
   ;

statement
   : elements
   ;

elements
    :  (element)*
    ;

element
    : declaration
    | assigment
    | if_elem
    | while_elem
    | alert
    ;

assignvalue
    : ASSIGN value_types
    ;

assigment
    : name assignvalue SEMICOLON?
    ;

declaration
    : LET name assignvalue SEMICOLON?
    ;

name
    : ID
    ;

value_types
    : number
    | name
    | calculations
    | read
    ;

value_calc
    : number
    | name
    ;

number
   : NUMBER
   ;

alert
    : ALERT RLP value_types RRP SEMICOLON?
    ;

compare
    : value_types compareoperation value_types
    ;

compareoperation
    : MR
    | LESSEQ
    | LESS
    | MREQ
    | EQ
    | NOTEQ
    ;

while_elem
    : WHILE RLP compare RRP CLB elements CRB
    ;

if_elem
    : IF RLP compare RRP CLB elements CRB
    ;

calculations
    : value_calc
	| calculations lvl1op calculations
	| calculations lvl2op calculations
	| calculations compareoperation calculations
	| RLP calculations RRP
    ;

lvl1op
    : MULT 
    | DIV
    ;

lvl2op
    : PLUS
    | MINUS
    ;

read
    : 'read' RLP RRP
    ;

HEX : '0' [xX] [0-9a-fA-F] [_0-9a-fA-F]*;
OCTAL: '0' [oO] [0-7] [_0-7]*;
BIN: '0' [bB] [01] [_01]*;
// NUMBER : ([0-9]+ [.] [0-9]*) | ([.]? [0-9]+) ([eE] [+-]? [0-9]+)? [fFlL]?;
NUMBER: [+-]? ('0' | [1-9] [0-9]*);

LINECOMMENT : '//' ~[\r\n]* -> skip;
MULTYCOMMENT : '/*' .*? '*/' -> skip;
WHILE : 'while';
IF  : 'if';
LET : 'let';
ALERT : 'alert';
ASSIGN : '=';
LESS : '<';
MR : '>';
LESSEQ : '<=';
MREQ : '>=';
PLUSPLUS : '++';
PLUS : '+';
MINUSMINUS : '--';
MINUS : '-';
MULT : '*';
DIV : '/';
MODULE : '%';
NOTEQ : '!=';
EQ : '==';
COLON: ':';
SEMICOLON : ';';
SLB : '[';
SRB : ']';
RLP : '(';
RRP : ')';
CLB : '{';
CRB : '}';
DOT : '.';
COMMA : ',';
QUOT : '\'';
DOUBLEQUOT : '"';

ID : [A-Za-z_$] [A-Za-z0-9_$]*;
WS: [ \n\t\r]+ -> skip;
INVALID: .;