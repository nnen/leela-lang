/**
 * \file   leela/token_types.h
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2011-03-26
 *
 * \brief  Contains token types definitions.
 */

#ifndef T
	#define T(name)
#endif

#ifndef TK
	#define TK(name, kwrd)
#endif

#ifndef TD
	#define TD(name, ch)
#endif

#ifndef T2
	#define T2(name, ch1, ch2)
#endif

T(  IDENTIFIER              )
T(  NUMBER_LITERAL          )
T(  STRING_LITERAL          )
T(  UNKNOWN                 )
T(  END                     )
TK( VAR,           "var"    )
TK( CONST,         "const"  )
TK( BEGIN,         "begin"  )
TK( END,           "end"    )
TK( IF,            "if"     )
TK( THEN,          "then"   )
TK( ELSE,          "else"   )
TK( WHILE,         "while"  )
TK( DO,            "do"     )
TK( WRITE,         "write"  )
TK( RETURN,        "return" )
TD( SEMICOLON,     ';'      )
TD( COMMA,         ','      )
TD( LEFT_PAR,      '('      )
TD( RIGHT_PAR,     ')'      )
TD( LEFT_BRA,      '['      )
TD( RIGHT_BRA,     ']'      )
TD( LEFT_ANGLE,    '<'      )
TD( RIGHT_ANGLE,   '>'      )
TD( EQUALS,        '='      )
TD( PLUS,          '+'      )
TD( MINUS,         '-'      )
TD( SLASH,         '/'      )
TD( ASTERIX,       '*'      )
T2( ASSIGN,        ':', '=' )
T2( NOT_EQUALS,    '<', '>' )
T2( LESS_OR_EQ,    '<', '=' )
T2( GRATER_OR_EQ,  '>', '=' )
T2( LAMBDA,        '-', '>' )

#undef T
#undef TK
#undef TD
#undef T2

