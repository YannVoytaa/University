type t=Dodatni of int | Ujemny of int | Zero


let wartosc x=if x>0 then Dodatni(x)
              else if x=0 then Zero
              else Ujemny(x);;




let dodaj x y=match (x,y) with
    (Zero,a) -> a |
    (a,Zero) -> a |
    (Ujemny a, Ujemny b) -> wartosc (a+b) |
    (Dodatni a, Dodatni b) -> wartosc(a+b)|
    (Ujemny a,Dodatni b) -> wartosc(a+b)|
    (Dodatni a, Ujemny b) -> wartosc(a+b);;


let znak_mnozenia x y=match (x,y) with
    (Zero,_) -> 0 |
    (_,Zero) -> 0 |
    (Ujemny a, Ujemny b) -> 1 |
    (Dodatni a, Dodatni b) -> 1|
    (Ujemny a,Dodatni b) -> -1|
    (Dodatni a, Ujemny b) -> -1;;


let x=(1,2);;
let x=1. /. -0.00;;
