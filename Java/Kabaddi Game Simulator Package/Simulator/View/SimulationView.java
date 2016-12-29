package Kabaddi.Simulator.View;

import Kabaddi.Simulator.View.Threads.ViewThread;
import Kabaddi.Engine.Engine;
import Kabaddi.Simulator.KabaddiConstants;
import java.awt.Color;
import javax.swing.JLabel;
import javax.swing.JPanel;

/**
 *
 * @author Nagarjuna
 */
public class SimulationView extends javax.swing.JPanel{
    
    private final javax.swing.border.Border PANEL_BORDER = javax.swing.BorderFactory.createEtchedBorder(javax.swing.border.EtchedBorder.RAISED);    
    private JPanel northPanel = null;
    private JPanel southPanel = null;
    private EastDeskView eastPanel = null;
    private WestDeskView westPanel = null;
    private FieldView fieldPanel = null;
    private Engine engineX = null;
    private Engine engineY = null;
    private JLabel teamXLabel = null;
    private JLabel teamYLabel = null;
    private ViewThread eastPanelThread = null;
    private ViewThread westPanelThread = null;
    private ViewThread fieldPanelThread = null;
    
    public SimulationView(Engine engineX,Engine engineY){
        this.engineX = engineX;
        this.engineY = engineY;        
        initSimulationViewComponents();
        addSimulationViewComponents();
        startViewThreads();
    }

    private void initSimulationViewComponents() {
        setLayout(new java.awt.BorderLayout());
        setPreferredSize(new java.awt.Dimension(KabaddiConstants.SIMULATION_PANEL_WIDTH, KabaddiConstants.SIMULATION_PANEL_HEIGHT));
        setBackground(KabaddiConstants.BACKGROUND_COLOR);
        setBorder(PANEL_BORDER);
        
        teamXLabel = new JLabel("Team "+this.engineX.getTeamName(),JLabel.CENTER);
        teamXLabel.setForeground(Color.WHITE);
        //teamXLabel.setForeground(Color.BLACK);
        teamYLabel = new JLabel("Team "+this.engineY.getTeamName(),JLabel.CENTER);
        teamYLabel.setForeground(Color.WHITE);
        //teamYLabel.setForeground(Color.BLACK);
        northPanel = new JPanel(new java.awt.GridLayout());        
        northPanel.setPreferredSize(new java.awt.Dimension(KabaddiConstants.SIMULATION_PANEL_WIDTH, KabaddiConstants.SIMULATION_Y_OFFSET));        
        northPanel.setBackground(KabaddiConstants.BACKGROUND_COLOR);
        southPanel = new JPanel();
        southPanel.setPreferredSize(new java.awt.Dimension(KabaddiConstants.SIMULATION_PANEL_WIDTH, KabaddiConstants.SIMULATION_Y_OFFSET));
        southPanel.setBackground(KabaddiConstants.BACKGROUND_COLOR);
        eastPanel = new EastDeskView(engineY);
        eastPanel.setPreferredSize(new java.awt.Dimension(KabaddiConstants.SIMULATION_X_OFFSET, KabaddiConstants.FIELD_HEIGHT));
        eastPanel.setBackground(KabaddiConstants.BACKGROUND_COLOR);
        westPanel = new WestDeskView(engineX);
        westPanel.setPreferredSize(new java.awt.Dimension(KabaddiConstants.SIMULATION_X_OFFSET, KabaddiConstants.FIELD_HEIGHT));
        westPanel.setBackground(KabaddiConstants.BACKGROUND_COLOR);
        fieldPanel = new FieldView(this.engineX,this.engineY);
        
        eastPanelThread = new ViewThread(eastPanel);
        westPanelThread = new ViewThread(westPanel);
        fieldPanelThread = new ViewThread(fieldPanel);
    }
    
    private void addSimulationViewComponents() {
        northPanel.add(teamXLabel);
        northPanel.add(teamYLabel);
        add(northPanel,java.awt.BorderLayout.NORTH);         
        add(southPanel,java.awt.BorderLayout.SOUTH);
        add(eastPanel,java.awt.BorderLayout.EAST);
        add(westPanel,java.awt.BorderLayout.WEST);
        add(fieldPanel,java.awt.BorderLayout.CENTER);
    }

    private void startViewThreads() {
        eastPanelThread.start();
        westPanelThread.start();
        fieldPanelThread.start();
    }
}
