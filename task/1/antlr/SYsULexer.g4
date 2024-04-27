lexer grammar SYsULexer;

    Int : 'int';
    Return : 'return';
    Const : 'const';
    Void : 'void';
    If : 'if';
    Else : 'else';
    While : 'while';
    Break : 'break';
    Continue : 'continue';

    LeftParen : '(';
    RightParen : ')';
    LeftBracket : '[';
    RightBracket : ']';
    LeftBrace : '{';
    RightBrace : '}';

    Plus : '+';
    Minus : '-';
    Star : '*';
    Div : '/';
    Percent : '%';

    Ampamp : '&&';
    Pipepipe : '||';

    Semi : ';';
    Comma : ',';

    Equalequal : '==';
    Exclaimequal : '!=';
    Greaterequal  : '>=';
    Lessequal  : '<=';
    Equal : '=';
    Greater : '>';
    Less : '<';
    Exclaim : '!';

    Identifier
        :   IdentifierNondigit
            (   IdentifierNondigit
            |   Digit
            )*
        ;

    fragment
    IdentifierNondigit
        :   Nondigit
        ;

    fragment
    Nondigit
        :   [a-zA-Z_]
        ;

    fragment
    Digit
        :   [0-9]
        ;

    Constant
        :   IntegerConstant
        ;

    fragment
    IntegerConstant
        :   DecimalConstant
        |   OctalConstant
        |   HexadecimalConstant
        ;

    fragment
    DecimalConstant
        :   NonzeroDigit Digit*
        ;

    fragment
    OctalConstant
        :   '0' OctalDigit*
        ;

    fragment
    HexadecimalConstant
        :   '0x' HexadecimalDigit*
        ;

    fragment
    NonzeroDigit
        :   [1-9]
        ;

    fragment
    OctalDigit
        :   [0-7]
        ;

    fragment
    HexadecimalDigit
        :   [a-fA-F0-9]
        ;

    NonzeroHexadecimalDigit
        :   [a-fA-F1-9]
            -> skip
        ;

    // 预处理信息处理，可以从预处理信息中获得文件名以及行号
    // 预处理信息前面的数组即行号
    LineAfterPreprocessing
        :   '#' Whitespace* ~[\r\n]*
            //-> skip
        ;

    Whitespace
        :   [ \t]+
            //-> skip
        ;

    // 换行符号，可以利用这个信息来更新行号
    Newline
        :   (   '\r' '\n'?
            |   '\n'
            )
            //-> skip
        ;

