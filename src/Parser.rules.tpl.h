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

/* Nonterminals ****************************************/

{% for nonterminal in grammar.nonterminals.values() -%}
Ref<Object> parse{{ nonterminal.name }}(Ref<Object> inherited, vector<Ref<Object> > siblings);
{% endfor %}

/* Semantic actions ************************************/

{% for action in grammar.actions.values() -%}
void {{ action.name }}(
	Ref<Object> inherited,
	Match siblings,
	Match& matched,
	Ref<Object>& result);
{% endfor %}

