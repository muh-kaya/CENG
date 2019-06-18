import java.rmi.server.UnicastRemoteObject;
import java.rmi.RemoteException;
import java.util.ArrayList;
import java.util.List;


public class IMazeHubImpl extends UnicastRemoteObject implements IMazeHub {

    private static List<IMaze> mazes = new ArrayList<>();

    public IMazeHubImpl() throws RemoteException {}

    public void createMaze(int width, int height) throws RemoteException {
        IMazeImpl newIMaze = new IMazeImpl();
        newIMaze.create(height, width);
        mazes.add(newIMaze);
    }

    public IMaze getMaze(int index) throws RemoteException {
        if(index < 0 || index >= mazes.size())
            return null;
        else
            return mazes.get(index);
    }

    public boolean removeMaze(int index) throws RemoteException {
        IMaze maze = getMaze(index);
        return mazes.remove(maze);
    }
}
