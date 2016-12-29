package Kabaddi.Simulator.View;

import Kabaddi.Engine.Engine;
import Kabaddi.Simulator.KabaddiConstants;

/**
 *
 * @author Nagarjuna
 */
public class SimulatorView extends javax.swing.JPanel{
    
    private SimulationView simulationView = null;
    private DataView dataView = null;
    private Engine engineX = null;
    private Engine engineY = null;
    
    public SimulatorView(Engine engineX,Engine engineY){
        this.engineX = engineX;
        this.engineY = engineY;
        initSimulatorViewComponents();
        addSimulatorComponents();
    }

    private void initSimulatorViewComponents() {
        setLayout(new java.awt.BorderLayout());
        setPreferredSize(new java.awt.Dimension(KabaddiConstants.SIMULATOR_PANEL_WIDTH, KabaddiConstants.SIMULATOR_PANEL_HEIGHT));
        simulationView = new SimulationView(this.engineX,this.engineY);
        dataView = new DataView(this.engineX,this.engineY);
    }
    
    private void addSimulatorComponents(){
        add(simulationView,java.awt.BorderLayout.CENTER);
        add(dataView, java.awt.BorderLayout.EAST);
    }
}
