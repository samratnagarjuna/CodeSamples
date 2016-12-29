package Kabaddi.Comm;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.Enumeration;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.comm.*;

/**
 *
 * @author Nagarjuna
 */
public class SerialIO {
    private Enumeration portList;
    //private CommPortIdentifier commPortId;
    private SerialPort serialPort;
    private OutputStream outputStream;
    //private InputStream inputStream;
    

    public SerialIO(String commPort){
        getCommPort(commPort);
    }

    private void getCommPort(String commPort){
        portList = CommPortIdentifier.getPortIdentifiers();
        CommPortIdentifier portId = null;
        while (portList.hasMoreElements()) {
            portId = (CommPortIdentifier) portList.nextElement();
            if (portId.getPortType() == CommPortIdentifier.PORT_SERIAL) {
                 if (portId.getName().equals(commPort)) break;
            }
        }
        try {
            serialPort = (SerialPort)portId.open("SimpleWriteApp", 2000);
        } catch (PortInUseException e) {}

        try {
            System.out.print(serialPort);
            outputStream = serialPort.getOutputStream();
        } catch (IOException e) {}

        try {
             serialPort.setSerialPortParams(9600,SerialPort.DATABITS_8,SerialPort.STOPBITS_1, SerialPort.PARITY_NONE);
        } catch (UnsupportedCommOperationException e) {}
    }

   

    public synchronized void sendString(String messageString){
        try {
            outputStream.write(messageString.getBytes());
            System.out.println(messageString);
            try {
                Thread.sleep(10);
            } catch (InterruptedException ex) {
                Logger.getLogger(SerialIO.class.getName()).log(Level.SEVERE, null, ex);
            }
        } catch (IOException e) {}
    }
}
