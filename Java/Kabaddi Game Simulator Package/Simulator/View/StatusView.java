package Kabaddi.Simulator.View;

import Kabaddi.Engine.Engine;
import java.awt.Font;
import java.awt.Graphics;
import java.awt.Graphics2D;

/**
 *
 * @author Nagarjuna
 */
public class StatusView extends javax.swing.JPanel{
    
    private Engine engineX = null;
    private Engine engineY = null;
    private Font FONT = new Font(Font.SANS_SERIF, Font.PLAIN, 12);
    
    StatusView(Engine engineX,Engine engineY){
        this.engineX = engineX;
        this.engineY = engineY;
        
    }
    
    @Override
    public void paintComponent(Graphics g) {
        super.paintComponent(g);
        Graphics2D g2D = (Graphics2D) g;
        setStatus(g2D); 
    }

    private void setStatus(Graphics2D g2D) {
        g2D.setFont(FONT);
        g2D.drawString("Team "+engineX.getTeamName()+" Score: "+engineX.getTeamScore(),10,50);
        g2D.drawString("Active Team "+engineX.getTeamName()+" Agents: "+engineX.getNoOfInGameAgents(),10,70);
        g2D.drawString("Team "+engineY.getTeamName()+" Score: "+engineY.getTeamScore(),10,90);
        g2D.drawString("Active Team "+engineY.getTeamName()+" Agents: "+engineY.getNoOfInGameAgents(),10,110);
        
        boolean engineXWin = engineX.getWinStatus();
        boolean engineYWin = engineY.getWinStatus();
        
        if(engineXWin && engineYWin) g2D.drawString("Match Draw",10,130);
        else if( engineXWin ) g2D.drawString("Team "+engineX.getTeamName()+" Wins",10,130);
        else if( engineYWin ) g2D.drawString("Team "+engineY.getTeamName()+" Wins",10,130);        
    }
    
    
    
    
}
