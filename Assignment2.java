//package Assignments;
import java.net.*;
import java.io.*;
import java.nio.charset.StandardCharsets;
public class Assignment2 {
	public static void main(String args[]){
		String url = "https://en.wikipedia.org/wiki/Modern_Greek";
		InputStream is = null;
		try 
		{
			URL u = new URL(url);
			System.out.println("Protocol: " + u.getProtocol());
			System.out.println("User Info: " + u.getUserInfo());
			System.out.println("Host: " + u.getHost());
			System.out.println("Port: " + u.getPort());
			System.out.println("Path: " + u.getPath());
			System.out.println("Query: " + u.getQuery());
			System.out.println("Fragment: " + u.getRef());
			
			//is = u.openStream();
			//is = new BufferedInputStream(is);
			BufferedReader ir = new BufferedReader(new InputStreamReader(u.openStream()));
			
			BufferedWriter writer = new BufferedWriter(new FileWriter("out.html"));
			//Reader ir = new InputStreamReader(is, "UTF8");
			int i;
			char check;
			while ((i = ir.read()) != -1 ){
				check = invertGreek((char)i);
				writer.write(check);
			}
			ir.close();
			writer.close();
		}
		catch (MalformedURLException e) 
		{
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		catch(IOException ex) 
		{
			System.out.println("IO Exception: " + ex);
		}
		finally {
				if (is != null){
				 try{
				 is.close();
				 }
				 catch (IOException e)
				 {
				 // ignore
				 }
				}
			}
		}
	//}
	
	public static char invertGreek(char c) {
		char base = '\u0390';
		//upper case
		for(int i = 1; i < 26; i++) {
			base = '\u0390';
			base += i;
			if(base == c) {
				if(Character.isUpperCase(base)) {
					base = Character.toLowerCase(base);
					return base;
				}
			}
		}
		
		//lower case
		for(int i = 1; i < 26; i++) {
			base = '\u03B0';
			base += i;
			if(base == c) {
				if(Character.isLowerCase(base)) {
					base = Character.toUpperCase(base);
					return base;
				}
			}
		}
		return c;
	}
}
