package Kabaddi.Agent;

import java.awt.geom.Point2D;
import java.awt.geom.Point2D.Float;

/**
 *
 * @author Nagarjuna
 */
public class Agent {
    
    private Point2D.Float agentInitialLocation;
    private Point2D.Float agentLocation;
    private Point2D.Float raiderInitialLocation;
    private boolean isOutOfGame;
    private boolean isActive;
    private boolean isRaider;
    private boolean isTouched;
    private boolean isCaptured;
    private boolean isSucessfullRaid;
    private int steps;
    private boolean powerUp;
    private String id;
    
    
    
    public Agent(){
        agentInitialLocation = new Point2D.Float();
        agentLocation = new Point2D.Float();
        raiderInitialLocation = new Point2D.Float();
        initVariables();
    }
    
    private void initVariables() {
        isActive = false;
        isOutOfGame = false;        
        isTouched = false;
        isCaptured = false;
        isSucessfullRaid = false;
        powerUp = false;
        steps = 1000;
    }
    
    public void setPowerUp(){
        powerUp = true;
    }
    
    public void resetPowerUp(){
        powerUp = false;
    }
    public boolean isPoweredUp(){
        return powerUp;
    }
    
    public void setAgentId(String id){
        this.id = id;
    }

    public String getAgentId(){
        return id;
    }
    
    public void setSteps(int steps){
        this.steps = steps;
    }
    
    public void decrementSteps(){
        steps--;
    }
    
    public int getSteps(){
        return steps;
    }
    
    public boolean hasSteps(){
        return (steps != 0);
    }

    public void setAgentLocation(int xCordinate, int yCordinate) {
        agentLocation.setLocation(xCordinate, yCordinate);
    }
    
    public void setAgentLocation(Point2D.Float location){        
        agentLocation.setLocation(location);
    }

    public void intializeAgent(int xCordinate, int yCordinate, String id) {
        agentInitialLocation.setLocation(xCordinate, yCordinate);
        agentLocation.setLocation(agentInitialLocation.x, agentInitialLocation.y);
        setAgentId(id);
    }


    public void intializeAgentPosition(int xCordinate, int yCordinate) {
        agentInitialLocation.setLocation(xCordinate, yCordinate);
        agentLocation.setLocation(agentInitialLocation.x, agentInitialLocation.y);
    }

    public void intializeRaiderPosition(int xCordinate, int yCordinate) {
        raiderInitialLocation.setLocation(xCordinate, yCordinate);
    }

    public void resetRaiderPosition() {        
        agentLocation.setLocation(raiderInitialLocation.x, raiderInitialLocation.y);

    }

    public void resetAgentPosition() {        
        agentLocation.setLocation(agentInitialLocation.x, agentInitialLocation.y);
    }

    public int getIntX() {
        return (int) agentLocation.getX();
    }

    public int getIntY() {
        return (int) agentLocation.getY();
    }
    
    public Point2D.Float getAgentLocation(){
        return this.agentLocation;
    }

    public void moveAgent(int xInc, int yInc) {        
        if((xInc == 1 || xInc == -1 || xInc ==0) && (yInc == 1 || yInc == -1 || yInc ==0))
            agentLocation.setLocation(getIntX() + xInc, getIntY() + yInc);
    }

    public void setCaptured(boolean isCaptured) {
        this.isCaptured = isCaptured;
    }

    public boolean isCaptured() {
        return this.isCaptured;
    }

    public void setIsOutOfGame(boolean isOutOfGame) {
        this.isOutOfGame = isOutOfGame;
    }

    public boolean isOutOfGame() {
        return this.isOutOfGame;
    }

    public void setIsRaider(boolean isRaider) {
        this.isRaider = isRaider;
    }

    public boolean isRadier() {
        return this.isRaider;
    }

    public void setIsSucessfullRaid(boolean isSucessfullRaid){
        this.isSucessfullRaid = isSucessfullRaid;
    }

    public boolean isSucessfullRaid(){
        return this.isSucessfullRaid;
    }
    
     public void setActive(boolean mode) {
        this.isActive = mode;
    }

    public boolean isAgentActive() {
        return this.isActive;
    }  
    
    public void setTouch(boolean isTouched) {
        this.isTouched = isTouched;
    }

    public boolean isTouched() {
        return this.isTouched;
    }

    public void setAgentLocationRel(Float relPosition) {
        setAgentLocation((int)(getIntX() + relPosition.x),(int)(getIntY() + relPosition.y));
    }
    
}
