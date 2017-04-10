import java.net.*;

public class Z2Receiver
{
static final int datagramSize=50;
InetAddress localHost;
int destinationPort;
DatagramSocket socket;

ReceiverThread receiver;

byte[][] sended = new byte[512][5];
byte[][] received = new byte[512][5];
int lastPrinted = 0;
boolean gap = false;

public Z2Receiver(int myPort, int destPort)
throws Exception
    {
    localHost=InetAddress.getByName("127.0.0.1");
    destinationPort=destPort;
    socket=new DatagramSocket(myPort);
    receiver=new ReceiverThread();    
    for (int i = 0; i < 512; i++) {
    	received[i][0] = -1;
    }
    }

class ReceiverThread extends Thread
{

public void run()
    {
    try
	{
	    while(true)
		{
		    byte[] data=new byte[datagramSize];
		    DatagramPacket packet= 
			new DatagramPacket(data, datagramSize);
		    socket.receive(packet);
                    Z2Packet p=new Z2Packet(packet.getData());
            int idx = p.getIntAt(0);
            if (received[idx][0] == idx) {
            	packet.setPort(destinationPort);
                socket.send(packet);
        		//System.out.println("Receiver - Duplikat | " + p.getIntAt(0) + " - " + (char) p.data[4] + " || lastPrinted = " + lastPrinted);
        	}
            else {
            received[idx][0] = (byte)((idx)&0xFF);
            for (int i = 1; i <= 4; i++) {
                   	received[idx][i] = p.data[i];
            }
            if (p.getIntAt(0) == 0) {
            	lastPrinted = 0;
            	//System.out.println("Receiver - Otrzymano poczatkowy pakiet | " + received[idx][0] + " = " +(char) received[idx][4]);
            	int i = 0;
        		System.out.print("R: ");
        		while (received[i][0] != -1) {
        			System.out.print((char) received[i][4] + " ");
        			lastPrinted = i;
        			i++;
        		}
        		System.out.println("");
        		//System.out.println("Receiver - lastPrinted zwiekszone do " + lastPrinted);
            }
            else {
            	for (int i = idx; i >= lastPrinted; i--) { //> na końcu trzeba bedzie zamienić na >=
            		if (received[i][0] == -1) {
            			gap = true;
            			break;
            		}
            		else {
            			gap = false;
            		}
            	}
            	if (gap == false) {
            		//System.out.println("Nie ma przerwy, mozna drukować od lastPrinted = " + lastPrinted + " do " + idx);
            		int i = idx;
            		System.out.print("R: ");
            		while (received[i][0] != -1) {
            			System.out.print((char) received[i][4] + " ");
            			lastPrinted = i;
            			i++;
            		}
            		System.out.println("");
            		//System.out.println("Receiver - lastPrinted zwiekszone do " + lastPrinted);
            	}
            	else {
            		//System.out.println("Jest przerwa miedzy lastPrinted = " + lastPrinted + " a " + idx + " lost:");
            		for (int i = 0; i <= idx; i++) {
            			if(received[i][0] != i) {
            				//System.out.print(i + " ");
            			}
            		}
            		//System.out.println("");
            		gap = false;
            	}            	
            }
                 // WYSLANIE POTWIERDZENIA
                 packet.setPort(destinationPort);
                 socket.send(packet);
            }	    	
		}
	}
    catch(Exception e)
	{
        System.out.println("Z2Receiver.ReceiverThread.run: "+e);
	}
    }

}

public static void main(String[] args)
throws Exception
    {
	Z2Receiver receiver=new Z2Receiver( Integer.parseInt(args[0]),
				   Integer.parseInt(args[1]));
	receiver.receiver.start();
    }


}
