package Kabaddi.Controller;

import Kabaddi.Engine.Engine;
import Kabaddi.Simulator.KabaddiConstants;
import java.awt.geom.Point2D;

/**
 *
 * @author Nagarjuna
 */
public class Controller {
    
    private static final int ZERO = 0;    
    
    public synchronized static boolean checkTouch(Point2D.Float antiRaiderPosition,Point2D.Float raiderPosition){        
        return (antiRaiderPosition.distance(raiderPosition) <= KabaddiConstants.AGENT_DIAMETER/2);
    }
    
    public synchronized static boolean checkBorderCross(Point2D.Float agentPosition){
        int xPosition = (int)agentPosition.getX();
        int yPosition = (int)agentPosition.getY();
        return ((xPosition > KabaddiConstants.FIELD_WIDTH/2) || (xPosition < ZERO) || (yPosition > KabaddiConstants.FIELD_HEIGHT) || (yPosition < ZERO));
    }
    
    public synchronized static boolean checkCapture(Point2D.Float raiderPosition,Point2D.Float[] antiRaiderPositions){
        int noOfAntiRaiders = antiRaiderPositions.length;
        int touchNumber = 0;        
        for( int i = 0 ; i < noOfAntiRaiders ; i++){            
            if(Controller.checkTouch(antiRaiderPositions[i],raiderPosition)) {
                touchNumber++;
                if(touchNumber == KabaddiConstants.CAPTURE_NUMBER) return true;                
            }
        }        
        return false;
    }
    
    public synchronized static boolean checkSucessfullRaid(Point2D.Float raiderPosition){
        return (raiderPosition.getX() > KabaddiConstants.FIELD_WIDTH/2);
    }
    
    public static boolean isThereAWinner(Engine antiRaidersEngine,Engine raidersEngine){
        
        boolean GAME_END = false;
        /* Get number of ingame agents of antiraider team and raider team */
        int noAntiRaiderAgents = antiRaidersEngine.getNoOfInGameAgents();
        int noRaiderAgents = raidersEngine.getNoOfInGameAgents();
        
        /* set the win status accordingly */
        if( noAntiRaiderAgents == 0 && noRaiderAgents == 0){
            antiRaidersEngine.setWinStatus(true);
            raidersEngine.setWinStatus(true);
            GAME_END = true;            
        }
        else if( noAntiRaiderAgents > 0 && noRaiderAgents == 0){
            antiRaidersEngine.setWinStatus(true);
            GAME_END = true;            
        }
        else if( noAntiRaiderAgents < 2 && noRaiderAgents > 0 ){
            raidersEngine.setWinStatus(true);
            GAME_END = true;            
        }
        
        return GAME_END;
    }
    
}
