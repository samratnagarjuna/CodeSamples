package Kabaddi.Controller;

import Kabaddi.Agent.Agent;
import Kabaddi.Controller.Threads.AntiRaiderThread;
import Kabaddi.Controller.Threads.RaiderThread;
import Kabaddi.Engine.Engine;
import Kabaddi.Simulator.KabaddiSimulator;
import Kabaddi.Comm.SerialIO;
import java.io.BufferedWriter;
import java.io.FileWriter;

/**
 *
 * @author Nagarjuna
 */
public class Cordinator extends Thread{
    
    private Engine engineX = null;    
    private Engine engineY = null;    
    public static volatile boolean PLAY;
    private static volatile boolean stopBit;
    //private Timer timer = null;
    //private boolean running;
    private SerialIO sio = null;
    
    /*ActionListener timerTimeOut = new ActionListener(){
        @Override
        public void actionPerformed(ActionEvent e){
            running = false;
        }
    };*/
    
    public Cordinator(Engine engineX,Engine engineY){
        this.engineX = engineX;
        this.engineY = engineY;        
        PLAY = false;
        stopBit = false;
        //running = false;
        //initializeTimer();
        initializeHardwareMode();
    }
    
    @Override
    @SuppressWarnings("empty-statement")
    public void run(){
        int teamXScoreIni = 0;
        int teamXScoreFin = 0;
        int teamYScoreIni = 0;
        int teamYScoreFin = 0;
        String data = null;
        /* Loop through thread stop bit */
        while(!stopBit){
            /* Wait for PLAY to be clicked */
            
            while(!PLAY);
            
            teamXScoreIni = engineX.getTeamScore();
            teamYScoreIni = engineY.getTeamScore();
            engineX.setNewGame(true);
            engineY.setNewGame(true);
            if(gamePlay(engineX,engineY)) {
                this.terminate();
                break;
            }
            
            teamXScoreFin = engineX.getTeamScore();
            teamYScoreFin = engineY.getTeamScore();
            engineX.setNewGame(true);
            engineY.setNewGame(true);
            if(KabaddiSimulator.LOGGER_MODE){
                data = null;
                data = "("+teamXScoreIni+","+teamYScoreIni+")";
                data += "["+engineX.getStrategyName()+","+engineY.getStrategyName()+"]";
                data += "("+teamXScoreFin+","+teamYScoreFin+")";;

                logResults(data);
            }
            
            teamXScoreIni = engineX.getTeamScore();
            teamYScoreIni = engineY.getTeamScore();
            while(!PLAY);
            if(gamePlay(engineY,engineX)) {
                this.terminate();
                break;
            }
            teamXScoreFin = engineX.getTeamScore();
            teamYScoreFin = engineY.getTeamScore();
            
            if(KabaddiSimulator.LOGGER_MODE){
                data = null;
                data = "("+teamXScoreIni+","+teamYScoreIni+")";
                data += "["+engineX.getStrategyName()+","+engineY.getStrategyName()+"]";
                data += "("+teamXScoreFin+","+teamYScoreFin+")";;

                logResults(data);
            }
        }
        
    }
    
