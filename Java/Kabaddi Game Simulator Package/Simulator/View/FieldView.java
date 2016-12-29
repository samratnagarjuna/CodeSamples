package Kabaddi.Simulator.View;

import Kabaddi.Agent.Agent;
import Kabaddi.Engine.Engine;
import Kabaddi.Simulator.KabaddiConstants;
import java.awt.BasicStroke;
import java.awt.Graphics;
import java.awt.Graphics2D;

/**
 *
 * @author Nagarjuna
 */
public class FieldView extends javax.swing.JPanel{
    
    private final javax.swing.border.Border PANEL_BORDER = javax.swing.BorderFactory.createEtchedBorder(javax.swing.border.EtchedBorder.LOWERED);
    private Engine engineX = null;
    private Engine engineY = null;    
    
    public FieldView(Engine engineX,Engine engineY){
        this.engineX = engineX;
        this.engineY = engineY;
        initFieldViewComponents();
    }

    private void initFieldViewComponents() {
        setLayout(null);
        setPreferredSize(new java.awt.Dimension(KabaddiConstants.FIELD_WIDTH,KabaddiConstants.FIELD_HEIGHT));
        setBorder(PANEL_BORDER);        
        setBackground(KabaddiConstants.BACKGROUND_COLOR);
    }
    
    @Override
    public void paintComponent(Graphics g) {
        super.paintComponent(g);
        Graphics2D g2D = (Graphics2D) g;
        drawKabaddiField(g2D);
        drawKabaddiAgents(g2D,this.engineX,this.engineY);        
    }
    
    private void drawKabaddiField(Graphics2D g2D) {
        
        g2D.setColor(KabaddiConstants.FIELD_COLOR);
        g2D.setStroke(new BasicStroke(1.0f));        
        
        /*Separator Line*/
        g2D.drawLine(KabaddiConstants.FIELD_WIDTH/2,0,KabaddiConstants.FIELD_WIDTH/2,KabaddiConstants.FIELD_HEIGHT);
        /*Game Point Lines*/
        g2D.drawLine(KabaddiConstants.FIELD_WIDTH/8,0, KabaddiConstants.FIELD_WIDTH/8,KabaddiConstants.FIELD_HEIGHT);
        g2D.drawLine(KabaddiConstants.FIELD_WIDTH*7/8,0, KabaddiConstants.FIELD_WIDTH*7/8,KabaddiConstants.FIELD_HEIGHT);
    }
    
    private void drawKabaddiAgents(Graphics2D g2D,Engine engineX,Engine engineY){
        drawTeamAgents(g2D,engineX,0);
        drawTeamAgents(g2D,engineY,1);        
    }
    
    private void drawTeamAgents(Graphics2D g2D,Engine engine,int mirror){
        Agent[] agents = engine.getAgents();
        int xPosition;
        int yPosition;
        
        for(int i = 0 ; i < engine.getNoOfAgents() ; i++){            
            
            if(agents[i].isAgentActive() && !agents[i].isOutOfGame()){
                /* Set base Color here based on mirror value 
                 If mirror is o then set to teamX color if 1 them teamY color
                 */ 
                
                if(mirror == 0) g2D.setColor(KabaddiConstants.TEAM_X_COLOR);
                else g2D.setColor(KabaddiConstants.TEAM_Y_COLOR);
                if(!agents[i].isRadier()){
                    if(agents[i].isTouched()) g2D.setColor(KabaddiConstants.TOUCH_COLOR);                    
                    xPosition = mirror*KabaddiConstants.FIELD_WIDTH + (int)Math.pow(-1, mirror)*agents[i].getIntX();              
                }
                else {
                    xPosition = (1-mirror)*KabaddiConstants.FIELD_WIDTH + (int)Math.pow(-1,1-mirror)*agents[i].getIntX();                    
                }
                yPosition = agents[i].getIntY();                
                g2D.fillOval(xPosition -KabaddiConstants.AGENT_DIAMETER/2 , yPosition - KabaddiConstants.AGENT_DIAMETER/2, KabaddiConstants.AGENT_DIAMETER, KabaddiConstants.AGENT_DIAMETER);                
            }
        }
    }
    
}
