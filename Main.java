class InfoLogProcessor extends LogProcessor{
    InfoLogProcessor(LogProcessor nextLogProcessor) {
        super(nextLogProcessor);
    }

    public void log(int logLevel, String message) {
        if(logLevel == INFO) {
            System.out.println("INFO : " + message);
        } else {
            super.log(logLevel, message);
        }
    }
}

class DebugLogProcessor extends LogProcessor{
    DebugLogProcessor(LogProcessor nextLogProcessor) {
        super(nextLogProcessor);
    }

    public void log(int logLevel, String message) {
        if(logLevel == DEBUG) {
            System.out.println("INFO : " + message);
        } else {
            super.log(logLevel, message);
        }
    }
}
class ErrorLogProcessor extends LogProcessor{
    ErrorLogProcessor(LogProcessor nextLogProcessor) {
        super(nextLogProcessor);
    }

    public void log(int logLevel, String message) {
        if(logLevel == ERROR) {
            System.out.println("INFO : " + message);
        } else {
            super.log(logLevel, message);
        }
    }
}


abstract class LogProcessor {
    public static int INFO = 1;
    public static int DEBUG = 2;
    public static int ERROR = 3;

    LogProcessor nextLogProcessor;

    LogProcessor(LogProcessor processor) {
        this.nextLogProcessor = processor;
    }

    public void log(int logLevel, String message) {
        if(nextLogProcessor != null) {
            nextLogProcessor.log(logLevel, message);
        }
    }
}

public class Main {
    public static void main(String[] args) {
        LogProcessor logObject = new InfoLogProcessor(new DebugLogProcessor(new ErrorLogProcessor(null)));

        logObject.log(LogProcessor.ERROR, "SHIT HAPPENS");
        logObject.log(LogProcessor.DEBUG, "FINDING SHIT");
        logObject.log(LogProcessor.INFO, "SHIT FOUND");
    }
}
