package test;

import java.io.FileNotFoundException;
import java.io.PrintWriter;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author Stefan
 */
public class TestWriter {
    
    private PrintWriter out = null;

    public TestWriter(String filename) {
        try {
            out = new PrintWriter(filename);
        } catch (FileNotFoundException ex) {
            Logger.getLogger(TestWriter.class.getName()).log(Level.SEVERE, null, ex);
        }
    }
    
    public void printCount(int count) {
        out.println(count);
    }
    
    // mica modificare aici, ca sa-mi fie mai usor sa afisez
    public void printWord(String word) {
    	out.print(word);
    }
    
    public void close() {
        out.close();
    }
    
    
}
