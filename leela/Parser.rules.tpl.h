/**
 * \file   Parser.rules.tpl.h
 * \author Jan Milik <milikjan@fit.cvut.cz>
 * \date   2011-04-28
 *
 * \brief  
 */

enum Nonterminals {
	{% for nonterminal in grammar.nonterminals.values() -%}
		{{ nonterminal.name|cc2underscore|upper }},
	{% endfor -%}
		
	NONTEMRINAL_COUNT
};

private:

{% for nonterminal in grammar.nonterminals.values() -%}
Ref<Object> parse{{ nonterminal.name }}();
{% endfor %}

