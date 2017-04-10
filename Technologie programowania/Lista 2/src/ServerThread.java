import java.io.BufferedReader;
 
import java.io.EOFException;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;
 
 
import java.util.ArrayList;
import java.util.Arrays;
 
class MyException1 extends Exception {};
class MyException2 extends Exception {};
 
 
public class ServerThread extends Thread {
         
        Socket client = null;
        SocketServer server = null;
        int numberOfDisconnectedClients = 0;
        String[] tableOfDisconnectedClients = new String[server.getLimitOfPlayers()];
         
        //As parameter takes whole SocketServer Object and Socket
        public ServerThread (SocketServer s, Socket socket) {
            this.server = s;
            this.client = socket;
            start();
        }
             
             
         
        public void run() {
             
            try {
                BufferedReader in = new BufferedReader(new InputStreamReader(
                        client.getInputStream()));
             
                while (true) {
                 
                    server.message = in.readLine();
                    System.out.println("Nas�uchuj� na nowe informacje");
                     
                    server.table.interpreter(server.message);
                     
                    //server.broadcast(message); <--- This is moved to interpreter from Table
                 
                }
            }
                     
            catch (IOException e) {
                    System.out.println("Read line");
            } 
             
            catch (NumberFormatException e) {
                    // TODO Auto-generated catch block
                    e.printStackTrace();
            } 
            /*catch (NullPointerException e) {
                System.out.println("Klient sie odl�czy�, dalsza obs�uga roz��czenia...");
                server.broadcast("Disconnected-0-0-0-0-0");
                 
            }*/
             
            // EOFException must be added
            finally {
                    System.out.println("finally executed!!!!!!!!!!!!!!!!!!!!!!");
                    //server.removeConnection(client);
                 
            }
        }
         
}