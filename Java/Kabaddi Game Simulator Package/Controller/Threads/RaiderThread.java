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
public class RaiderThread extends Thread{
    
    private Agent raider = null;
    private Engine raidersEngine = null;
    private Engine antiRaidersEngine = null;
    private int raiderRIndex;
    private boolean stopBit;
    private String id = null;
    private SerialIO sio = null;
    
    public RaiderThread(Agent raider,int raiderRIndex,Engine raidersEngine,Engine antiRaidersEngine,SerialIO sio){
        this.raider = raider;
        this.raidersEngine = raidersEngine;
        this.antiRaidersEngine = antiRaidersEngine;
        this.raiderRIndex = raiderRIndex;
        this.stopBit = false;
        this.sio = sio;
        initializeRaider();
    }
    
    private void initializeRaider() {
        raider.setActive(true);
        raider.setIsRaider(true);
        raider.setCaptured(false);
        raider.setIsSucessfullRaid(false);
        raider.setTouch(false);
        raider.setAgentLocation(raidersEngine.RaiderInitialPosition(raidersEngine.getNoOfInGameAgents(), antiRaidersEngine.getNoOfInGameAgents()));
        if(KabaddiSimulator.HARDWARE_MODE) id = raidersEngine.agentID(raiderRIndex);
    }
    
    public void terminate(){
        raider.setActive(false);
        this.stopBit = true;
    }
    
    @Override
    @SuppressWarnings("SleepWhileInLoop")
    public void run(){
        
        Agent[] antiRaiderAgents = antiRaidersEngine.getAgents();
        //System.out.println("Thread Created !!");
        while(!stopBit){
            /* If the raider is not yet captured and did not make a sucessfull raid */
            if(!raider.isCaptured() && !raider.isSucessfullRaid()){
                /* Get the positions of anti raiders in the game */
                /* Get number of Ingame anti raiders */
                int noOfInGameAgents = antiRaidersEngine.getNoOfInGameAgents();
                /* Initialize antiRaiderPositions array to noOfInGameAgents */
                Point2D.Float[] antiRaiderPositions = new Point2D.Float[noOfInGameAgents];
                /* populate antiRaiderPositions with data*/
                /* get no of agents in team */
                int noOfAgents = antiRaidersEngine.getNoOfAgents();
                /* initialize antiRaiderPositions index */
                int index = 0;
                /* loop through antiRaiderAgents to find anti raider agent postions*/
                for(int i = 0; i <  noOfAgents; i++){
                    if(!antiRaiderAgents[i].isOutOfGame()){
                        /* Here index value should be less the noOfInGameAgents */
                        antiRaiderPositions[index] = (Point2D.Float)antiRaiderAgents[i].getAgentLocation().clone();
                        index++;
                    }
                }
                /* Get antiRaidersScore */
                int antiRaiderScore = antiRaidersEngine.getTeamScore();
                /* Move Raider Agent */
                /* get relative motion command */                
                Point2D.Float relPosition = raidersEngine.moveRaiderAgent(raider, antiRaiderPositions,antiRaiderScore);
                /*decrement raider steps */
                if(raider.hasSteps()) raider.decrementSteps();
                else relPosition.setLocation(0,0);
                
                /* Send action command To the robot */
                if(KabaddiSimulator.HARDWARE_MODE) {
                    sio.sendString(id+SerialCommands.getCommandRelPostion(relPosition));
                    try {
                        Thread.sleep(KabaddiConstants.MESSAGE_DELAY);
                    } catch (InterruptedException ex) {
                        Logger.getLogger(RaiderThread.class.getName()).log(Level.SEVERE, null, ex);
                    }
                }
                raider.setAgentLocationRel(relPosition);
                /* Check if Raider is captured */
                if(!raider.isCaptured()){
                    if(Controller.checkCapture(raider.getAgentLocation(), antiRaiderPositions)){
                        /* set captured as true */
                        raider.setCaptured(true);
                        /* Set is out of game as true */
                        raider.setIsOutOfGame(true);
                        /* increment the out of game agents */
                        raidersEngine.incrementOutOfGameAgents();                        
                    }
                    /* check for sucessfull raid*/
                    else if(!raider.isSucessfullRaid()){                        
                        if(Controller.checkSucessfullRaid(raider.getAgentLocation())){
                            /* set sucessfullRaid to true */
                            raider.setIsSucessfullRaid(true);                            
                        }
                        /* check for border cross */
                        else if(Controller.checkBorderCross(raider.getAgentLocation())){
                            /* Set is out of game as true */
                            raider.setIsOutOfGame(true);
                            /* increment the out of game agents */
                            raidersEngine.incrementOutOfGameAgents();                           
                        }
                    }
                    
                }
                /* set anti raiders positions to null ( memory management )*/
                antiRaiderPositions = null;
                /*Sleep for some time */
                try {
                    if(raider.isPoweredUp()){
                        Thread.sleep(KabaddiConstants.SLEEP_TIME/2);
                    }
                    else Thread.sleep(KabaddiConstants.SLEEP_TIME);
                } catch (InterruptedException ex) {
                    Logger.getLogger(RaiderThread.class.getName()).log(Level.SEVERE, null, ex);
                }
            }
            else break;
        }
        //System.out.println("Thread terminated !!");
    }

    
}
