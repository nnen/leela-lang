# -*- coding: utf-8 -*-

import sys
import re
import logging

import ply.lex as lex
import ply.yacc as yacc

from jinja2 import Environment, FileSystemLoader

__doc__ = "Very simple C++ parser generator script." 
__author__ = "Jan Milik <milikjan@fit.cvut.cz>"

logging.basicConfig(
    level = logging.DEBUG,
    filename = "parselog.txt",
    filemode = "w",
    format = "%(filename)10s:%(lineno)4d:%(message)s"
)

class Grammar(object):
	def __init__(self):
		self.nonterminals = {}
	
	def __str__(self):
		result = ""
		for nonterminal in self.nonterminals.values():
			result += nonterminal.print_table()
		return result
	
	def update(self):
		nonterminals = self.nonterminals.values()
		for nonterm in nonterminals:
			nonterm.update_first()
		for nonterm in nonterminals:
			nonterm.update_first()
		for nonterm in nonterminals:
			nonterm.update_first()
		for nonterm in nonterminals:
			nonterm.update_first()

class Symbol(object):
	def __init__(self, name):
		self.name = unicode(name)
	
	def __str__(self):
		return self.name
	
	def __hash__(self):
		return hash(self.name)

class Terminal(Symbol):
	TERMINALS = {
		'minus'     : '-',
		'plus'      : '+',
		'asterix'   : '*',
		'slash'     : '/',
		
		'left_par'  : '(',
		'right_par' : ')',
		'left_bra'  : '[',
		'right_bra' : ']',
		'semicolon' : ';',
		'colon'     : ':',
		'comma'     : ',',

		'assign'    : ':=',
		'equals'    : '=',
	}
	
	CAPTURE = set([
		'ident',
		'number',
	])
	
	def __str__(self):
		if self.name in self.TERMINALS:
			return repr(self.TERMINALS[self.name])
		return self.name
	
	def get_first(self):
		return set([self, ])
	
	def maybe_empty(self):
		return self.name == 'epsilon'

class Nonterminal(Symbol):
	def __init__(self, name):
		Symbol.__init__(self, name)
		self.children = []
		self.table = {}
		self.rules = {}
		self.annotations = {}
	
	def print_table(self):
		result = "%s:\n" % self.name
		#for terminal, child in self.table.items():
		#	result += "\t%s : %s\n" % (terminal, child)
		for child, terminals in self.rules.items():
			for terminal in terminals:
				result += "\t%s:\n" % terminal
			result += "\t\t%s\n" % child
		return result
	
	def maybe_empty(self):
		for child in self.children:
			if child.maybe_empty():
				return True
		return False
	
	def update_first(self):
		self.table = {}
		self.rules = {}
		for child in self.children:
			for terminal in child.get_first():
				self.table[terminal] = child
				self.rules.setdefault(child, set([])).add(terminal)
	
	def get_first(self):
		return set(self.table.keys())

class Action(Symbol):
	def maybe_empty(self):
		return True
	
	def get_first(self):
		return set([])

class String(object):
	def __init__(self):
		self.children = []
	
	def __str__(self):
		return " ".join([str(child) for child in self.children])
	
	def get_first(self):
		first = set([])
		for child in self.children:
			first.update(child.get_first())
			if not child.maybe_empty():
				break
		if not self.maybe_empty():
			first.discard(Terminal("epsilon"))
		return first
	
	def maybe_empty(self):
		for child in self.children:
			if not child.maybe_empty():
				return False
		return True

