import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Scanner;

public class Read {
    private static boolean created_yet;
    public static Read createRead(){
        if(created_yet){
            System.out.println("Reader został już stworzony wcześniej");
            System.exit(1);
            return null;
        }
        created_yet = true;
        return new Read();
    }
    private Read(){
        
    }
    private Scanner scanner;
    private final HashMap<String, String> res = new HashMap<>();
    private final ArrayList<char[]> plansza = new ArrayList<>();
    public int getInt(String param) {
        if(res.containsKey(param)) return Integer.parseInt(res.get(param));
        else noArg(param);
        return 0;
    }
    public double getDouble(String param) {
        if(res.containsKey(param)) return Double.parseDouble(res.get(param));
        else noArg(param);
        return 0;
    }
    private void noArg(String param){
        System.out.println("Nie znaleziono parametru "+param);
        System.exit(1);
    }
    public String get(String param) {
        if(res.containsKey(param)) return res.get(param);
        else noArg(param);
        return "";
    }
    public ArrayList<char[]> getPlansza() {
        return plansza;
    }
    public void readPlansza() {
        try{
            scanner = new Scanner(new File("/home/yannvoytaa/IdeaProjects/Roby/src/plansza.txt"));
        }
        catch (Exception e){
            System.out.println("File doesnt exist");
        }
        while(scanner.hasNextLine()){
            plansza.add(scanner.nextLine().toCharArray());
        }
        res.put("rozmiar_planszy_x", String.valueOf(plansza.size()));
        if(plansza.size()!=0)
            res.put("rozmiar_planszy_y", String.valueOf(plansza.get(0).length));
        else
            res.put("rozmiar_planszy_y", "0");
    }
    public void readParams() {
        readParametry();
        readPlansza();
    }
    public void readParametry() {
        try{
            scanner = new Scanner(new File("/home/yannvoytaa/IdeaProjects/Roby/src/parametry.txt"));
        }
        catch (Exception e){
            System.out.println("File doesnt exist");
        }
        while(scanner.hasNextLine()){
            res.put(scanner.next(), scanner.next());
        }
    }
}
