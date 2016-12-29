package Kabaddi.Engine;

import Kabaddi.Agent.Agent;

/**
 *
 * @author Nagarjuna
 */
public abstract class Engine implements Strategy{
    
    private int noOfAgents;
    protected Agent[] agents;
    private int outOfGameAgents;
    private int teamScore;
    private boolean WIN_STATUS;
    private String teamName;
    private String strategyName;
    private boolean newGame;
    
    public Engine(int noOfAgents,String teamName){
        this.noOfAgents = noOfAgents;
        this.outOfGameAgents = 0;
        this.WIN_STATUS = false;
        this.teamName = teamName;
        this.teamScore = 0;
        strategyName = null;
        newGame = true;
        createAgents();
    }

    private void createAgents() {
        this.agents = new Agent[this.noOfAgents];
        for(int index = 0 ; index < this.noOfAgents ; index++) agents[index] = new Agent();
    }
    
    public boolean isNewGame() {
        if(this.newGame) {
            setNewGame(false);
            return true;
        }
        else return false;
    }
    
    public void setNewGame(boolean newGame) {
        this.newGame = newGame;
    }
    
    public void setNoOfAgents(int noOfAgents){
        this.noOfAgents = noOfAgents;
    }
    
    public Agent[] getAgents(){
        return this.agents;
    }
    
    public Agent getAgent(int agentIndex){
        return this.agents[agentIndex];
    }
    
    public synchronized void setOutOfGameAgents(int outOfGameAgents){
        this.outOfGameAgents = outOfGameAgents;
    }

    public synchronized void incrementOutOfGameAgents(){
        this.outOfGameAgents++;
    }
    
    public synchronized void incrementOutOfGameAgents(int value){
        this.outOfGameAgents = this.outOfGameAgents + value;
    }

    public synchronized void decrementOutOfGameAgents(){
       this.outOfGameAgents--; 
    }
    
    public synchronized void decrementOutOfGameAgents(int value){
        this.outOfGameAgents = this.outOfGameAgents - value;
    }

    public int getNoOfOutOfGameAgents(){
        return this.outOfGameAgents;
    }
    
    public int getNoOfInGameAgents(){
        return this.noOfAgents-this.outOfGameAgents;
    }

    public void incrementScore(){
        this.teamScore++;
    }
    
    public void incrementScore(int value){
        this.teamScore = this.teamScore + value;
    }

    public void decrementScore(int value){
        this.teamScore = this.teamScore - value;
    }

    public int getTeamScore(){
        return this.teamScore;
    }

    public void resetTeamScore(){
        this.teamScore = 0;
    }

    public void setTeamScore(int teamScore){
        this.teamScore = teamScore;
    }

    public int getNoOfAgents(){
        return this.noOfAgents;
    }
    
    public boolean getWinStatus(){
        return WIN_STATUS;
    }
    
    public void setWinStatus(boolean WIN_STATUS){
        this.WIN_STATUS = WIN_STATUS;
    }
    
    public String getTeamName(){
        return this.teamName;
    }
    
    public void setStrategyName(String strategyName){
        this.strategyName = strategyName;
    }
    
    public String getStrategyName(){
        return this.strategyName;
    }
    
}
