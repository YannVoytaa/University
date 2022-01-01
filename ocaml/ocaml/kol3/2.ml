(*Zamysl: zakladamy, ze na poczatku wszystkie komorki sa oddzielone od innych i bedziemy zabierac sciany od konca:
  jesli zabierajac sciane zlaczymy jakies komorki to znaczy to, ze w naszym problemie nie mozemy wziac tej sciany (bo rozspojni ona pewna czesc)*)
(*W rozwiazaniu korzystam ze struktury find union z wykladu*)

let labirynt n m ls=
    for i=1 to n do
      for j=1 to m do
          Find_Union.make_set (i,j)
      done;
    done;
    let kogo_blokuje (x,y,b)=
        if b=false then ((x+1,y+1),(x,y+1))
        else  ((x+1,y+1),(x+1,y))
    let pom_fun res x=
        let (w1,w2)=kogo_blokuje x
        in
        if Find_Union.find w1 = Find_Union.find w2 then (x::res)
        else begin
        Find_Union.union w1 w2;
        res;
        end;
    in
    List.fold_left pom_fun [] (rev ls);;
(*Zlozonosc czasowa: O(n*m*log(log(n*m))), pamieciowa: O(n*m)*)
