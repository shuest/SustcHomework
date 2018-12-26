package com.fantai.util;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.InetAddress;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.Scanner;

public class client {
	public static void main(String[] args)  {
	
		Socket s;
		while(true) {
		try {
			//s = new Socket("118.126.90.148",50005);
			s = new Socket("localhost",50005);
			System.out.println("客户端socket连接成功");
			DataMsg dataMsg = new DataMsg();
			while(true) {
				InputStream is = s.getInputStream();
				byte[] buffer =	new byte[1024];
				int len;
				len = is.read(buffer);
				if(len == -1)  {	//没有应答
					System.out.println("no return");
					OutputStream os = s.getOutputStream();
					os.write(buffer);
					continue;
				}
					byte[] jbuffer = new byte[len];
				//buffer.length 是1024
				//截断
				System.arraycopy(buffer, 0, jbuffer, 0, len);
				//测试
				
				System.out.println(jbuffer);
				System.out.println(jbuffer.length);
				System.out.println(len);
				
				//s.shutdownInput();
				
				
				OutputStream os = s.getOutputStream();
				try {
					dataMsg.RecieveDataMsg(jbuffer, len, 0, s, os);
				} catch (Exception e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
				//s.shutdownOutput();
				
				//dataMsg.sendControlMsg(addr, func, content);
			} 
		}catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		}
		
	}

}

