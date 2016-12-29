package Kabaddi.Simulator.View;


import Kabaddi.Controller.Cordinator;
import Kabaddi.Engine.Engine;
import Kabaddi.Simulator.KabaddiConstants;
import Kabaddi.Simulator.View.Threads.ViewThread;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.border.TitledBorder;

/**
 *
 * @author Nagarjuna
 */
public class DataView extends javax.swing.JPanel implements ActionListener{
    
    
    private final java.awt.Color BACKGROUND_COLOR = java.awt.Color.LIGHT_GRAY;
    private final javax.swing.border.Border RAISED_ETCHED_BORDER = javax.swing.BorderFactory.createEtchedBorder(javax.swing.border.EtchedBorder.RAISED);
    private final javax.swing.border.Border LOWER_ETCHED_BORDER = javax.swing.BorderFactory.createEtchedBorder(javax.swing.border.EtchedBorder.LOWERED);
    private final TitledBorder CONTROL_TITLED_BORDER = javax.swing.BorderFactory.createTitledBorder(LOWER_ETCHED_BORDER, "Controls");
    private final TitledBorder STATUS_TITLED_BORDER = javax.swing.BorderFactory.createTitledBorder(LOWER_ETCHED_BORDER, "Game Status");
    private final JLabel DATA_HEADER = new JLabel("KABADDI SIMULATION");
    private final javax.swing.JPanel controllerPanel = new javax.swing.JPanel();    
    private final JButton bt_start = new JButton("Start");
    private final JButton bt_pr = new JButton("Play");
    private final JButton bt_reset = new JButton("Reset");    
    private StatusView statusPanel = null;
    private Engine engineX = null;
    private Engine engineY = null;
    private Cordinator cordinator = null;    
    
    public DataView(Engine engineX,Engine engineY){
        this.engineX = engineX;
        this.engineY = engineY;
        initDataViewComponents();
        addDataViewComponents();
        addControllerComponents();
        startStatusThread();
    }
    
    private void initDataViewComponents() {
        setPreferredSize(new java.awt.Dimension(KabaddiConstants.DATA_PANEL_WIDTH, KabaddiConstants.DATA_PANEL_HEIGHT));
        setBackground(BACKGROUND_COLOR);
        setBorder(RAISED_ETCHED_BORDER);
        CONTROL_TITLED_BORDER.setTitleJustification(TitledBorder.CENTER);
        STATUS_TITLED_BORDER.setTitleJustification(TitledBorder.CENTER);
        controllerPanel.setPreferredSize(new java.awt.Dimension(KabaddiConstants.CONTROL_PANEL_WIDTH,KabaddiConstants.CONTROL_PANEL_HEIGHT));
        controllerPanel.setBorder(CONTROL_TITLED_BORDER);
        controllerPanel.setBackground(BACKGROUND_COLOR);
        statusPanel = new StatusView(engineX, engineY);
        statusPanel.setPreferredSize(new java.awt.Dimension(KabaddiConstants.STATUS_PANEL_WIDTH,KabaddiConstants.STATUS_PANEL_HEIGHT));
        statusPanel.setBorder(STATUS_TITLED_BORDER);
        statusPanel.setBackground(BACKGROUND_COLOR);
        bt_start.setActionCommand("Start");
        bt_pr.setActionCommand("Play");
        bt_reset.setActionCommand("Reset");
        bt_reset.setEnabled(false);
        bt_start.addActionListener(this);
        bt_pr.addActionListener(this);
        bt_reset.addActionListener(this);        
    
    }

    private void addDataViewComponents() {
        add(DATA_HEADER);
        add(controllerPanel);
        add(statusPanel);
    }
    
    private void addControllerComponents(){        
        controllerPanel.add(bt_start);
        controllerPanel.add(bt_pr);
        controllerPanel.add(bt_reset);        
    }

    @Override
    public void actionPerformed(ActionEvent e) {
        String command = e.getActionCommand();
        
        if(command.contentEquals("Start")){                        
            startNewGame();            
        }
        else if(command.contentEquals("Play")){
            if(cordinator != null && !cordinator.PLAY) {
                cordinator.PLAY = true;                
            }
        }
        else if(command.contentEquals("Reset")){
            
        }
        else if(command.contentEquals("Pause")){
            
        }
        else if(command.contentEquals("Resume")){
            
        }
    }
    
    private void startNewGame(){
        if(cordinator != null) {
            cordinator.terminate();
            try {
                Thread.sleep(KabaddiConstants.BREAK_TIME);
            } catch (InterruptedException ex) {
                Logger.getLogger(DataView.class.getName()).log(Level.SEVERE, null, ex);
            }
        }        
        //This is wrong you have to pass cloned engines this is temporary
        cordinator = new Cordinator(this.engineX,this.engineY);
        cordinator.start();        
    }

    private void startStatusThread() {
        ViewThread statusThread = new ViewThread(statusPanel);
        statusThread.start();        
    }
    
}
