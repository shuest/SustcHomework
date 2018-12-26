package myserver;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.InetAddress;
import java.net.ServerSocket;
import java.net.Socket;

public class Tcpserver3 {
	//tcp将接收到1的内容发出
	public void createConnect() {
		try {
			stoc = new ServerSocket(50005);
			s = stoc.accept();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	public void tcpconnect3(byte[] bys,int length) {
		try {
			//用50005端口,发给客户端
			
			
			OutputStream os = s.getOutputStream();
			os.write(bys, 0, length);
			InetAddress address = s.getInetAddress();
			//System.out.println("client ---> " + address.getHostAddress());
			System.out.println("send message to local client success");
			
			
			
			
			
			//is.close();
//			//关闭和本地客户端交互
			//os.close();
			//s.close();
			//stoc.close();
			
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		
	}
	public boolean ackReply() {
		//收来自客户端的ack
		
		InputStream is;
		try {
			is = s.getInputStream();
			acklength = is.read(ack);
			//System.out.println(acklength);
			if(acklength == -1)
				return false;
			jack = new byte[acklength];
			System.arraycopy(ack, 0, jack, 0, acklength);
			//System.out.println("来自客户端的ack:"+bts.bytesToHexString(ack, acklength));
			
			byte [] sendtime = new byte[1024];
			lentime = is.read(sendtime);
			System.out.println(sendtime);
			return true;
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return true;
		
	}
	ServerSocket stoc;
	Socket s;
	byte[] ack = new byte[1024];
	byte[] jack = null;
	int acklength;
	int lentime;
	public byte[] getack() {
		return jack;
	}
	public int getacklength() {
		return acklength;
	}
	public int getlentime() {
		return lentime;
	}
	bytesToHexString bts;
	
	
}
