import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.Date;

public class Head {
    static private int connections = 4;
    private static int start=1000, end=1000000;
    private static float startTime;
    private static Thread[] ths;

    public static void main(String[] args) throws Exception {
        try {
            int connected = 0;
            ths = new Thread[connections];

            // array of connected sockets
            Socket[] sockets = new Socket[connections];

            // setting up serversocket
            ServerSocket ss = new ServerSocket(7000);

            // waiting for connections and threads for each connected socket
            while (connected < connections) {
                Node n = new Node();

                Thread th = new Thread(n);
                ths[connected] = th;

                System.out.println("Created thread: " + connected);

                Socket s = ss.accept();
                sockets[connected] = s;
                connected++;
                System.out.println("Accepted: " + connected);
            }

            // calculating range for each node
            int rangePerNode = (end - start) / connections;

            System.out.println("All nodes connected, starting timer");
            long StartTime = new Date().getTime();
            int result = 0;

            // starting all threads
            for(int i=0; i<ths.length; i++)
            {
                ths[i].start();
            }

            // writing ranges to connected sockets
            ObjectInputStream[] oisArray = new ObjectInputStream[connections];
            for (int i = 0; i < sockets.length; i++) {
                ObjectInputStream ois = new ObjectInputStream(sockets[i].getInputStream());
                ObjectOutputStream oos = new ObjectOutputStream(sockets[i].getOutputStream());
                oos.writeObject(new int[]{start + (i*rangePerNode), (start + ((i+1)*rangePerNode)-1)});
                oisArray[i] = ois;
            }

            // join all threads to make sure everything has finished
            for (int i = 0; i < 4; i++) {
                try {
                    ths[i].join();
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }

            System.out.println("All threads complete");

            // calculate total result
            for(int i=0; i<oisArray.length; i++)
            {
                result += (int)oisArray[i].readObject();
            }

            long EndTime = new Date().getTime();
            System.out.println("Result: " + result);
            System.out.println("Total time: " + (EndTime-StartTime)*0.001 + " Seconds");

            // Total time unthreaded: 131.072 seconds
            // Total time threaded: 77.877 seconds

        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}