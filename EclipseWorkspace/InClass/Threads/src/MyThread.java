public class MyThread implements Runnable {
    private int start, stop;
    private Test1 theMain;

    public MyThread(Test1 theMain, int start, int stop)
    {
        this.start = start;
        this.stop = stop;
        this.theMain = theMain;
    }

    @Override
    public void run()
    {
        // System.out.println(start + " " + stop);
        for(int i=start; i<stop; i++)
        {
            System.out.println(i);
        }
        theMain.collectInMain(10);
    }
}