class Parser(object):
	states = [
		('annotation', 'exclusive'),
	]
	
	tokens = [
		'TERMINAL',
		'NONTERMINAL',
		'ACTION',
		'RULE',
		'ALTERNATION',
		'PERIOD',
		'ANNOTATION',
		'ANNOTATION_END',
		'LABEL',
		'IDENT',
		'COMMENT',
	]
	
	t_RULE = r'->'
	t_ALTERNATION = r'\|'
	t_PERIOD = r'\.'
	
	t_ignore_COMMENT = r'\/\*(.|\n)*?\*\/'
	t_ignore = ' \t'
	t_annotation_ignore = ' \t'
	
	def t_TERMINAL(self, t):
		r'[a-z][a-z_]*'
		t.value = Terminal(t.value)
		return t
	
	def t_NONTERMINAL(self, t):
		r'[A-Z][A-Za-z_]*'
		name = t.value
		t.value = self.nonterminals.setdefault(name, Nonterminal(name))
		return t
	
	def t_ACTION(self, t):
		r'\![A-Za-z][A-Za-z0-9_-]*'
		t.value = Action(t.value[1:])
		return t
	
	def t_ANNOTATION(self, t):
		r'\/\*\!'
		t.lexer.push_state('annotation')
	
	def t_annotation_ANNOTATION_END(self, t):
		r'\*\/'
		t.lexer.pop_state()
	
	def t_annotation_LABEL(self, t):
		r'[a-zA-Z][a-zA-Z0-9]*\:'
		t.value = t.value[:-1]
		return t
	
	def t_annotation_IDENT(self, t):
		r'[a-zA-Z][a-zA-Z0-9<>]*'
		return t
	
	def t_newline(self, t):
		r'\n+'
		t.lexer.lineno += len(t.value)
	
	def t_error(self, t):
		print "Illegal character: %r" % t.value[0]
		t.lexer.skip(1)
	
	def t_annotation_error(self, t):
		print "Illegal character in annotation state: %r" % t.value[0]
		t.lexer.skip(1)
	
	def p_grammar(self, p):
		'''grammar : grammar definition
		           | definition'''
		if len(p) > 2:
			p[0] = p[1]
			p[0].nonterminals[p[2].name] = p[2]
		else:
			p[0] = Grammar()
			p[0].nonterminals[p[1].name] = p[1]
		return p[0]
	
	def p_definition(self, p):
		'''definition : annotation NONTERMINAL RULE rule_expr PERIOD'''
		p[0] = p[2]
		p[0].children.extend(p[4])
		p[0].annotations = p[1]
		return p[0]
	
	def p_definition_error(self, p):
		'''definition : error NONTERMINAL RULE rule_expr PERIOD'''
		print "Invalid rule annotation."
	
	def p_annotation(self, p):
		'''annotation : annotation LABEL IDENT
		              | epsilon'''
		if len(p) > 2:
			p[0] = p[1]
			p[0][p[2]] = p[3]
		else:
			p[0] = {}
		return p[0]
	
	def p_rule_expr(self, p):
		'''rule_expr : rule_expr ALTERNATION string
		             | string'''
		if len(p) > 2:
			p[0] = p[1]
			p[0].append(p[3])
		else:
			p[0] = [p[1], ]
		return p[0]
	
	def p_string(self, p):
		'''string : string TERMINAL
		          | string NONTERMINAL
		          | string ACTION
		          | TERMINAL
                | NONTERMINAL
		          | epsilon'''
		if len(p) > 2:
			p[0] = p[1]
			p[0].children.append(p[2])
		else:
			if p[1] is None:
				p[0] = String()
				p[0].children.append(Terminal("epsilon"))
			else:
				p[0] = String()
				p[0].children.append(p[1])
		return p[0]
	
	def p_epsilon(self, p):
		'epsilon :'
		pass
	
	def p_error(self, p):
		print "Syntax error in input on line %d, column %d." % (p.lineno, p.lexpos)
		# print "Error at token: %r" % self.parser.token()
		print "Error at token: %r" % yacc.token()
	
	def __init__(self):
		self.lexer = lex.lex(module = self)
		self.parser = yacc.yacc(module = self, debug = True)
		self.nonterminals = {}
	
	def parse(self, input):
		self.nonterminals = {}
		return self.parser.parse(input, debug = logging.getLogger())

	def dump_tokens(self, input):
		self.lexer.input(input)
		while True:
			token = self.lexer.token()
			if not token: break
			print token

class Generator(object):
	def __init__(self):
		self.env = Environment(loader = FileSystemLoader('.'))
		def cc2underscore(name):
			s1 = re.sub('(.)([A-Z][a-z]+)', r'\1_\2', name)
			return re.sub('([a-z0-9])([A-Z])', r'\1_\2', s1).lower()	
		self.env.filters['cc2underscore'] = cc2underscore
	
	def generate_header(self, grammar, output):
		#self.env.get_template("parsing.tpl.h").stream(grammar = grammar).dump(output)
		#self.env.get_template("parsing.tpl.h").stream(grammar = grammar).dump("parsing.h")
		result = self.env.get_template("Parser.rules.tpl.h").render(
			grammar = grammar,
			Terminal = Terminal,
			Nonterminal = Nonterminal,
		)
		f = open(output, mode = 'w')
		f.write(result)
		f.close()
	
	def generate_rules(self, grammar, output):
		#self.env.get_template(u"parsing.tpl.cpp").stream(
		#	grammar     = grammar,
		#	isinstance  = isinstance,
		#	Terminal    = Terminal,
		#	Nonterminal = Nonterminal,
		#).dump(output)
		result = self.env.get_template("Parser.rules.tpl.cpp").render(
			grammar = grammar,
			isinstance = isinstance,
			Terminal = Terminal,
			Nonterminal = Nonterminal,
		)
		f = open(output, mode = 'w')
		f.write(result)
		f.close()

if __name__ == "__main__":
	parser = Parser()
	#parser.dump_tokens(sys.stdin.read())
	grammar = parser.parse(sys.stdin.read())
	grammar.update()
	# grammar.generate_header(sys.stdout)
	# grammar.generate_rules(sys.stdout)
	generator = Generator()
	generator.generate_header(grammar, u"Parser.rules.h")
	generator.generate_rules(grammar, u"Parser.rules.cpp")

