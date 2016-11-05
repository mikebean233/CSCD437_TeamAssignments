import javax.crypto.SecretKeyFactory;
import javax.crypto.spec.PBEKeySpec;
import java.io.*;
import java.nio.file.*;
import java.nio.file.attribute.BasicFileAttributeView;
import java.nio.file.attribute.FileOwnerAttributeView;
import java.text.SimpleDateFormat;
import java.util.Arrays;
import java.util.Date;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import java.security.SecureRandom;

public class Assignment6 implements Runnable{

    private static String OS = System.getProperty("os.name").toLowerCase();
    private static String userName = System.getProperty("user.name").toLowerCase();
    private static final int STRING_INPUT_LENGTH = 50;
    private static final int INT_INPUT_LENGTH    = 11;
    private static final int PWD_INPUT_LENGTH    = 56;
    private static boolean TEST                  = false;

    File logFile;


    public static void main(String[] args) {
        (new Assignment6()).run();
    }


    private class RegexValidator{
        public String regex;
        public Boolean shouldMatch;
        public String failDescription;

        public RegexValidator(String inRegex, Boolean inShouldMatch, String inFailDescription){
            if(inRegex == null || inShouldMatch == null || inFailDescription == null)
                throw new NullPointerException();

            regex = inRegex;
            shouldMatch = inShouldMatch;
            failDescription = inFailDescription;
        }

        public Boolean validate(String input){
            Matcher matcher = Pattern.compile(regex).matcher(input);

            return shouldMatch == matcher.matches();
        }
    }

    private enum FileType{
        Input,
        Output
    }

    private enum UserInputStatus{
        Empty,
        WithinLimits,
        TooLarge
    }

    private class UserInputResult{
        String value;
        UserInputStatus status;

        public UserInputResult(String inValue, UserInputStatus inStatus){
            value = inValue;
            status = inStatus;
        }
    }

    @Override
    public void run() {
        // First things first, make sure this isn't being ran as root
        if("root".equals(userName)){
            System.err.println("This program must not be run as the root user!");
            System.exit(1);
        }

        if (!(OS.indexOf("nix") >= 0 || OS.indexOf("nux") >= 0 || OS.indexOf("aix") > 0)) {
            System.err.println("This software is designed to be run on a Linux operating system.");
            System.exit(1);
        }

        String firstName, lastName;

        // Files
        File inputFile, outputFile;

        logFile = new File("error.log");
        if(!logFile.exists()) {
            try {
                logFile.createNewFile();
            }
            catch(Exception e){
                System.err.println(e.getMessage());
                System.exit(1);
            }
        }

        if(!logFile.canWrite()){
            System.err.println("Unable to write to log file \"error.log\"");
            System.exit(1);
        }


        // regexes
        String nameRegex = "^[a-zA-Z]{1,}$";
        String numberRegex = "^(()|\\+|\\-)[0-9]{1,10}$";
        String filenameRegex_InCurrentDir = "^(\\.\\/|[^\\/])[a-zA-Z0-9\\s]+(\\.[a-zA-Z]{1,4})$";
        String filenameRegex_HasAcceptedExtension = "[^\\.]{1,}\\.(text|txt)$"; // White list of valid extensions, we need more ideas ...
        String filenameRegex_HasRelativePath = "\\.\\.";
        String passwordRegex = "^(?=(.*[a-z].*))(?=(.*[A-Z].*))(?=.*\\d.*)(?=.*\\W.*)[a-zA-Z0-9\\S]{12,56}$";

        if(TEST){
            String nameTestCases[]     = {"", "\0", "\n", "Jhon", " Jhon", "1ll124lk1 1 4l ", "bob", "a", "13", "bob ", "0", "..", "Jo hn"};
            String numberTestCases[]   = {"", "\0", "\n", "0", " 0", "0 ", " 0 ", "-0", "+0", "123", "-123", "+123", "- 123", "123-", "+0123546789", "+01235467891", " ", "apple", "abc", "-12345678901", "--12345678", "-+34567890", "- 34567890", "123456789-","123456789 +", "123456789++", "-123456789-", "(123*456)", "0000000000", "-0000000000", "*1234567890", "1-23456789", "--", "--0", "0000000000000001"};
            String filenameTestCases[] = {"", "\0", "\n", "file.dog", "file.Txt", "file.txt", "./file/../.txt", "/file.txt", "./file.txt", "../file.txt", "/../file.txt", "./ /", "\\/bin\\/file.txt", "*.txt", "file.*"};
            String passwordTestCases[] = {"", "\0", "\n", "abcde", "Slg3k4k23j4Dkj4k23j4kn234jh", "2l23k23ISFLk#lk429safnsf3", "2l23k23lSFLk#lk429saf\nsf3", ";2l23k23l SFLk#lk429'saf\nsf3 ", "abcDEF123@#$abc", "!@12asAS90123456fifty-one-charachers789012345678901","abCD$%789012345fifty-characters2345678901234567890"};

            runRegexTestCases(nameRegex                         , nameTestCases);
            runRegexTestCases(numberRegex                       , numberTestCases);
            runRegexTestCases(filenameRegex_InCurrentDir        , filenameTestCases);
            runRegexTestCases(filenameRegex_HasAcceptedExtension, filenameTestCases);
            runRegexTestCases(filenameRegex_HasRelativePath     , filenameTestCases);
            runRegexTestCases(passwordRegex                    , passwordTestCases);
	    }


        /**
         * 1) get first and last name from user
         */
        RegexValidator nameVerifiers[] = {new RegexValidator(nameRegex, true, "You must enter only letters")};
        firstName = getValidString("Enter your first name: ", STRING_INPUT_LENGTH, nameVerifiers);
        lastName  = getValidString("Enter your last name: " , STRING_INPUT_LENGTH, nameVerifiers);


        /**
         * 2) get 2 32 bit ints from user
         */
        RegexValidator numberVerifiers[] = {new RegexValidator(numberRegex, true, "You must only enter a maximum of ten digits, which may be preceded with an \"+\" or \"-\"")};
        long integerA = getValidInteger("Enter the first 32 bit integer: ", INT_INPUT_LENGTH, numberVerifiers);
        long integerB = getValidInteger("Enter the second 32 bit integer: ",INT_INPUT_LENGTH, numberVerifiers);
        long addResult  = integerA + integerB;
        long multResult = integerA * integerB;

        /**
         * 3) get input/output filename from user
         */
        RegexValidator filenameValidators[] = {
            new RegexValidator(filenameRegex_HasAcceptedExtension, true, "Only files with the extensions .txt and .text are allowed"),
            new RegexValidator(filenameRegex_HasRelativePath,      false, "You are not allowed to use relative paths"),
            new RegexValidator(filenameRegex_InCurrentDir,         true, "You may only specify files in the current directory"),
        };
        inputFile = getValidFile("Enter an input file path from the current directory"  , STRING_INPUT_LENGTH, filenameValidators, FileType.Input);
        outputFile = getValidFile("Enter an output file path from the current directory", STRING_INPUT_LENGTH, filenameValidators, FileType.Output);
        writeOutputFile(firstName, lastName, addResult, multResult, inputFile, outputFile);

        /**
         * 4) get password from user
         */
        RegexValidator passwordVerifiers[] = {new RegexValidator(passwordRegex, true, "Criteria not met.")};
	    doPasswordThing("Enter a password between 12 and 56 characters that contains one lower-case letter, one upper-case letter, one numerical digit, and one symbol.", passwordVerifiers);

    }// End run()

