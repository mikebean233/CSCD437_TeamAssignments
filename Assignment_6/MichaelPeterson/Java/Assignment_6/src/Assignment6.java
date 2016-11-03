import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.Reader;
import java.util.Scanner;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class Assignment6 implements Runnable{

    private static String OS = System.getProperty("os.name").toLowerCase();
    private static final int INPUT_LENGTH_LIMIT = 3;


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
            return shouldMatch == Pattern.compile(regex).matcher(input).matches();
        }
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
        //if (!(OS.indexOf("nix") >= 0 || OS.indexOf("nux") >= 0 || OS.indexOf("aix") > 0)) {
        //    System.err.println("This software is designed to be run on a Linux operating system.");
        //    System.exit(1);
        //}

        String firstName, lastName;




        // regexes
        String nameRegex = "^[a-zA-Z]{1,}$";
        String numberRegex = "^(()|\\+|\\-)[0-9]{1,10}$";
        String filenameRegex_InCurrentDir = "^(\\.\\/|[^\\/])[a-zA-Z0-9\\s]+(\\.[a-zA-Z]{1,4})$";
        String filenameRegex_HasAcceptedExtension = "\\.(text|txt)$"; // White list of valid extensions, we need more ideas ...
        String filenameRegex_HasRelativePath = "\\.\\.";
        String passwordRegex = "^[0-9a-zA-Z]{12,56}$";

        /**
         * 1) get first and last name from user
         */
        RegexValidator nameVerifiers[] = {new RegexValidator(nameRegex, true, "You must enter only letters")};
        firstName = getValidatedString("Enter your first name: ", INPUT_LENGTH_LIMIT, nameVerifiers);
        lastName  = getValidatedString("Enter your last name: " , INPUT_LENGTH_LIMIT, nameVerifiers);


        /**
         * 2) get 2 32 bit ints from user
         */
        RegexValidator numberVerifiers[] = {new RegexValidator(numberRegex, true, "you must only enter digits which may be preceded with an \"+\" or \"-\"")};
        //long long integerA = getVerifiedInteger("Enter the first 32 bit integer: " ,intA, NUMBER_BUFF_LENGTH, numberVerifiers);
        //long long integerB = getVerifiedInteger("Enter the second 32 bit integer: ",intB, NUMBER_BUFF_LENGTH, numberVerifiers);
        //long long addResult  = integerA + integerB;
        //long long multResult = integerA * integerB;

        /**
         * 3) get input/output filename from user
         */
        RegexValidator filenameValidators[] = {
            new RegexValidator(filenameRegex_HasAcceptedExtension, true, "only files with the extensions .txt and .text are allowed"),
            new RegexValidator(filenameRegex_HasRelativePath,      false, "you are not allowed to use relative paths"),
            new RegexValidator(filenameRegex_InCurrentDir,         true, "you may only specify files in the current directory"),
        };
        //inputFile  = getValidFile("Enter an input file path from the current directory" , inFilename , IN_BUFF_LENGTH, filenameVerifiers, INPUT_FILE);
        //outputFile = getValidFile("Enter an output file path from the current directory", outFilename, IN_BUFF_LENGTH, filenameVerifiers, OUTPUT_FILE);
        //writeOutputFile(firstName, lastName, addResult, multResult, inputFile, outputFile, logFile);

        /**
         * 4) get password from user
         */
        RegexValidator passwordVerifiers[] = {new RegexValidator(passwordRegex, true, "You must enter between 12 and 56 characters all of which must be numbers or letters")};
        //doPasswordThing("Enter a password between 12 and 56 characters that only contains numbers and letters", passwordVerifiers);

        //fclose(inputFile);
        //fclose(outputFile);
        //fclose(logFile);
    }



    private String getValidatedString(String prompt, int inputLength, RegexValidator validators[]){
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
                System.out.println("You Must Enter fewer then " + (inputLength + 1) + " characters, try again:");
                isValid = false;
            }
            else if(thisInput.status == UserInputStatus.Empty){
                System.out.println("You must enter something...");
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
    }

    private UserInputResult getInput(int length){
        StringBuilder value = new StringBuilder();
        //Scanner scanner = new Scanner(System.in);
        InputStreamReader reader = new InputStreamReader(System.in);
        int copyCount = 0;
        char thisChar = 0;
        boolean exceededBuffer = false;

        try {
            while ((thisChar = (char)reader.read()) != '\n') {
                if(copyCount < length){
                    value.append((char)thisChar);
                    ++ copyCount;
                }
                else
                    exceededBuffer = true;
            }
        }
        catch(Exception e){
            logError(e.getMessage());
            System.exit(1);
        }
        return new UserInputResult(value.toString(), (exceededBuffer) ? UserInputStatus.TooLarge : (copyCount == 0) ? UserInputStatus.Empty : UserInputStatus.WithinLimits);
    }

    private void logError(String error){

    }

}
