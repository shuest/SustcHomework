import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;

public class FileCopyWithBuffer {
	public static void main(String[] args) {
		try (FileInputStream fis = new FileInputStream("./TestDir/src.zip");
				BufferedInputStream bis = new BufferedInputStream(fis);
				FileOutputStream fos = new FileOutputStream("./TestDir/subDir/src.zip");
				BufferedOutputStream bos = new BufferedOutputStream(fos)) {		
			//��ʼʱ��
			long startTime = System.nanoTime();
			// ׼��һ��������
			byte[] buffer = new byte[1024];
			// ���ȶ�ȡһ��
			int len = bis.read(buffer);
			while (len != -1) {
				// ��ʼд������
				bos.write(buffer, 0, len);
				// �ٶ�ȡһ��
				len = bis.read(buffer);
			}
			//����ʱ��
			long elapsedTime = System.nanoTime() - startTime;
			System.out.println("��ʱ��" + (elapsedTime / 1000000.0) + " ����");
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
}