    @SuppressWarnings("empty-statement")
    private boolean gamePlay(Engine antiRaidersEngine,Engine raidersEngine){
        /* Check the time and display the result */        
        if(Controller.isThereAWinner(antiRaidersEngine, raidersEngine)) return true;
        else {
            
            int i;
            /* Get the raider and create Raiders thread*/
            RaiderThread raiderThread = null;
            int raiderIndex = findRaiderIndex(raidersEngine);            
            Agent raider = raidersEngine.getAgent(raiderIndex);
            
            /* Get anti raiders and create antiraiders threads */
            int noAntiRaiders = antiRaidersEngine.getNoOfInGameAgents();
            AntiRaiderThread[] antiRaiderThreads = new AntiRaiderThread[noAntiRaiders];
            
            Agent[] antiRaiders = antiRaidersEngine.getAgents();
            /* Loop through all the agents */
            int index = 0;
            for(i = 0 ; i < antiRaidersEngine.getNoOfAgents() ; i++){                
                if(!antiRaiders[i].isOutOfGame()){
                    antiRaiderThreads[index] = new AntiRaiderThread(antiRaiders[i], i, index, antiRaidersEngine, raider, raidersEngine,sio);
                    /* Here the index is always less then the  noofingame agents of the antiRaidersTeam */
                    index++;
                }
            }  
            raiderThread = new RaiderThread(raider,raiderIndex,raidersEngine, antiRaidersEngine,sio);
            /* Start the threads */
            
            for(i = 0 ; i < antiRaiderThreads.length ; i++){
                antiRaiderThreads[i].start();
            }
            raiderThread.start();
            //startTimer();            
            
            /* wait for raider status to update or time to finish*/
            while(!raider.isCaptured() && !raider.isSucessfullRaid() && !raider.isOutOfGame() && !stopBit );//&& running);
            /* check if there is a terminate command */
            if(stopBit) return false;
            /* stop the threads */
            
            for(i = 0 ; i < antiRaiderThreads.length ; i++){
                if(antiRaiderThreads[i].isAlive()) antiRaiderThreads[i].terminate();
            }
            if(raiderThread.isAlive()) raiderThread.terminate();
            
            /* If raider is captured then increment the score of the antiRaidersTeam */
            if(raider.isCaptured()){
                antiRaidersEngine.incrementScore();                
            }
            /* If the raider makes a sucessfull raid */
            else if(raider.isSucessfullRaid()){
                int touchNumber = 0;
                /* Find the number of players that are touched and ingame as out of game */
                for(i = 0 ; i < antiRaidersEngine.getNoOfAgents() ; i++){
                    if(!antiRaiders[i].isOutOfGame() && antiRaiders[i].isTouched()){
                        antiRaiders[i].setIsOutOfGame(true);
                        antiRaidersEngine.incrementOutOfGameAgents();
                        raidersEngine.incrementScore();
                        touchNumber++;
                    }
                }               
                /* Make the out of game agents as ingame */
                Agent[] raiderAgents = raidersEngine.getAgents();
                for(i = 0 ; i < raiderAgents.length ; i++){
                    if(raiderAgents[i].isOutOfGame()){
                        raiderAgents[i].setIsOutOfGame(false);
                        raidersEngine.decrementOutOfGameAgents();
                        touchNumber--;                        
                    }
                    if(touchNumber == 0) break;
                }
            }
            else if(raider.isOutOfGame()) {
                /* border cross */
                antiRaidersEngine.incrementScore();
            }
            /*else if(!running){
                //Timed out
                raider.setIsOutOfGame(true);
                raidersEngine.incrementOutOfGameAgents();
                antiRaidersEngine.incrementScore();
            }*/
            
            
            raiderThread = null;
            antiRaiderThreads = null;
            this.PLAY = false;
            //stopTimer();
            return Controller.isThereAWinner(raidersEngine,antiRaidersEngine);
        }
        
    }    
    
    private int findRaiderIndex(Engine raidersEngine){
        Agent[] raiderAgents = raidersEngine.getAgents();
        int raiderIndex = 0;
        /* Pick the first non out of game raider */
        for(int i = 0 ; i < raiderAgents.length ; i++ ){
            if(!raiderAgents[i].isOutOfGame()){
                raiderIndex = i;
                break;
            }
        }        
        return raiderIndex;
    }
    
    public void terminate(){        
        this.stopBit = true;
    }
    
    

    /*private void startTimer() {
        running = true;        
        timer.restart();
    }

    private void stopTimer() {
        if(timer.isRunning()) timer.stop();
    }

    private void initializeTimer() {
        timer = new Timer(KabaddiConstants.GAME_TIME,timerTimeOut);
    }*/

    private void initializeHardwareMode() {
        if(KabaddiSimulator.HARDWARE_MODE){
            sio = new SerialIO(KabaddiSimulator.commPortID);
        }
    }
    
    private void logResults(String data){
        try{
                // Create file 
                FileWriter fstream = new FileWriter("logger.txt",true);          
                BufferedWriter out = new BufferedWriter(fstream);
                out.write(data+"\n");
                //Close the output stream
                out.close();
        }catch (Exception e){//Catch exception if any
                System.err.println("Error: " + e.getMessage());
        }
    }
    
}
