/**
 * \file   Parser.rules.tpl.cpp
 * \author Jan Milik <milikjan@fit.cvut.cz>
 * \date   2011-04-28
 *
 * \brief  
 */

#include "Parser.h"

{% for nonterminal in grammar.nonterminals.values() %}
Ref<Object> Parser::parse{{ nonterminal.name }}(Ref<Object> inherited, vector<Ref<Object> > siblings)
{
	vector<Ref<Object> > match;
	Ref<Object>          result;
	
	_nonterminals.push_back("{{ nonterminal }}");

	switch (peek().type) {
{%- for rule, terminals in nonterminal.rules.items() -%}
{%- for terminal in terminals %}
	{% if terminal.is_epsilon %}default:{% else %}case Token::{{ terminal.name|upper }}:{% endif %}{% endfor -%}
{% for child in rule.children %}{% if isinstance(child, Terminal) and not child.is_epsilon %}
		match.push_back(accept(Token::{{ child.name|upper }}).value);
		{%- elif isinstance(child, Nonterminal) %}
		match.push_back(parse{{ child.name }}(result, match));
		{%- elif isinstance(child, Action) %}
		{% if child.instruction %}_writer.writeInstruction(AsmScanner::TOKEN_{{ child.name }});{% else %}{{ child.name }}(inherited, siblings, match, result);{% endif %}
		{%- endif %}{% endfor %}
		break;
		{% endfor %}
	}

	_nonterminals.pop_back();
	
	return result;
}
{% endfor %}

