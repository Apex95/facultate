package homeworkPP;

import java.util.LinkedList;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import homeworkPP.nodes.IfNode;
import homeworkPP.nodes.Node;
import homeworkPP.nodes.WhileNode;

public class Main {

	/**
	 * IMPORTANT! Your solution will have to implement this method.
	 * @param exp - a string, which represents an expression (that
	 * follows the specification in the homework);
	 * @param c - the context (a one-to-one association between variables
	 * and values);
	 * @return - the result of the evaluation of the expression;
	 */
	
	public static Integer evalExpression(String exp, Context c)
	{
		// evaluez folosind contextul dat
		return generateProgramTree(exp, c).evaluate();
	}

	// genereaza un arbore de sintaxa pt inputul dat (program sau expresie)
	// parsez programul pana cand ajung la un element iar apoi generez arborele
	private static Node generateProgramTree(String program, Context context)
	{
		if (program.startsWith("[") && program.endsWith("]"))
		{
			String[] parsedProgram = splitList(program.substring(1, program.length()-1));
			
			Node crtNode = Node.newNode(parsedProgram[0], context);

			// daca e IF, lista se imparte in 3 sau 4 elemente (daca are si ramura pt else)
			if (crtNode instanceof IfNode || crtNode instanceof WhileNode)
			{
				crtNode.conditionChild = generateProgramTree(parsedProgram[1], context);
				crtNode.leftChild = generateProgramTree(parsedProgram[2], context);
				
				// daca are lungime 4 => include ramura de else, pt IF
				if (parsedProgram.length == 4 && crtNode instanceof IfNode)
					crtNode.rightChild = generateProgramTree(parsedProgram[3], context);
				else
					crtNode.rightChild = null;	
			}
			else
			{
				// orice alta instructiune se poate trata in mod general astfel
				crtNode.leftChild = generateProgramTree(parsedProgram[1], context);

				// verific daca functioneaza cu 2 operanzi (return are doar 1)
				if (parsedProgram.length >= 3)
					crtNode.rightChild = generateProgramTree(parsedProgram[2], context);
				else
					crtNode.rightChild = null;
			}
			
			return crtNode;
		}
		else
			return Node.newNode(program, context);
	}
	
	/**
	 * IMPORTANT! Your solution will have to implement this method.
	 * @param program - a string, which represents a program (that
	 * follows the specification in the homework);
	 * @return - the result of the evaluation of the expression;
	 */
	
	public static Integer evalProgram(String program) 
	{
		// evaluez programul incepand cu un context nou
		return generateProgramTree(program, new Context()).evaluate();
	}

	/**
	 * IMPORTANT! Your solution will have to implement this method.
	 * @param program - a string, which represents a program (that
	 * follows the specification in the homework);
	 * @return - whether the given program follow the syntax rules
	 * specified in the homework (always return a value and always
	 * use variables that are "in scope");
	 */

	public static Boolean checkCorrectness(String program) 
	{	
		Node root = generateProgramTree(program, new Context());
		return (new ScopeCheckVisitor().check(root)) && (new ReturnCheckVisitor().check(root));
	}


	/**
	 *
	 * @param s - a string, that contains a list of programs, each
	 * program starting with a '[' and ending with a matching ']'.
	 * Programs are separated by the whitespace caracter;
	 * @return - array of strings, each element in the array representing
	 * a program;
	 * Example: "[* [+ 1 2] 3] [* 4 5]" -> "[* [+ 1 2] 3]" & "[* 4 5]";
	 */
	
	 public static String[] splitList(String s){
		String[] result = new String[0];
		List<String> l = new LinkedList<String>();
        int inside = 0;
        int start = 0, stop = 0;
        for (int i=0; i<s.length(); i++){
                if (s.charAt(i) == '['){
                    inside++;
                    stop++;
                    continue;
                }
                if (s.charAt(i) == ']'){
                    inside--;
                    stop++;
                    continue;
                }
                if (s.charAt(i) == ' ' && inside == 0){
                    l.add(s.substring(start,stop));
                    start = i+1; //starting after whitespace
                    stop = start;

                    continue;
                }
                stop++; //no special case encountered
        }
        if (stop > start) {
            l.add(s.substring(start, stop));
        }

        return l.toArray(new String[l.size()]);

	 }

	public static void main(String[] args) {
		/* Suggestion: use it for testing */
		// no need to
	}
}
