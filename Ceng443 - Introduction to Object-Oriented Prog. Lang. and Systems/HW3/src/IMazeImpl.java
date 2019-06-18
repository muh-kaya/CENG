import java.rmi.server.UnicastRemoteObject;
import java.rmi.RemoteException;
import java.util.ArrayList;
import java.util.List;

public class IMazeImpl extends UnicastRemoteObject implements IMaze {

    private List<MazeObject> mazeObjects;
    private List<Agent> agents;
    private int height;
    private int width;

    public IMazeImpl() throws RemoteException {}

    @Override
    public void create(int height, int width) throws RemoteException {
        this.height = height;
        this.width = width;
        this.mazeObjects = new ArrayList<>();
        this.agents = new ArrayList<>();
    }

    @Override
    public MazeObject getObject(Position position) throws RemoteException {
        for(int i=0; i<mazeObjects.size(); i++){
            if((mazeObjects.get(i).getPosition().getX() == position.getX()) && (mazeObjects.get(i).getPosition().getY() == position.getY())){
                return mazeObjects.get(i);
            }
        }
        return null;
    }

    @Override
    public boolean createObject(Position position, MazeObjectType type) throws RemoteException {
        MazeObject mazeObject = getObject(position);
        if(mazeObject == null) {
          if(type == MazeObjectType.AGENT){
            Agent newMazeObject = new Agent(position, agents.size());
            mazeObjects.add(newMazeObject);
            agents.add(newMazeObject);
          }
          else{
            MazeObject newMazeObject = new MazeObject(position, type);
            mazeObjects.add(newMazeObject);
          }
          return true;
        }
        return false;
    }

    @Override
    public boolean deleteObject(Position position) throws RemoteException {
        MazeObject mazeObject = getObject(position);
        if(mazeObject == null){
            return false;
        }
        else {
            mazeObjects.remove(mazeObject);
            if(mazeObject.getType() == MazeObjectType.AGENT)
                agents.remove(mazeObject);
            return true;
        }
    }

    @Override
    public Agent[] getAgents() throws RemoteException {
        Agent[] agentArray = agents.toArray(new Agent[agents.size()]);
        return agentArray;
    }

    @Override
    public boolean moveAgent(int id, Position position) throws RemoteException {
        Agent agent = null;

        for(int i=0; i<agents.size(); i++){
            if(agents.get(i).getId() == id){
                agent = agents.get(i);
            }
        }

        if(agent == null){
            return false;
        }

        if(agent.getPosition().distance(position) == 1){
          MazeObject mazeObject = getObject(position);
          if(mazeObject == null){
              agent.setPosition(position);
          }
          else if(mazeObject.getType() == MazeObjectType.HOLE){
              mazeObjects.remove(agent);
              agents.remove(agent);
          }
          else if(mazeObject.getType() == MazeObjectType.GOLD){
              mazeObjects.remove(mazeObject);
              agent.setPosition(position);
              agent.incCollectedGold();
          }
          return true;
        }
        return false;
    }

    @Override
    public String print() throws RemoteException {
        StringBuilder maze = new StringBuilder("+");
        for(int k=0; k<width; k++){
            maze.append("-");
        }
        maze.append("+\n");

        for(int i=0; i<height; i++){
            maze.append("|");
            for(int j=0; j<width; j++){
                MazeObject mazeObject = getObject(new Position(j, i));
                if(mazeObject == null){
                    maze.append(" ");
                }
                else{
                  maze.append(mazeObject.toString());
                }
            }
            maze.append("|\n");
        }
        maze.append("+");
        for(int k=0; k<width; k++){
            maze.append("-");
        }
        maze.append("+\n");

        return maze.toString();
    }
}
