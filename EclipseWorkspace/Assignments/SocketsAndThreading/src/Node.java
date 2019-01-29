import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.net.Socket;

public class Node implements Runnable {

    private Socket s;

    public Node() {
        System.out.println("About to call");

        boolean connected = false;
        s = new Socket();
        while (!connected) {
            try {
                s = new Socket("localhost", 7000);
                connected = true;
            } catch (Exception e) {
                System.out.println("Trying connection again");
            }
        }

        System.out.println("Connected: " + s.getPort());
    }

    @Override
    public void run() {

        try {
            ObjectOutputStream oos = new ObjectOutputStream(s.getOutputStream());
            ObjectInputStream ois = new ObjectInputStream(s.getInputStream());

            int[] vals = (int[]) ois.readObject();

            int start = vals[0], end = vals[1], result = 0;
            System.out.println("Start: " + start + " " + "End: " + end);

            // find number of primes between to numbers
            for (int i = start; i <= end; i++) {
                boolean prime = true; // start by assuming the current number is prime
                for (int j = 2; j < i; j++) { // Loop till j < i
                    if (i % j == 0) {
                        prime = false;
                        break;
                    }
                }
                if (prime) {
                    result += 1; // Add to result
                }
            }

            System.out.println("Result: " + result);
            oos.writeObject(result);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}