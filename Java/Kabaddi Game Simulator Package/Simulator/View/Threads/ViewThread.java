package Kabaddi.Simulator.View.Threads;

import Kabaddi.Simulator.KabaddiConstants;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author Nagarjuna
 */
public class ViewThread extends Thread{
    
    javax.swing.JPanel panel = null;
    private boolean stopBit;
    public ViewThread(javax.swing.JPanel panel){
        this.panel = panel;
        stopBit = false;
    }
    
    @Override
    @SuppressWarnings("SleepWhileInLoop")
    public void run(){
        while(!stopBit) {
            try {
                Thread.sleep(KabaddiConstants.SLEEP_TIME);
            } catch (InterruptedException ex) {
                Logger.getLogger(ViewThread.class.getName()).log(Level.SEVERE, null, ex);
            }
            panel.repaint();
        }
    }
    
    public void terminate(){
        this.stopBit = true;
    }
}
