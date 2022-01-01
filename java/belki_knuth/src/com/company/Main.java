package com.company;

import java.util.Scanner;
//Kod przechodzi testy na onlinejudge
public class Main {
    static int[] tab = new int[55];
    static int[][] dp = new int[55][55];
    static int[][] argmin = new int[55][55];

    public static int belki(int n) {
        //odwrocmy problem- powiedzmy, ze mamy pociete kawalki
        //i chcemy je polaczyc, a koszt polaczenia kawalkow to ich suma
        //dp[i][j]- minimalny koszt zlaczenia kawalkow na przedziale tab[i] do tab[j]
        //argmin[i][j]- indeks k taki, ze dp[i][j]=dp[i][k]+dp[k][j]+c[i][j]
        //koszt polaczenia jednego kawalka jest zerowy-od razu go mamy
        for (int i = 0; i < n - 1; i++) {
            dp[i][i + 1] = 0;
            argmin[i][i + 1] = i;
        }
        //liczymy koszt polaczenia 2,3,...n kawalkow
        for (int size = 2; size <= n; size++) {
            for (int i = 0; i < n - size; i++) {
                //ustalamy na maxa- l<1000, wiec max wynik < 1000*50
                dp[i][i + size] = 2000000000;
                //iterujemy sie po ostatnim zlaczeniu
                //jesli na koniec laczymy na pozycji j to najpierw laczymy od i do j kosztem dp[i][j]
                //a potem laczymy od j do i+size kosztem dp[j][i+size]
                //a ostatnie laczenie zawsze kosztuje nas sume wszystkich wartosci na tym przedziale
                //do tego jest obserwacja- dp[a][c]+dp[b][d]<=dp[a][d]+dp[b][c] oraz dp[b][c]<=dp[a][d] dla
                //a<=b<=c<=d; wynika z tego, ze argmin[i][j-1]<=argmin[i][j]<=argmin[i+1][j]
                //(dowod zajmuje jakas jedna strone A5)
                for (int j = argmin[i][i + size - 1]; j <= argmin[i + 1][i + size]; j++) {
                    if (dp[i][i + size] > dp[i][j] + dp[j][i + size] + (tab[i + size] - tab[i])) {
                        dp[i][i + size] = dp[i][j] + dp[j][i + size] + (tab[i + size] - tab[i]);
                        argmin[i][i + size] = j;
                    }
                }
                //ta petla amortyzuje sie do O(n), bo argmin rosnie i kazdy element odwiedzimy max 2 razy
                //to daje nam ogolna zlozonosc O(n^2)
            }
        }
        //najtanszy koszt polaczenia wszystkich kawalkow to najtanszy koszt polaczenia przedzialu 0...n-1
        return dp[0][n - 1];
    }

    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        while (true) {
            int l = scanner.nextInt();
            if (l == 0) break;
            int n = scanner.nextInt();
            tab[0] = 0;
            tab[n + 1] = l;
            for (int i = 1; i <= n; i++) {
                tab[i] = scanner.nextInt();
            }
            System.out.println("The minimum cutting is " + belki(n + 2) + ".");
        }
    }
}
