import java.rmi.*;
import java.io.IOException;

public class RMIServer {
    public static void main(String[] args) throws RemoteException, IOException{
        IMazeHub server = new IMazeHubImpl();
        Naming.rebind("mazeservice", server);
    }
}
