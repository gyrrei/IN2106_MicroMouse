package controller;

import bluetooth.Bluetooth;
import model.Data;
import view.AnimationTimer;
import view.BlView;
import view.InfoView;

import javax.bluetooth.RemoteDevice;
import javax.swing.*;
import java.awt.*;
import java.io.IOException;
import java.io.OutputStream;
import java.io.PrintStream;
import java.util.Timer;
import java.util.Vector;

public class Controller extends JFrame {

    private static final long serialVersionUID = 1L;
    private Data model;
    private Bluetooth bluetooth;
    private InfoView info_view;
    private BlView bluetooth_view;
    private AnimationTimer task;
    private Timer timer;
    private boolean periodic = false;

    public Controller() {
        super("MICROMOUSE");
        setDefaultCloseOperation(EXIT_ON_CLOSE);
        setSize(500, 300);
        model = new Data();
        bluetooth = new Bluetooth();
        bluetooth_view = new BlView(this, bluetooth);
        add(bluetooth_view);
        setVisible(true);
        timer = new java.util.Timer();
        task = new AnimationTimer(this);
        timer.scheduleAtFixedRate(task, 0, 20);
        switchToInfo(new InfoView(this));
        try {
            bluetooth.search();
        } catch (IOException e) {
        } catch (InterruptedException e) {
        }
        Vector<RemoteDevice> vr =  bluetooth.getDevicesDiscovered();
        int index = 0;
        try {
            for (RemoteDevice rd : vr) {
                if (rd.getFriendlyName(false).startsWith("DSD TECH")) {
                    bluetooth.serial_service(index);
                    Vector<String> vs = bluetooth.getServiceFound();
                    bluetooth.connect(vs.get(0));
                    System.out.println("Connected!!!");
                    info_view.setPeriodic();
                }
                index++;
            }
        } catch (Exception e){}
    }

    public void switchToInfo(InfoView info_view) {
        remove(bluetooth_view);
        this.info_view = info_view;
        setLayout(new BorderLayout(0, 0));
        setSize(850, 660);

        JTextArea textArea = new JTextArea(10, 60);
        textArea.setSize(750, 150);
        textArea.setBackground(Color.DARK_GRAY);
        textArea.setForeground(Color.LIGHT_GRAY);
        textArea.setFont(new Font(Font.MONOSPACED, Font.PLAIN, 12));
        System.setOut(new PrintStream(new OutputStream() {
            @Override
            public void write(int b) throws IOException {
                textArea.append(String.valueOf((char) b));
                textArea.setCaretPosition(textArea.getDocument().getLength());
            }
        }));
        add(info_view, BorderLayout.CENTER);
        add(new JScrollPane(textArea), BorderLayout.SOUTH);
    }

    public int[] getData() {
        return new int[]{model.getFront(), model.getRight(), model.getLeft(), model.getX(), model.getY(), model.getDirection()};
    }

    public void refresh() {
        String data = bluetooth.read();
        System.out.println(data);
        if(data.startsWith("X"))
            model.setX(Integer.parseInt(data.substring(data.indexOf(' ')+1)));
        else if(data.startsWith("Y"))
            model.setY(Integer.parseInt(data.substring(data.indexOf(' ')+1)));
        else if(data.startsWith("D"))
            model.setDirection(Integer.parseInt(data.substring(data.indexOf(' ')+1)));
    }


    public static void main(String[] args) {
        new Controller();
    }

    public void showRead() {
        System.out.println(bluetooth.read());
    }
}