    private void doPasswordThing(String prompt, RegexValidator validators[]){
        try {

            boolean isValid = false;
            byte[] salt = new byte[8];
            // Generate salt
            SecureRandom random = SecureRandom.getInstance("SHA1PRNG");
            random.nextBytes(salt);

            // Get the hash of the password
            byte[] originalHash = SecretKeyFactory
                    .getInstance("PBKDF2WithHmacSHA1")
                    .generateSecret(new PBEKeySpec(getValidString(prompt, PWD_INPUT_LENGTH, validators).toCharArray(), salt, 65536, 128))
                    .getEncoded();


            while(!isValid){
                String passwordGuess = getValidString("Re-enter your password", PWD_INPUT_LENGTH, validators);

                // Get the hash of the guess
                byte[] guessHash = SecretKeyFactory
                        .getInstance("PBKDF2WithHmacSHA1")
                        .generateSecret(new PBEKeySpec(passwordGuess.toCharArray(), salt, 65536, 128))
                        .getEncoded();

                if(Arrays.equals(originalHash, guessHash)){
                    System.out.println("Congratulations, you matched your own password!");
                    isValid = true;
                }
                else{
                    System.out.println("- That was the wrong password");
                    isValid = false;
                }
            }// End while
        }// End try
        catch(Exception e){
            System.err.println();
            logError(e.getMessage());
            System.exit(1);
        }
    }// End doPasswordThing

    private String getValidString(String prompt, int inputLength, RegexValidator validators[]){
        if(prompt == null || validators == null)
            throw new NullPointerException();

        boolean isValid = false;
        UserInputResult thisInput = null;

        while(!isValid){
            isValid = true;

            System.out.println(prompt);
            System.out.println(">");
            thisInput = getInput(inputLength);

            if(thisInput.status == UserInputStatus.TooLarge){
                System.out.println("- You must enter fewer than " + (inputLength + 1) + " characters");
                isValid = false;
            }
            else if(thisInput.status == UserInputStatus.Empty){
                System.out.println("- You must enter something.");
            }
            else {
                for (RegexValidator thisValidator : validators){
                    if (!thisValidator.validate(thisInput.value)) {
                        isValid = false;
                        System.out.println("- " + thisValidator.failDescription);
                    }
                }
            }
        }// End while(!isValid)
        return thisInput.value;
    }// End getValidString

