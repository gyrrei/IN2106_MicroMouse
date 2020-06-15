package view;

import controller.Controller;
import javax.swing.*;
import java.util.TimerTask;

public class AnimationTimer extends TimerTask {

    private Controller contr;

    public AnimationTimer(Controller contr) {
        super();
        this.contr = contr;
    }

    public void run() {
        SwingUtilities.invokeLater(new Runnable() {
            public void run() {
                contr.repaint();
            }
        });
    }
}