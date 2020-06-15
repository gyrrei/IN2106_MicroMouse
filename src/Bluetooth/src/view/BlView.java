package view;

import bluetooth.Bluetooth;
import controller.Controller;

import javax.bluetooth.RemoteDevice;
import javax.swing.*;
import java.awt.*;
import java.io.IOException;
import java.io.OutputStream;
import java.io.PrintStream;
import java.util.Vector;

public class BlView extends JPanel {

    private static final long serialVersionUID = 1L;
    private Controller contr;
    private Bluetooth bluetooth;
    private JComboBox deviceList, serviceList;

    public BlView(Controller contr, Bluetooth bluetooth) {
        super(true);
        this.contr = contr;
        this.bluetooth = bluetooth;
        JButton search_button = new JButton("search.. ");
        search_button.addActionListener(e -> {
            try {
                bluetooth.search();
                Vector<RemoteDevice> devices = bluetooth.getDevicesDiscovered();
                deviceList.removeAllItems();
                deviceList.addItem("");
                for (int i = 0; i < devices.size(); i++) {
                    deviceList.addItem(devices.get(i).getFriendlyName(false));
                }
            } catch (Exception f) {
            }
        });
        String[] input = {""};
        deviceList = new JComboBox(input);
        deviceList.addActionListener(e -> {
            JComboBox cb = (JComboBox) e.getSource();
            int id = cb.getSelectedIndex();
            if (id != 0) {
                try {
                    bluetooth.serial_service(id - 1);
                    Vector<String> services = bluetooth.getServiceFound();
                    serviceList.removeAllItems();
                    serviceList.addItem(" ");
                    for (int i = 0; i < services.size(); i++) {
                        serviceList.addItem(services.get(i));
                    }
                } catch (Exception f) {
                }
            }
        });

        String[] inputService = {""};
        serviceList = new JComboBox(inputService);
        serviceList.addActionListener(e -> {
            JComboBox cb = (JComboBox) e.getSource();
            String service = (String) cb.getSelectedItem();
            try {
                if (!service.equals("")) {
                    bluetooth.connect(service);
                    contr.switchToInfo(new InfoView(contr));
                }
            } catch (Exception f) {
            }
        });

        JTextArea textArea = new JTextArea(10, 60);
        textArea.setBackground(Color.BLACK);
        textArea.setForeground(Color.LIGHT_GRAY);
        textArea.setFont(new Font(Font.MONOSPACED, Font.PLAIN, 12));
        System.setOut(new PrintStream(new OutputStream() {
            @Override
            public void write(int b) throws IOException {
                textArea.append(String.valueOf((char) b));
                textArea.setCaretPosition(textArea.getDocument().getLength());
            }
        }));
        add(search_button);
        add(deviceList);
        add(serviceList);
        add(new JScrollPane(textArea));
        setFocusable(true);
    }

}
