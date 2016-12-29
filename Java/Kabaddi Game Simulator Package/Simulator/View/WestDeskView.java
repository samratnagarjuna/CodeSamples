package Kabaddi.Simulator.View;

import Kabaddi.Agent.Agent;
import Kabaddi.Engine.Engine;
import Kabaddi.Simulator.KabaddiConstants;
import java.awt.Color;
import java.awt.Graphics;
import java.awt.Graphics2D;

/**
 *
 * @author Nagarjuna
 */
public class WestDeskView extends javax.swing.JPanel{
    
    private Engine engine = null;
    private int height;
    private int width;
    private int noOfPlayers;
    private int xPosition;
    private int yPosition;
    
    public WestDeskView(Engine engine){
        this.engine = engine;
        this.height = KabaddiConstants.FIELD_HEIGHT;
        this.width = KabaddiConstants.SIMULATION_X_OFFSET;
        this.noOfPlayers = engine.getNoOfAgents();
        xPosition = width/8;
        yPosition = (height - KabaddiConstants.DESK_GAP*noOfPlayers + KabaddiConstants.DESK_GAP)/2;                
    }
    
    @Override
    public void paintComponent(Graphics g) {
        super.paintComponent(g);
        Graphics2D g2D = (Graphics2D) g;
        DrawDesk(g2D);
        DrawPlayers(g2D);        
    }
    
    private void DrawDesk(Graphics2D g2D) {
        g2D.setColor(Color.WHITE);        
        g2D.drawRect(0, (height - KabaddiConstants.DESK_GAP*noOfPlayers)/2 , width/4, (KabaddiConstants.DESK_GAP*noOfPlayers));
    }
    
    private void DrawPlayers(Graphics2D g2D) {
        g2D.setColor(KabaddiConstants.TEAM_X_COLOR);
        Agent[] agents = engine.getAgents();         
        for(int i = 0 ; i < noOfPlayers ; i++){            
            if(!agents[i].isAgentActive() || agents[i].isOutOfGame()){
                //has to be changed with respect to the diameter .. the width and height of the desk also to be adjusted
                //g2D.fillOval(xPosition - KabaddiConstants.AGENT_DIAMETER/2 , (yPosition + KabaddiConstants.DESK_GAP*i) - KabaddiConstants.AGENT_DIAMETER/2, KabaddiConstants.AGENT_DIAMETER, KabaddiConstants.AGENT_DIAMETER);
                g2D.fillOval(xPosition - 5 , (yPosition + KabaddiConstants.DESK_GAP*i) - 5, 10, 10);
            }            
        }
    }
}
