(*Autor: Jan Wojtach, grupa 3*)

(*Przeszukuje graf mozliwych stanow kubkow szukajac pozadanego stanu*)
let bfs max wanted n=
    let q=Queue.create ()
    and
    (*odw dla danego stanu zwraca najmniejsza liczbe krokow w jakiej mozna otrzymac ten stan*)
    odw=Hashtbl.create 100000
    and
    first=Array.make n 0
    in
    Queue.add first q;
    Hashtbl.add odw first 0;
    (*Funkcja ktora dla danego stanu przechodzi do stanow mozliwych do osiagniecia jednym krokiem*)
    let gen a=
        (*Sprawdza czy stan wystapil i aktualizuje odw + jesli znajdzie szukany stan to czysci kolejke*)
        let try_to_append next=
            (*Stan juz wystapil wczesniej- nic nie robimy*)
            if Hashtbl.mem odw next then ()
            else begin
                      Hashtbl.add odw next ((Hashtbl.find odw a)+1);Queue.add next q;
                      if Hashtbl.mem odw wanted then begin
                          while Queue.is_empty q=false do
                            ignore (Queue.take q);
                            done;
                      end;
                 end;
        in
        (*Stany powstale przez dolanie/wylanie wody z dowolnego kubka*)
        for i=0 to n-1 do
            let kand=Array.copy a
            in
            kand.(i)<-0;
            try_to_append kand;
            let kand=Array.copy a
            in
            kand.(i)<-max.(i);
            try_to_append kand;
            (*Stany powstale przez przelanie wody z kubka i do j lub z kubka j do i*)
            for j=0 to i-1 do
                let kand=Array.copy a
                and
                diff=max.(i)-a.(i)-a.(j)
                in
                if diff>=0 then begin
                    kand.(i)<-a.(i)+a.(j);
                    kand.(j)<-0;
                end
                else begin
                    kand.(i)<-max.(i);
                    kand.(j)<-(-diff);
                end;
                try_to_append kand;
                let kand=Array.copy a
                and
                diff=max.(j)-a.(j)-a.(i)
                in
                if diff>=0 then begin
                    kand.(j)<-a.(j)+a.(i);
                    kand.(i)<-0;
                end
                else begin
                    kand.(j)<-max.(j);
                    kand.(i)<-(-diff);
                end;
                try_to_append kand;
            done;
        done;
    in
    while Queue.is_empty q=false do
        let akt=Queue.take q
        in
        gen akt;
        (*Jesli szukany stan zostal znaleziony w trakcie funkcji gen to konczymy, nie znajdziemy lepszego wyniku*)
        if Hashtbl.mem odw wanted then begin
            while Queue.is_empty q=false do
            ignore (Queue.take q);
            done;
        end;
    done;
    if Hashtbl.mem odw wanted then Hashtbl.find odw wanted
    (*Jesli odw nie ma wartosci dla szukanego stanu to nie da sie go uzyskac*)
    else -1;;


let przelewanka arr=
    let n=Array.length arr
    in
    let max_size=Array.make n 0
    and
    desired=Array.make n 0
    and
    (*nwd to nwd pojemnosci kubkow*)
    (*Na poczatku zakladamy, ze nie ma kubka napelnionego w calosci lub pustego; jesli  takiego znajdziemy to zmienimy zmienna niepoprawne na false*)
    nwd=ref 0 and niepoprawne=ref true
    in
    let rec gcd a b=
        if a=0 || b=0 then a+b
        else if a>=b then gcd b (a mod b)
        else gcd a (b mod a)
    in
    for i=0 to n-1 do
      let (a,b)=arr.(i)
      in
      max_size.(i)<-a;
      desired.(i)<-b;
      (*Jesli jest jakikolwiek kubek, ktory ma byc pusty lub napelniony w calosci to jeden z warunkow koniecznych jest spelniony*)
      if desired.(i)=0 || desired.(i)=max_size.(i) then niepoprawne:=false;
      nwd:=gcd (!nwd) (max_size.(i))
    done;
    (*Jesli nwd=0 to pojemnosci wszystkich kubkow (byc moze zera kubkow) sa rowne 0, wtedy szukany stan (jest jeden mozliwy) da sie osiagnac w 0 ruchach*)
    if n=0||(!nwd=0) then 0
    (*Jesli zaden kubek nie mial byc napelniony w calosci lub pusty- nie da sie uzyskac takiego stanu*)
    else if (!niepoprawne) then -1
    else begin
      for i=0 to n-1 do
          (*Jesli jakikolwiek kubek ma byc wypelniony wartoscia niepodzielna przez nwd pojemnosci to nie da sie go tak napelnic*)
          if desired.(i) mod (!nwd)<>0 then niepoprawne:=true;
      done;
      if (!niepoprawne) then -1
      else bfs max_size desired n;
    end;