    private UserInputResult getInput(int length){
        StringBuilder value = new StringBuilder();

        InputStreamReader reader = new InputStreamReader(System.in);
        int copyCount = 0;
        char thisChar = 0;
        boolean exceededBuffer = false;

        try {
            while ((thisChar = (char)reader.read()) != '\n') {
                if(copyCount++ < length)
                    value.append(thisChar);
                else
                    exceededBuffer = true;
            }
        }
        catch(Exception e){
            logError(e.getMessage());
            System.out.println(e.getMessage());
            System.exit(1);
        }
        return new UserInputResult(value.toString(), (exceededBuffer) ? UserInputStatus.TooLarge : (copyCount == 0) ? UserInputStatus.Empty : UserInputStatus.WithinLimits);
    }// End getInput

    long getValidInteger(String prompt, int length, RegexValidator validators[]){
        boolean isValid = false;

        long returnValue = 0;

        while(!isValid) {
            String thisInput = getValidString(prompt, length, validators);
            isValid = true;
            try {
                returnValue = Long.parseLong(thisInput);
                if(returnValue > Integer.MAX_VALUE){
                    System.out.println("- The provided value exceeds the maximum value for a 32 bit signed integer");
                    isValid = false;
                }

                if(returnValue < Integer.MIN_VALUE){
                    System.out.println("- The provided value is less than the minimum value for a 32 bit signed integer");
                    isValid = false;
                }
            }
            catch(NumberFormatException e){ // This should never happen...
                logError(e.getMessage());
                System.out.println("- " + e.getMessage());
                isValid = false;
            }
        }// End while(!isValid)
        return returnValue;
    }// getValidInteger

    File getValidFile(String prompt, int length, RegexValidator validators[], FileType type){
        File validFile = null;

        boolean isValid = false;
        while(!isValid){
            String thisInput = getValidString(prompt, length,validators);

            try {
                Path thisPath = Paths.get(thisInput);
                boolean exists = Files.exists(thisPath);

                isValid = true;
                if (type == FileType.Input) {
                    if(!exists){
                        System.out.println("- You must specify an input file that exists.");
                        isValid = false;
                    }
                    else{
                        BasicFileAttributeView attributes = Files.getFileAttributeView(thisPath, BasicFileAttributeView.class, LinkOption.NOFOLLOW_LINKS);
                        FileOwnerAttributeView ownerAttributes = Files.getFileAttributeView(thisPath, FileOwnerAttributeView.class, LinkOption.NOFOLLOW_LINKS);


                        // Make sure we are dealing with a regular file
                        if(!attributes.readAttributes().isRegularFile()){
                            System.out.println("- You must specify a regular file (directories, character/block devices, named pipes, sockets, and symbolic links are not allowed)");
                            isValid = false;
                        }

                        // Make sure the current user is actually the owner of the input file
                        if(!ownerAttributes.getOwner().getName().equals(userName)){
                            System.out.println("- You must specify a file that you own");
                            isValid = false;
                        }

                        // Make sure the current user has read privileges for the specified file
                        validFile = thisPath.toFile();
                        if(!validFile.canRead()){
                            System.out.println("- You must specify a file that you have read privileges to");
                            isValid = false;
                        }
                    }// End else
                } else if (type == FileType.Output) {
                    if(exists){
                        System.out.println("- You must specify a file that doesn't exist already");
                        isValid = false;
                    }
                    else{
                        validFile = thisPath.toFile();
                    }
                }// End else if
            }
            catch(Exception e){
                logError(e.getMessage());
                System.err.println(e.getMessage());
                System.exit(1);
            }
        }// End while(!isValid)
        return validFile;

    }// End getValidFile

    private void writeOutputFile(String firstName, String lastName, long addResult, long multResult, File inputFile, File outputFile){
        String newLine = System.lineSeparator();
        String header = firstName + newLine + lastName + newLine + addResult + newLine + multResult + newLine;

        byte[] thisByte = new byte[1];

        try {
            PrintStream printStream = new PrintStream(outputFile);
            BufferedInputStream reader = new BufferedInputStream(new FileInputStream(inputFile));
            printStream.print(header);

            while(reader.read(thisByte) > 0){
                printStream.write(thisByte);
            }
            printStream.close();
            reader.close();
        }
        catch(Exception e){
            logError(e.getMessage());
            System.err.println(e.getMessage());
            System.exit(1);
        }
    }// End writeOutputFile

    private void logError(String error){
        try {
            PrintStream printStream = new PrintStream(logFile);
            printStream.println((new SimpleDateFormat("yyyy-MM-dd HH:mm:ss").format(new Date())) + " - " +  error);
        }
        catch(Exception e){
            System.err.println(e.getMessage());
            System.exit(1);
        }
    }

    private void runRegexTestCases(String regex, String inputs[]){
        System.out.println("---- Test Cases for Regex: " + regex + "---------");
        System.out.println("  Is Match  |  Input  ");
        System.out.println("----------------------");

        RegexValidator thisValidator = new RegexValidator(regex, true, "");

        for(String thisInput: inputs){
            System.out.printf("%12s|%12s",(thisValidator.validate(thisInput) ? "YES" : "NO"), "\"" + thisInput + "\"");
            System.out.println();
        }
        System.out.println();
    }
}
