package myserver;

public class bytesToHexString{
	public String bytesToHexString(byte[] bArr,int len) {
	    StringBuffer sb = new StringBuffer(len);
	    String sTmp;

	    for (int i = 0; i < bArr.length; i++) {
	        sTmp = Integer.toHexString(0xFF & bArr[i]);
	        if (sTmp.length() < 2)
	            sb.append(0);
	        sb.append(sTmp.toUpperCase());
	    }

	    return sb.toString();
	}
}