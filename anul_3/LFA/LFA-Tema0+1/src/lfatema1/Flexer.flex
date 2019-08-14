package lfatema1;

%% /* user code */

%class Flexer

%standalone
%unicode
%line
%column

%{
	private int CRT_STATE;

	private SymbolsSet symbolsSet = new SymbolsSet();
	SymbolsSet getSymbolsSet() { return symbolsSet; }
	
	private TerminalsSet terminalsSet = new TerminalsSet();
	TerminalsSet getTerminalsSet() { return terminalsSet; }
	
	private RulesSet rulesSet = new RulesSet();
	RulesSet getRulesSet() { return rulesSet; }
	
	private Rule crtRule = null;
	
	private NonTerminal startSymbol;
	
	NonTerminal getStartSymbol() { return startSymbol; }
	
	private boolean reachedFinalState = false;
	private boolean canReadMore = false;
	boolean isValidSyntax() { return reachedFinalState && !canReadMore && (symbolsSet.getSize() > 0); }
	
	private boolean _ruleRHasSymbol = false;
	private boolean _ruleRHasVoid = false;
%}

lineTerminator = \r|\n|\r\n
wordSeparator = {lineTerminator} | [ \t\f]
specialChar =  "`"|"~"|"!"|"@"|"#"|"$"|"%"|"^"|"&"|"*"|"-"|"_"|"+"|"="|"["|"]"|":"|";"|"'"|"<"|"."|">"|"/"|"?"|"\""|"|"
lowerLetter = [a-df-z]
upperLetter = [A-Z]

voidWord = "e"

terminal = {lowerLetter} | [:digit:] | {specialChar}
nonTerminal = {upperLetter}


%state READ_SYMBOLS, READ_SYM_SEPARATOR, READ_SET_SEPARATOR, READ_TERMINALS, NEXT_SET, READ_RULES_L, READ_RULES_R, READ_RULES_SEPARATOR1, READ_RULES_SEPARATOR2, NEXT_RULE, READ_START_SYMBOL, FINAL_STATE, SYN_CHECK

%% /* rules */

{wordSeparator} { }

<YYINITIAL>"(" { CRT_STATE = READ_SYMBOLS; yybegin(READ_SET_SEPARATOR); }
<YYINITIAL>. { yybegin(SYN_CHECK); }

<READ_SYMBOLS> {terminal} { symbolsSet.addTerminal(new Terminal(yytext().charAt(0))); yybegin(READ_SYM_SEPARATOR);}
<READ_SYMBOLS> {nonTerminal} { symbolsSet.addNonTerminal(new NonTerminal(yytext().charAt(0))); yybegin(READ_SYM_SEPARATOR);}
<READ_SYMBOLS>. { yybegin(SYN_CHECK); }


<NEXT_SET> "," { 
					if (CRT_STATE == READ_SYMBOLS)
					{
						CRT_STATE = READ_TERMINALS;
						yybegin(READ_SET_SEPARATOR);
					} 
					else
						if (CRT_STATE == READ_TERMINALS)
						{
							CRT_STATE = READ_RULES_SEPARATOR1;
							yybegin(READ_SET_SEPARATOR);
						}
						else
							if (CRT_STATE == READ_RULES_SEPARATOR1)
							{
								CRT_STATE = READ_START_SYMBOL;
								yybegin(READ_START_SYMBOL);
							}
					
			   }
<NEXT_SET>. { yybegin(SYN_CHECK); }			   


<READ_SYM_SEPARATOR> "," { yybegin(CRT_STATE); }
<READ_SYM_SEPARATOR> "}" { yybegin(NEXT_SET); }
<READ_SYM_SEPARATOR>. { yybegin(SYN_CHECK); }	

<READ_SET_SEPARATOR> "{"{wordSeparator}*"}" { yybegin(NEXT_SET); } 
<READ_SET_SEPARATOR> "{" { yybegin(CRT_STATE); }
<READ_SET_SEPARATOR>. { yybegin(SYN_CHECK); }	

<READ_TERMINALS> {terminal} { terminalsSet.addTerminal(new Terminal(yytext().charAt(0))); yybegin(READ_SYM_SEPARATOR); }
<READ_TERMINALS>. { yybegin(SYN_CHECK); }	



<READ_RULES_SEPARATOR1> "(" { yybegin(READ_RULES_L);}
<READ_RULES_SEPARATOR1>. { yybegin(SYN_CHECK); }	

<READ_RULES_SEPARATOR2> "," { yybegin(READ_RULES_R); }
<READ_RULES_SEPARATOR2>. { yybegin(SYN_CHECK); }	

<READ_RULES_L> {nonTerminal}  { 
								crtRule = new Rule();
								crtRule.setLeft(new NonTerminal(yytext().charAt(0)));
								
								yybegin(READ_RULES_SEPARATOR2);
					  		  }
<READ_RULES_L>. { yybegin(SYN_CHECK); }	
							 
<READ_RULES_R> {terminal} { if (_ruleRHasVoid) yybegin(SYN_CHECK); else {_ruleRHasSymbol = true; crtRule.addRight(new Terminal(yytext().charAt(0))); yybegin(READ_RULES_R); }}
<READ_RULES_R> {nonTerminal} { if (_ruleRHasVoid) yybegin(SYN_CHECK); else {_ruleRHasSymbol = true; crtRule.addRight(new NonTerminal(yytext().charAt(0))); yybegin(READ_RULES_R); }}

<READ_RULES_R> {voidWord} { if (_ruleRHasSymbol) yybegin(SYN_CHECK); else { _ruleRHasVoid = true; crtRule.addRight(new Symbol(yytext().charAt(0))); yybegin(READ_RULES_R);} }

<READ_RULES_R> ")" { _ruleRHasSymbol = false; _ruleRHasVoid = false; rulesSet.addRule(crtRule); yybegin(NEXT_RULE);}
<READ_RULES_R> . { yybegin(SYN_CHECK); } // ceva ce nu e simbol sau paranteza => syntax error

<NEXT_RULE> "," { yybegin(READ_RULES_SEPARATOR1); }
<NEXT_RULE> "}" { yybegin(NEXT_SET); }
<NEXT_RULE> . { yybegin(SYN_CHECK); }	

<READ_START_SYMBOL> {nonTerminal} { startSymbol = new NonTerminal(yytext().charAt(0)); yybegin(FINAL_STATE);}
<READ_START_SYMBOL>. { yybegin(SYN_CHECK); }	

<FINAL_STATE> ")" { reachedFinalState = true; yybegin(SYN_CHECK); }
<FINAL_STATE>. { yybegin(SYN_CHECK); }	

<SYN_CHECK>[^ ]+ { canReadMore = true; }

					 

