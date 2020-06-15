package bluetooth;

import javax.bluetooth.*;
import javax.microedition.io.Connector;
import javax.microedition.io.StreamConnection;
import java.io.*;
import java.util.Vector;

public class Bluetooth {

    private final Vector devicesDiscovered = new Vector();
    private final Vector serviceFound = new Vector();
    private final Object inquiryCompletedEvent = new Object();
    private final Object serviceSearchCompletedEvent = new Object();
    private DiscoveryListener listener;
    private BufferedReader in = null;
    private OutputStream os = null;
    private InputStream is = null;
    private StreamConnection streamConnection = null;

    public Bluetooth() {
        setup();
    }

    public void setup() {
        serviceFound.clear();
        devicesDiscovered.clear();

        listener = new DiscoveryListener() {

            int counter = 0;

            public void deviceDiscovered(RemoteDevice btDevice, DeviceClass cod) {
                devicesDiscovered.addElement(btDevice);
                try {
                    System.out.println("device " + counter++ + ": " + btDevice.getFriendlyName(false));
                } catch (IOException cantGetDeviceName) {
                }
            }

            public void inquiryCompleted(int discType) {
                System.out.println("------------------------------------");
                System.out.println();
                synchronized (inquiryCompletedEvent) {
                    inquiryCompletedEvent.notifyAll();
                }
            }

            public void serviceSearchCompleted(int transID, int respCode) {
                System.out.println("------------------------------------");
                System.out.println();
                synchronized (serviceSearchCompletedEvent) {
                    serviceSearchCompletedEvent.notifyAll();
                }
            }

            public void servicesDiscovered(int transID, ServiceRecord[] servRecord) {
                for (int i = 0; i < servRecord.length; i++) {
                    String url = servRecord[i].getConnectionURL(ServiceRecord.NOAUTHENTICATE_NOENCRYPT, false);
                    if (url == null)
                        continue;
                    serviceFound.add(url);
                    DataElement serviceName = servRecord[i].getAttributeValue(0x0100);
                    if (serviceName != null)
                        System.out.println("service " + i + ": " + serviceName.getValue());
                    else
                        System.out.println("service  " + i + ": " + url);
                }
            }
        };
    }

    public void search() throws IOException, InterruptedException {
        synchronized (inquiryCompletedEvent) {
            System.out.println();
            System.out.println("search...");
            boolean started = LocalDevice.getLocalDevice().getDiscoveryAgent().startInquiry(DiscoveryAgent.GIAC, listener);
            if (started)
                inquiryCompletedEvent.wait();
        }
    }

    public void serial_service(int device) throws IOException, InterruptedException {
        RemoteDevice btDevice = (RemoteDevice) devicesDiscovered.get(device);
        if (btDevice == null) {
            System.out.println("Device not listed. Try again.");
            return;
        }
        synchronized (serviceSearchCompletedEvent) {
            System.out.println("search services.. ");
            LocalDevice.getLocalDevice().getDiscoveryAgent().searchServices(new int[]{0x0100}, new UUID[]{new UUID(0x1101)}, btDevice, listener);
            serviceSearchCompletedEvent.wait();
        }
    }

    public void connect(String serverURL) throws IOException {
        System.out.println("Connecting to " + serverURL);
        try {
            streamConnection = (StreamConnection) Connector.open(serverURL);
        } catch (
                java.lang.ClassCastException e) {
            System.out.println("service not supported");
        }

        if (streamConnection == null)
            return;

        os = streamConnection.openOutputStream();
        is = streamConnection.openInputStream();
        in = new BufferedReader(new InputStreamReader(is));

    }

    public String read() {
        String read = "";
        try {
            read = in.readLine();
        } catch (IOException e) {
        }
        return read;
    }

    public void write(String send) throws IOException{
        os.write(send.getBytes());
    }

    public void close() throws IOException {
        if (os != null)
            os.close();
        if (is != null)
            is.close();
        if (streamConnection != null)
            streamConnection.close();
    }

    public Vector<RemoteDevice> getDevicesDiscovered() {
        return devicesDiscovered;
    }

    public Vector<String> getServiceFound() {
        return serviceFound;
    }
}
