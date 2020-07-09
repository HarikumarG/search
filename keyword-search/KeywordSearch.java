import java.io.FileWriter;
import java.util.Scanner;
public class KeywordSearch {

	static {
		System.loadLibrary("hello");
	}

	private native void startKeySearch(String path,String filename,String keyword);

	private static FileWriter fwrite;

	private void filewrite(String text) {
		try {
			fwrite.write(text);
			fwrite.write("\n");
		} catch(Exception e) {
			System.out.println("Error in writing");
		}
	}

	public static void main(String args[]) {
		Scanner scan = new Scanner(System.in);
		System.out.println("Enter the path");
		String path = scan.nextLine(); // ".";
		System.out.println("Enter the filename");
		String filename = scan.nextLine(); // "Input.txt";
		System.out.println("Enter the keyword to be searched");
		String keyword = scan.nextLine(); // "Mother";
		
		try {
			FileWriter fw = new FileWriter("./Output.txt");
			fwrite = fw;
			new KeywordSearch().startKeySearch(path,filename,keyword);
			fwrite.close();
		} catch (Exception e) {
			System.out.println("Error in opening Output.txt file");
		}
	}
}