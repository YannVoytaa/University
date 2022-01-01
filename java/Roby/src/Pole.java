import java.util.ArrayList;

public class Pole {
    private int x,y;
    public Pole(int x, int y){
        this.x = x;
        this.y = y;
    }
    public int getX(){
        return x;
    }
    public int getY(){
        return y;
    }
    public void symuluj(){

    }
    public boolean maPożywienie(){
        return false;
    }
    public void jedzPożywienie(){
        assert false;
    }
    public int energiaZPożywienia(){
        return 0;
    }

    @Override
    public String toString() {
        return "O";
        //pole bez jedzenia - O
        //pole z jedzeniem (w danym momencie symulacji) - X
    }
}
