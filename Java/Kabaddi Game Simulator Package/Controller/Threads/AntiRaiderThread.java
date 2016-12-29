package Kabaddi.Controller.Threads;

import Kabaddi.Agent.Agent;
import Kabaddi.Comm.SerialCommands;
import Kabaddi.Comm.SerialIO;
import Kabaddi.Controller.Controller;
import Kabaddi.Engine.Engine;
import Kabaddi.Simulator.KabaddiConstants;
import Kabaddi.Simulator.KabaddiSimulator;
import java.awt.geom.Point2D;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author Nagarjuna
 */

public class AntiRaiderThread extends Thread {

    private Agent antiRaider = null;
    private int antiRaiderRIndex;
    private int antiRaiderVIndex;
    private Agent raider = null;     
    private Engine antiRaidersEngine = null;
    private Engine raidersEngine = null;
    private boolean stopBit;
    private SerialIO sio = null;
    private String id = null;
    
    public AntiRaiderThread(Agent antiRaider,int antiRaiderRIndex,int antiRaiderVIndex,Engine antiRaidersEngine,Agent raider,Engine raidersEngine,SerialIO sio){
        this.antiRaider = antiRaider;
        this.antiRaiderRIndex = antiRaiderRIndex;
        this.antiRaiderVIndex = antiRaiderVIndex;
        this.antiRaidersEngine = antiRaidersEngine;
        this.raidersEngine = raidersEngine;
        this.raider = raider;        
        this.stopBit = false;
        this.sio = sio;
        initializeAntiRaider();        
    }
    
    
    private void initializeAntiRaider() {
        antiRaider.setActive(true);
        antiRaider.setIsRaider(false);        
        antiRaider.setCaptured(false);
        antiRaider.setIsSucessfullRaid(false);
        antiRaider.setTouch(false);        
        antiRaider.setAgentLocation(antiRaidersEngine.antiRaiderInitialPosition(antiRaiderVIndex, antiRaidersEngine.getNoOfInGameAgents(),raidersEngine.getNoOfInGameAgents()));
        antiRaider.setAgentId(Integer.toString(this.antiRaiderRIndex));
        if(KabaddiSimulator.HARDWARE_MODE) id = antiRaidersEngine.agentID(antiRaiderRIndex);
    }
    
    public void terminate(){
        antiRaider.setActive(false);
        this.stopBit = true;
    }
    
    @Override
    @SuppressWarnings("SleepWhileInLoop")
    public void run() {
        
        /* Get the anti raider agents */
        Agent[] antiRaiderAgents = antiRaidersEngine.getAgents();
        //System.out.println("Thread Created !!");
        while(!stopBit){
            /* If the anti raider is not out of the game and raider is not yet captured */
            if(!antiRaider.isOutOfGame() && !raider.isCaptured()){
                /* Get the positions of other anti Raiders*/
                /* Find number of non out of game players*/
                int noOfInGameAgents = antiRaidersEngine.getNoOfInGameAgents();
                /* 
                 * Initialize antiRaiderPositions array
                 * noOfInGameAgents-1 is used as we exclude the present agent position in the array
                 */
                Point2D.Float[] antiRaiderPositions = new Point2D.Float[noOfInGameAgents-1];
                /* populate antiRaiderPositions with data*/
                /* get no of agents in team */
                int noOfAgents = antiRaidersEngine.getNoOfAgents();
                /* initialize antiRaiderPositions index */
                int index = 0;
                /* loop through antiRaiderAgents to find the rest of the agents */
                for(int i = 0; i <  noOfAgents; i++){
                    if(i != antiRaiderRIndex && !antiRaiderAgents[i].isOutOfGame()){
                        /* Here index value should be less the noOfInGameAgents-1 */
                        antiRaiderPositions[index] = (Point2D.Float)antiRaiderAgents[i].getAgentLocation().clone();
                        index++;
                    }
                }
                /* Get antiRaiderPostion and Raider Position */
                Point2D.Float antiRaiderPosition = antiRaider.getAgentLocation();                
                Point2D.Float raiderPosition = (Point2D.Float)raider.getAgentLocation().clone();//new Point2D.Float(raider.getIntX(),raider.getIntY());
                /* Get raider Team score */
                int raiderScore = raidersEngine.getTeamScore();
                /* move the Anti Raider Agent */
                /* get the relative motion command */
                Point2D.Float relPosition = antiRaidersEngine.moveAntiRaiderAgent(antiRaider, antiRaiderPositions,raiderPosition,raiderScore);
                /*decrement raider steps */
                if(antiRaider.hasSteps()) antiRaider.decrementSteps();
                else relPosition.setLocation(0,0);
                /* send motion command to the robot */
                if(KabaddiSimulator.HARDWARE_MODE) {
                    sio.sendString(id+SerialCommands.getCommandRelPostion(relPosition));
                    try {
                        Thread.sleep(KabaddiConstants.MESSAGE_DELAY);
                    } catch (InterruptedException ex) {
                        Logger.getLogger(AntiRaiderThread.class.getName()).log(Level.SEVERE, null, ex);
                    }
                }
                antiRaider.setAgentLocationRel(relPosition); 
                /* Check if raider and anti raider touch each other */
                if(!antiRaider.isTouched()){
                   if(Controller.checkTouch(antiRaiderPosition, raiderPosition)){
                       /* Set Touched as true*/
                       antiRaider.setTouch(true);
                       raider.setTouch(true);
                   } 
                }
                /* Check if anti raider crossed the border lines */
                if( Controller.checkBorderCross(antiRaiderPosition)){
                    /* Set out of game as true*/
                    antiRaider.setIsOutOfGame(true);
                    /* increment number of out of game agents by 1*/
                    antiRaidersEngine.incrementOutOfGameAgents();
                }
                /* initialize the antiRaiderPositions variable to null (memory management)*/
                antiRaiderPositions = null;
                /*Sleep for some time */
                try {
                    
                    if(antiRaider.isPoweredUp()){
                        Thread.sleep(KabaddiConstants.SLEEP_TIME/2);
                    }
                    else Thread.sleep(KabaddiConstants.SLEEP_TIME);
                } catch (InterruptedException ex) {
                    Logger.getLogger(AntiRaiderThread.class.getName()).log(Level.SEVERE, null, ex);
                }                
            }
            else break;
        }
        //System.out.println("Thread Terminated !!");
    }    
}