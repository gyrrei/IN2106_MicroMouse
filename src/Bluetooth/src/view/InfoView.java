package view;

import controller.Controller;

import javax.swing.*;
import java.awt.*;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;
import java.awt.geom.Rectangle2D;

public class InfoView extends JPanel {

    private static final long serialVersionUID = 1L;
    private Controller contr;
    private int N = 6;
    private Rectangle2D[][] rect = new Rectangle2D[N][N];
    private boolean periodic = false, all = false;
    private int counter = 0;


    public InfoView(Controller contr) {
        super(true);
        this.contr = contr;
        setFocusable(true);
        addKeyListener(new KeyAdapter() {
            public void keyPressed(KeyEvent e) {
                final int k = e.getKeyCode();
                if (k == KeyEvent.VK_U) {
                    periodic = false;
                    if (all)
                        contr.showRead();
                    else
                        contr.refresh();
                } else if (k == KeyEvent.VK_P) {
                    periodic = true;
                } else if (k == KeyEvent.VK_Q) {
                    periodic = false;
                } else if (k == KeyEvent.VK_A) {
                    all = true;
                } else if (k == KeyEvent.VK_R) {
                    all = false;
                }

            }
        });
        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
                rect[i][j] = new Rectangle2D.Double(10 + i * 60, 10 + j * 60, 55, 55);
    }

    public void paint(final Graphics g) {
        try {
            if (periodic) {
                if (counter >= 20) {
                    if (all)
                        contr.showRead();
                    else
                        contr.refresh();
                    counter = 0;
                } else {
                    counter++;
                }
            }
            int[] data = contr.getData();
            int front = data[0], right = data[1], left = data[2];
            int x = data[3], y = data[4];
            char dir = (char) data[5];

            final Graphics2D g2d = (Graphics2D) g;
            g.setColor(Color.BLACK);
            g.fillRect(0, 0, getWidth(), getHeight());
            for (int i = 0; i < N; i++)
                for (int j = 0; j < N; j++) {
                    if (j == x && i == y)
                        g2d.setColor(Color.RED);
                    else
                        g2d.setColor(Color.WHITE);
                    g2d.fill(rect[i][j]);
                    g2d.draw(rect[i][j]);
                }
            g.setColor(Color.WHITE);
            g.drawString(front + " mm (FRONT)", 460, 30);
            g.drawString(right + " mm (RIGHT)", 460, 60);
            g.drawString(left + " mm (LEFT)", 460, 90);
            g.drawString(dir + " (Direction)", 460, 120);

            g.drawString("Press R for sending request to get update message", 460, 230);
            g.drawString("Press A for just all other data to output", 460, 260);
            g.drawString("Press U for update once", 460, 340);
            g.drawString("Press P for starting periodic update", 460, 370);
            g.drawString("Press Q for stopping periodic update", 460, 400);

            requestFocus();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public void setPeriodic(){
        periodic = true;
    }
}
