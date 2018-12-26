package myserver;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;

public class udpReceiveDemo {
	public void udpconnection() throws Exception {
		ds = new DatagramSocket(60005);
		dp = new DatagramPacket(bys,bys.length);

		System.out.println("udp server started");
		
	}
	public void udpreceive() throws IOException {
		ds.receive(dp);
		InetAddress address = dp.getAddress();
		byte[] data = dp.getData();
		length = dp.getLength(); //dp的长度是对的
		jdata = new byte[length];
		System.arraycopy(data, 0, jdata, 0, length);
		
		bytesToHexString bh = new bytesToHexString();
		String s = bh.bytesToHexString(jdata, length);
		System.out.println("receive message from nb-iot");
		System.out.println("sender ---> " + address.getHostAddress());
		System.out.println(s);
		System.out.println("udp receive succeed");

		//ds.close();
	}
	
	public void udpSendToPda(byte [] ack,int length) throws IOException {
		dpbackTopda = new DatagramPacket(ack,length,dp.getAddress(),dp.getPort());
		ds.send(dpbackTopda);
	}
	byte[] bys = new byte[1024];
	int length;
	byte [] jdata;
	public byte[] getjdata() {
		return jdata;
	}
	public int getdplength() {
		return length;
	}
	public void udpclose() {
		ds.close();
	}
	DatagramSocket ds;
	DatagramPacket dp;
	DatagramPacket dpbackTopda;

}
