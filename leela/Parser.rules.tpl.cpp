/**
 * \file   parsing.tpl.cpp
 * \author Jan Milik <milikjan@fit.cvut.cz>
 * \date   2011-04-28
 *
 * \brief  
 */

{% for nonterminal in grammar.nonterminals.values() %}
Ref<Object> Parser::parse{{ nonterminal.name }}(Ref<Context> context)
{
	BRef<vector<Ref<Object> > > match;
	Ref<Object> result = match;
	
	switch (peek().type) {
{%- for rule, terminals in nonterminal.rules.items() -%}
{%- for terminal in terminals %}
	case Token::{{ terminal.name|upper }}:{% endfor -%}
{% for child in rule.children %}{% if isinstance(child, Terminal) %}
		match->push_back(accept(Token::{{ child.name|upper }}));
		{%- elif isinstance(child, Nonterminal) %}
		match->push_back(parse{{ child.name }}(context));
		{%- endif %}{% endfor %}
		break;
		{% endfor %}
	default:
		// Syntax error.
		break;
	}
	
	return result;
}
{% endfor %}

