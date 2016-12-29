package Kabaddi.Comm;

import java.awt.geom.Point2D;

/**
 *
 * @author Nagarjuna
 */
public class SerialCommands {    
    
    
    public static String getCommandRelPostion(Point2D.Float relPosition){
        int xPos = 0;
        int yPos = 0;
        xPos = (int)relPosition.getX();
        yPos = (int)relPosition.getY();
        String command = null;
        /*if(xPos == 1 && yPos == 0) command = "0";
        else if(xPos == 1 && yPos == 1) command = "1";
        else if(xPos == 1 && yPos == -1) command = "2";
        else if(xPos == 0 && yPos == 1) command = "3";
        else if(xPos == 0 && yPos == -1) command = "4";
        else if(xPos == -1 && yPos == 0) command = "5";
        else if(xPos == -1 && yPos == 1) command = "6";
        else if(xPos == -1 && yPos == -1) command = "7";
        else if(xPos == 0 && yPos == 0) command = "8";*/
        
        if(xPos == 1 && yPos == 0) command = "4";
        else if(xPos == 1 && yPos == 1) command = "2";
        else if(xPos == 1 && yPos == -1) command = "7";
        else if(xPos == 0 && yPos == 1) command = "0";
        else if(xPos == 0 && yPos == -1) command = "5";
        else if(xPos == -1 && yPos == 0) command = "3";
        else if(xPos == -1 && yPos == 1) command = "1";
        else if(xPos == -1 && yPos == -1) command = "6";
        else if(xPos == 0 && yPos == 0) command = "8";
        return command;
    }
}
