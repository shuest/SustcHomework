package myserver;

import java.io.IOException;

public class servermain {
	public static void main(String[] args) {
		try {
			// 1. 收pda终端的消息，存为bys
			udpReceiveDemo udpr = new udpReceiveDemo();
			Tcpserver3 tcpserver3 = new Tcpserver3();
			udpr.udpconnection();
			tcpserver3.createConnect();
			while (true) {
				udpr.udpreceive();
				bys = new byte[udpr.getjdata().length];
				System.arraycopy(udpr.getjdata(), 0, bys, 0, udpr.getjdata().length);
				// int lendp = udpr.getdplength();
				tcpserver3.tcpconnect3(bys, bys.length);
				if (tcpserver3.ackReply()) {	//如果有回复
					ack = tcpserver3.getack();

					// 3.与本地客户端连接,将数组内容发出

					// 4. 与pda相连，将ack传回
					int len = tcpserver3.getacklength();
					System.out.println(len);
					udpr.udpSendToPda(ack, len);
				}
			}

			// 关闭连接
			// udpr.udpclose();

		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

	static byte[] bys = null;
	static byte[] ack = null;
}
