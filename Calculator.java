package Guy;

import java.util.*;

/**
 * Created by EladKeyshawn on 12/01/2018.
 */
public class Calculator {


    public static boolean isNumeric(String s) {
        return s != null && s.matches("[-+]?\\d*\\.?\\d+");
    }
    public static boolean isLetter(String s) {
        return s != null && s.length() == 1 && s.charAt(0) <= 122 && s.charAt(0) >= 97;
    }

    public static boolean isOperator(String s) {
        return s != null && s.length() == 1 && (s.equals("+")|| s.equals("-") || s.equals("*") || s.equals("/") );
    }

    public static double performOperation(double a, String oper , double b) {
        if(!isOperator(oper)) {
            throw new Error("No valid operator");
        }
        switch (oper) {
            case "+": {
                return a+b;
            }
            case "-": {
                return a-b;
            }
            case "*": {
                return a*b;
            }
            case "/": {
                return a/b;
            }
        }

        return 0;
    }

    public static void calculator(String[] args, Map<String, Double> variablesMap) {

        double res = 0;
        String oper = "+";
        boolean inAssign = false;
        String varAssigned = null;

        for (int i = 0; i < args.length; i++) {
            String token = args[i];
            if(isLetter(token)) {
                if(i + 1 < args.length && args[i+1].equals("=")) {
                    if(inAssign) {
                        throw new Error("Already in assignment!");
                    }
                    inAssign = true;
                    varAssigned = token;
                    i++;

                } else if(variablesMap.containsKey(token)){
                    res = performOperation(res,oper,variablesMap.get(token));
                } else  {
                    throw new Error("No identifier called: " + token);
                }
            }
            else if(isNumeric(token)) {
                res = performOperation(res,oper,Double.valueOf(token));
                oper = null;
            }
            else if(isOperator(token)) {
                oper = token;
            }
            else {
                throw new Error("No valid token: " + token);
            }
        }

        if(inAssign) {
            variablesMap.put(varAssigned,res);
        }

        System.out.println(res);

    }


    public static void main(String[] args)
    {
        Map<String,Double> variablesMap = new HashMap<>();

        Scanner scanner = new Scanner( System.in);

        System.out.print( "==== CALCULATOR ====\n\n" );

        while (true) {

            System.out.print( "> " );

            String input = scanner.nextLine();
            if (input.equals("exit")) {
                System.out.println("Good bye!");
                return;
            }
            try {
                calculator(input.split(" "),variablesMap);
            } catch (Error error) {
                System.err.println(error);
            }
        }

    }
}
