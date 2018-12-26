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
			//开始时间
			long startTime = System.nanoTime();
			// 准备一个缓冲区
			byte[] buffer = new byte[1024];
			// 首先读取一次
			int len = bis.read(buffer);
			while (len != -1) {
				// 开始写入数据
				bos.write(buffer, 0, len);
				// 再读取一次
				len = bis.read(buffer);
			}
			//结束时间
			long elapsedTime = System.nanoTime() - startTime;
			System.out.println("耗时：" + (elapsedTime / 1000000.0) + " 毫秒");
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
}
