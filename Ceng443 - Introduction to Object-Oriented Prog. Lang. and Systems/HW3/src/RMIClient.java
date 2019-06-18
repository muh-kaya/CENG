import java.util.Scanner;
import java.rmi.*;
import java.io.IOException;

public class RMIClient {
    private static void printOperationValues(boolean flag) {
        if(flag == true) {
            System.out.println("Operation Success.");
        }
        else {
            System.out.println("Operation Failed.");
        }
    }

    public static void main(String[] args) throws RemoteException, NotBoundException, IOException{

        IMazeHub server = (IMazeHub) Naming.lookup("mazeservice");
        IMaze selectedMaze = null;
        Object[] param;
        boolean flag;

        Scanner scanner = new Scanner(System.in);
        ParsedInput parsedInput = null;
        String input;
        while( true ) {
            input = scanner.nextLine();
            try {
                parsedInput = ParsedInput.parse(input);
            }
            catch (Exception ex) {
                parsedInput = null;
            }
            if ( parsedInput == null ) {
                System.out.println("Wrong input format. Try again.");
                continue;
            }
            param = parsedInput.getArgs();
            if((selectedMaze == null) && (!((parsedInput.getType() == ParsedInputType.CREATE_MAZE)
                || (parsedInput.getType() == ParsedInputType.DELETE_MAZE)
                || (parsedInput.getType() == ParsedInputType.SELECT_MAZE)
                || (parsedInput.getType() == ParsedInputType.QUIT))))
            {
                printOperationValues(false);
                continue;
            }
            switch(parsedInput.getType()) {
                case CREATE_MAZE:
                    server.createMaze((int) param[0], (int) param[1]);
                    break;
                case DELETE_MAZE:
                    flag = server.removeMaze((int) param[0]);
                    printOperationValues(flag);
                    selectedMaze = null;
                    break;
                case SELECT_MAZE:
                    selectedMaze = server.getMaze((int) param[0]);
                    if(selectedMaze != null){
                      printOperationValues(true);
                    }
                    else{
                      printOperationValues(false);
                    }
                    break;
                case PRINT_MAZE:
                    String mazePrint = selectedMaze.print();
                    System.out.print(mazePrint);
                    break;
                case CREATE_OBJECT:
                    flag = selectedMaze.createObject(new Position((int) param[0], (int) param[1]), (MazeObjectType) param[2]);
                    printOperationValues(flag);
                    break;
                case DELETE_OBJECT:
                    flag = selectedMaze.deleteObject(new Position((int) param[0], (int) param[1]));
                    printOperationValues(flag);
                    break;
                case LIST_AGENTS:
                    Agent[] agents = selectedMaze.getAgents();
                    for(int i=0; i<agents.length; i++){
                        System.out.printf("Agent%d at %s. Gold collected: %d\n", agents[i].getId(), agents[i].getPosition(), agents[i].getCollectedGold());
                    }
                    break;
                case MOVE_AGENT:
                    flag = selectedMaze.moveAgent((int) param[0], new Position((int) param[1], (int) param[2]));
                    printOperationValues(flag);
                    break;
                case QUIT:
                    return;
            }
        }
    }
}
