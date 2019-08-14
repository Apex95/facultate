%%

%class Flexer
%unicode
/*%debug*/
%int
%line
%column

%{

    Context context = new Context();
    Alphabet alphabet = new Alphabet();

    void operandAction(Expression expression) {
        if(yystate() == EXPO) {
            operatorAction(Expression.Priorities.CONCAT_PRIORITY);
        }
        context.operands.push(expression);
        yybegin(EXPO);
    }

    void operatorAction(Expression.Priorities operator) {
        while(!context.operators.isEmpty()) {
            Expression.Priorities top = context.operators.peek();
            if(top.getPriority() >= operator.getPriority()) {
                context.force();
            } else {
                break;
            }
        }
        context.operators.push(operator);
    }

    Expression getResult() {
        return context.getResult();
    }
%}

LineTerminator = \r|\n|\r\n
WS = {LineTerminator} | [ \t\f]
special =  "!"|"#"|"$"|"%"|"&"|"-"|"."|"/"|":"|";"|"<"|">"|"="|"@"|"["|"]"|"^"|"`"|"~"
lower = [a-d]|[f-z]

Symbol = {lower} | [:digit:] | {special}


%state CHARA SEPA SEP EXP EXPO
/* States:
   **SEPA: reads the separator inside the alphabet
   **CHARA: reads a symbol(character) of the alphabet
   **SEP: reads the separator between the alphabet and the expression
   **EXP: an operator is needed here because it's either in the beginning of
   ****the file, or after a binary operator
   **EXPO: the state after reading an operand. If another one follows, they
   ****are treated like the operands of a concatenation.
*/

%%

{WS}	{/*Skip whitespace in any state*/}
<YYINITIAL>"{"	{//Read the alphabet
    yybegin(CHARA);}

<CHARA> {Symbol}	{//Read one symbol at a time
    yybegin(SEPA);
    String symbol = yytext();
    alphabet.addSymbol(symbol.charAt(0));
}

<SEPA> { //Read a separator and decide what is next
    ","				{yybegin(CHARA);}
    "}"			{yybegin(SEP);}
}

<SEP>":" { //This colon must be read
    yybegin(EXP);}

<EXP,EXPO> { //An operand can appear anywhere
    "e"					{operandAction(new NullExpression());}
    "O"					{operandAction(new VoidExpression());}
    {Symbol}			{operandAction(new SymbolExpression(yycharat(0)));}
    "("					{context = context.enter(yystate()); yybegin(EXP);}
}
<EXPO> { //An operator can only appear after an operand
    "|"					{operatorAction(Expression.Priorities.UNION_PRIORITY); yybegin(EXP);}
    "*"					{operatorAction(Expression.Priorities.KLEENE_PRIORITY);}
    "+"					{operatorAction(Expression.Priorities.PLUS_PRIORITY); }
    "?"					{operatorAction(Expression.Priorities.OPTIONAL_PRIORITY); }
    ")"					{context = context.exit(); yybegin(context.state);
        operandAction(context.operands.pop());}
}
