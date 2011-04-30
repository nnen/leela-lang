/**
 * \file   leela/token_types.h
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2011-03-26
 *
 * \brief  Contains token types definitions.
 */

#ifndef T
	#define T(name, repr)
#endif

#ifndef TK
	#define TK(name, kwrd, repr)
#endif

#ifndef TD
	#define TD(name, ch, repr)
#endif

#ifndef T2
	#define T2(name, ch1, ch2, repr)
#endif

T(  IDENT,                     "ident"    )
T(  NUMBER,                    "number"   )
T(  STRING,                    "string"   )
T(  UNKNOWN,                   "?"        )
T(  EOI,                       "[EOI]"    )
TK( VAR,           "var",      "var"      )
TK( CONST,         "const",    "const"    )
TK( BEGIN,         "begin",    "begin"    )
TK( END,           "end",      "end"      )
TK( IF,            "if",       "if"       )
TK( THEN,          "then",     "then"     )
TK( ELSE,          "else",     "else"     )
TK( WHILE,         "while",    "while"    )
TK( DO,            "do",       "do"       )
TK( WRITE,         "write",    "write"    )
TK( FUNCTION,      "function", "function" )
TK( LAMBDA,        "lambda",   "lambda"   )
TK( RETURN,        "return",   "return"   )
TD( SEMICOLON,     ';',        ";"        )
TD( COLON,         ':',        ":"        )
TD( COMMA,         ',',        ","        )
TD( LEFT_PAR,      '(',        "("        )
TD( RIGHT_PAR,     ')',        ")"        )
TD( LEFT_BRA,      '[',        "["        )
TD( RIGHT_BRA,     ']',        "]"        )
TD( LEFT_ANGLE,    '<',        "<"        )
TD( RIGHT_ANGLE,   '>',        ">"        )
TD( EQUALS,        '=',        "="        )
TD( PLUS,          '+',        "+"        )
TD( MINUS,         '-',        "-"        )
TD( SLASH,         '/',        "/"        )
TD( ASTERIX,       '*',        "*"        )
T2( ASSIGN,        ':', '=',   ":="       )
T2( NOT_EQUALS,    '<', '>',   "<>"       )
T2( LESS_OR_EQ,    '<', '=',   "<="       )
T2( GRATER_OR_EQ,  '>', '=',   ">="       )

#undef T
#undef TK
#undef TD
#undef T2

