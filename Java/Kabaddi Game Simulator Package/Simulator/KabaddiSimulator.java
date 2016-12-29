package Kabaddi.Simulator;

import Kabaddi.Engine.Engine;
import Kabaddi.Simulator.View.SimulatorView;
import javax.swing.WindowConstants;

/**
 *
 * @author Nagarjuna
 */
public class KabaddiSimulator {
    
    javax.swing.JFrame simulatorFrame = null;
    private Engine engineX = null;
    private Engine engineY = null;
    public static boolean HARDWARE_MODE = false;
    public static boolean LOGGER_MODE = false;
    public static String commPortID = null;
    
    public KabaddiSimulator(Engine engineX,Engine engineY){
       this.engineX = engineX;
       this.engineY = engineY;
       simulatorFrame = new javax.swing.JFrame("Kabaddi Simulator");       
    }
    
    public void enableHardwareMode(boolean mode,String PortID){
        HARDWARE_MODE = mode;
        commPortID = PortID;
    }
    
    public void enableLogs(boolean mode){
        LOGGER_MODE = mode;
    }
    public void run(){
       initSimulatorComponents(); 
    }

    private void initSimulatorComponents() {
        simulatorFrame.setLocation(150, 150);        
        simulatorFrame.setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);
        simulatorFrame.setLayout(new java.awt.BorderLayout());
        simulatorFrame.setVisible(true);
        simulatorFrame.getContentPane().add(new SimulatorView(this.engineX,this.engineY),java.awt.BorderLayout.CENTER);
        simulatorFrame.pack();
    }   
}
