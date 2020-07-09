import java.util.Scanner;

public class FolderSearch {

	static {
		System.loadLibrary("hello");
	}

	private native String[] startSearch(String path,String pat);

	public static void main(String args[]) {
		Scanner scan = new Scanner(System.in);
		System.out.println("Enter the folder path");
		String path = scan.nextLine(); //  ".";
		System.out.println("Enter the pattern");
		String pat = scan.nextLine(); // "FolderSearch";
		String[] res = new FolderSearch().startSearch(path,pat);
		System.out.println("The Output files list are ....");
		for(int i = 0; i < res.length; i++) {
			System.out.println(res[i]);
		}
	}
}