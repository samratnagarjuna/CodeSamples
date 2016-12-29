package Kabaddi.Simulator;

import java.awt.Color;

/**
 *
 * @author Nagarjuna
 */
public class KabaddiConstants {
    
    public static final short SIMULATOR_PANEL_WIDTH = 1000;
    public static final short SIMULATOR_PANEL_HEIGHT = 400;
    public static final short DATA_PANEL_WIDTH = 200;
    public static final short DATA_PANEL_HEIGHT = SIMULATOR_PANEL_HEIGHT;
    public static final short SIMULATION_X_OFFSET = 100;
    public static final short SIMULATION_Y_OFFSET = 50;
    public static final short SIMULATION_PANEL_WIDTH = SIMULATOR_PANEL_WIDTH - DATA_PANEL_WIDTH;
    public static final short SIMULATION_PANEL_HEIGHT = SIMULATOR_PANEL_HEIGHT;    
    public static final short CONTROL_PANEL_WIDTH = DATA_PANEL_WIDTH-10;
    public static final short CONTROL_PANEL_HEIGHT = 100;
    public static final short STATUS_PANEL_WIDTH = DATA_PANEL_WIDTH-10;
    public static final short STATUS_PANEL_HEIGHT = 150;
    public static final short FIELD_WIDTH = SIMULATION_PANEL_WIDTH - 2*SIMULATION_X_OFFSET;
    public static final short FIELD_HEIGHT = SIMULATION_PANEL_HEIGHT - 2*SIMULATION_Y_OFFSET;
    public static final short AGENT_DIAMETER = 10;
    public static final short SLEEP_TIME = 50;//In milli seconds
    public static final short MESSAGE_DELAY = 4000;
    public static final short CAPTURE_NUMBER = 2;
    public static final short BREAK_TIME = 3000;
    public static final short DESK_GAP = 30;
    //public static final Color BACKGROUND_COLOR = Color.DARK_GRAY;
    public static final Color BACKGROUND_COLOR = Color.WHITE;
    //public static final Color FIELD_COLOR = Color.LIGHT_GRAY;
    public static final Color FIELD_COLOR = Color.BLACK;
    //public static final Color TEAM_X_COLOR = new Color(0xcecece);
    public static final Color TEAM_X_COLOR = Color.BLACK;
    //public static final Color TEAM_Y_COLOR = new Color(0x54ff20);
    public static final Color TEAM_Y_COLOR = Color.GRAY;
    //public static final Color TOUCH_COLOR = new Color(0x1288ed);
    public static final Color TOUCH_COLOR = Color.LIGHT_GRAY;
    public static final int GAME_TIME = 1000;
    
}
