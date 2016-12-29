package Kabaddi.Controller;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import javax.swing.Timer;

/**
 *
 * @author Nagarjuna
 */
public class CountDownTimer {
    
    private int counter;
    ActionListener timerTimeOut = new ActionListener(){
        @Override
        public void actionPerformed(ActionEvent e){
            counter++;
        }
    };
    private Timer timer = null;
    private int countDownTime;
    
    public CountDownTimer(int countDownTime){
        this.countDownTime = countDownTime;
    }    
    
    public void resetCounter(){
        this.counter = 0;
    }
    
    public int getHrs(){
        return counter/3600;
    }
    
    public int getMins(){
        return counter/60;
    }
    
    public int getSecs(){
        return counter%60;
    }
    
}
