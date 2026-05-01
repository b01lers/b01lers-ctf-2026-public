import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;

public class Yuri {

    public static void main(String[] args) {
        try (BufferedReader reader1 = new BufferedReader(new FileReader("yuri_1.txt"));
             BufferedReader reader2 = new BufferedReader(new FileReader("yuri.txt"))) {

            while (true) {
                int char1 = reader1.read();
                int char2 = reader2.read();
                if (char1 == -1 || char2 == -1) {
                    break;
                }

                // When the difference between two characters isn't 0, it is a
                // valid character. 
                if (char1 - char2 == 0) {
                    continue;
                }

                System.out.printf("%c", char1 - char2);
            }
        } catch (IOException e) {
            System.err.println("Error reading files: " + e.getMessage());
        }
        System.out.print("\n");

    }
}
