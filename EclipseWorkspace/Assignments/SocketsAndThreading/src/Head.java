import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.net.ServerSocket;
import java.net.Socket;

public class Head {
    static private int connections = 3;

    public static void main(String[] args) throws Exception {
        try {
            int connected = 0;

            Socket[] sockets = new Socket[connections];

            ServerSocket ss = new ServerSocket(7000);
            while (connected < connections) {
                Socket s = ss.accept();
                sockets[connected] = s;
                connected++;
                System.out.println("Accepted: " + connected);
            }

            for (int i = 0; i < sockets.length; i++) {
                ObjectInputStream ois = new ObjectInputStream(sockets[i].getInputStream());
                ObjectOutputStream oos = new ObjectOutputStream(sockets[i].getOutputStream());
            }

        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}