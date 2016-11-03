import java.io*;
import java.util*;

public class jDefend
{

    private static String OS = System.getProperty("os.name").toLowerCase();
    
    public static void main(String[] args)
    {	
	if(! (OS.indexOf("nix") >= 0 || OS.indexOf("nux") >= 0 || OS.indexOf("aix") > 0) )
	    // This is copied from http://stackoverflow.com/questions/14288185/detecting-windows-or-linux
	    // Not sure about the usefulness of "aix". Left it in because it won't match any other OS, and someone thought it useful.
	    System.out.println( "This software is designed to be run on a Linux operating system." );

	Scanner user = new Scanner( System.in );
	String firstName = "";
	String lastName = "";
	
	System.out.println( "Enter a first name: " )

	    if( validName( user.next() ), user ) // Java limits input to 1024 characters. No overflow possible here.

		System.out.println( "Enter a last name: " )

	    if( ! validName( user.next() ), user ) // Java limits input to 1024 characters. No overflow possible here.
		
		
	
		} // end main

    public static boolean validName( String name, Scanner user )
    {
	int max = 50;
	boolean valid;
	
	while( !valid )
	    {
		if( name.length > max )
		    {
			System.out.println( "Names are limited to 50 characters. Please enter a valid name: \n" );
			name = user.next();
			valid = false;
		    }
		
		else // Name length is < 50 characters
		    {
			Pattern pattern = Pattern.compile( "^(?i)[a-z]+[//s[a-z]]?$" ); //One or more characters followed by zero or more characters or spaces.

			Matcher matcher = pattern.matcher( name );

			if( !matcher.find() ) // If name does not match regex
			    {
				System.out.println(" Names can contain only alphabetic characters and spaces. Please enter a valid name: \n" );
				name = user.next();
				valid = false;
			    }
			else // Name is < 50 chars, and input matches regex
			    valid = true;
		    }// end valid while
	    
		return matcher.find(); // This has no choice but to be true.
	    }
    }

}
