
public class Test1 {
    int globalx;

    public static void main(String[] args) throws Exception {
        new Test1();
    }

    public Test1() {
        globalx = 0;

        Thread[] ths = new Thread[4];
        for (int i = 0; i < 4; i++) {
            MyThread myTh = new MyThread(this, i * (10 / 4), (i + 1) * (10 / 4));

            Thread th = new Thread(myTh);
            th.start();
            ths[i] = th;

            // th.join(); // don't put this here, tells each thread to wait immediately
            // which is useless
        }

        // look through, make sure all threads are finished before continuing
        for (int i = 0; i < 4; i++) {
            try {
                ths[i].join();
            } catch (Exception e) {
                e.printStackTrace();
            }
        }

        System.out.println("In main");
        System.out.println("Globalx: " + globalx);

    }

    public synchronized void collectInMain(int x) { // synchronized to avoid race conditions with threads
        globalx = globalx + x;
    }
}