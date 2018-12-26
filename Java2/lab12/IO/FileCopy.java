
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;

public class FileCopy {
	public static void main(String[] args) {
		try (FileInputStream in = new FileInputStream("./TestDir/src.zip");
				FileOutputStream out = new FileOutputStream("./TestDir/subDir/src.zip")) {
			//��ʼʱ�䣬��ǰϵͳ����ʱ��
			long startTime = System.nanoTime();
			// ׼��һ��������
			byte[] buffer = new byte[1024];
			// ���ȶ�ȡһ��
			int len = in.read(buffer);
			while (len != -1) {
				// ��ʼд������
				out.write(buffer, 0, len);
				// �ٶ�ȡһ��
				len = in.read(buffer);
			}
			//����ʱ�䣬��ǰϵͳ����ʱ��
			long elapsedTime = System.nanoTime() - startTime;
			System.out.println("��ʱ��" + (elapsedTime / 1000000.0) + " ����");
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
}
