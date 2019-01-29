import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.net.Socket;

public class Node {
    public static void main(String[] args) throws Exception {
        System.out.println("About to call");
        Socket s = new Socket("localhost",7000);
        System.out.println("Connected.");

        ObjectOutputStream oos = new ObjectOutputStream(s.getOutputStream());
        ObjectInputStream ois = new ObjectInputStream(s.getInputStream());
        oos.writeObject(5);

        int start=0, end=0, result=0;

        // find number of primes between to numbers
        for(int i=start; i<= end; i++) {
            boolean prime = true;  //start by assuming the current number is prime
            for(int j=2; j<i; j++) { // Loop till j < i
                if(i%j == 0) {
                  prime = false; //Set the current number as not prime if it is divisible by any number lesser than it
                }
           }
           if (prime) {
               result += 1;   //Add to result
           }
        }
    }
}