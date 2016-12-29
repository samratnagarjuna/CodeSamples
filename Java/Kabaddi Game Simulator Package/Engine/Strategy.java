package Kabaddi.Engine;

import Kabaddi.Agent.Agent;
import java.awt.geom.Point2D;

/**
 *
 * @author Nagarjuna
 */
public interface Strategy{
    public Point2D.Float moveRaiderAgent(Agent raiderAgent,Point2D.Float[] antiRaiderPositions,int antiRaiderScore);
    public Point2D.Float moveAntiRaiderAgent(Agent antiRaiderAgent,Point2D.Float[] antiRaiderPositions,Point2D.Float raiderPosition,int raiderScore);
    public Point2D.Float antiRaiderInitialPosition(int index,int noOfActive,int noOfActiveRaiders);
    public Point2D.Float RaiderInitialPosition(int noOfActive,int noOfActiveAntiRaiders);
    public String agentID(int index);
}
