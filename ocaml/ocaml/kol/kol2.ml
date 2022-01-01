let wirus ls=
    (*dorzucamy indeks i dzielimy kazdy przedzial na poczatek i koniec (zaznaczamy na indeksie ktory to koniec)*)
    let (ls_poczkon,_)=List.fold_left (fun (acc,ind) (x,y) -> ((x,ind)::(y,-ind)::acc,ind+1)) ([],1) ls
    in
    (*sortujemy leksykograficznie, w ten sposob wiemy co zaczyna sie/konczy sie przed czym*)
    let sorted_ls= List.sort compare ls_poczkon
    in
    (*dla kazdej pozycji stworzymy dwie krotki: pierwsza (indeks, ile przedzialow zaczelo sie przede mna, ile przede mna sie skonczylo- czyli przede mna przecina mnie tyle ile bylo ogolnie przede mna- tyle ile sie skonczylo przede mna;
    druga krotka- indeks, ile sie zaczelo przed moim koncem, ile sie skonczylo przed moim koncem- czyli w srodku przecielo mnie tyle ile  bylo przed moim koncem - tyle ile bylo przed moim poczatkiem)*)
    let pom =fun (res,size,zdjete) (w,ind)->
        if ind<0 then ((-ind,size,zdjete)::res,size,zdjete+1)
        else ((ind,size,zdjete)::res,size+1,zdjete)
    in
    let (przed_po,_,_)=List.fold_left pom ([],0,0) sorted_ls
    in
    (*Krotki dla danej pozycji beda obok siebie jesli je posortujemy (mozemy compare, bo indeks jest na 1 pozycji, a potem bedziemy sortowac po tym ile wystapilo wczesniej. Logiczne jest, ze przed poczatkiem wystapi <= elementow niz przed koncem, tak samo z iloscia zdjetych)*)
    let res_sorted=List.sort compare przed_po
    in
    let rec stworz_wynik ls acc=match ls with
        | []-> acc
        | (_,przed,przed_skonczone)::(_,miedzy,_)::t -> stworz_wynik t (((miedzy-przed)+(przed-przed_skonczone)-1)::acc)
        (*wynik skonstruowany dla danej pozycji tak jak opisany wczesniej:wynik= ile zaczyna sie w srodku + ile zaczyna sie przed, (ile przecina w srodku=ile zaczelo sie przed koncem-ile zaczelo sie przed poczatkiem), (ile przecina bedacych przede mna=ile zaczelo sie przede mna-ile skonczylo sie przede mna)  (mozna zredukowac wynik, ale wg mnie wyglada czytelniej co jest naszym wynikiem :)*)
        | h::t -> assert false (*Dla kazdej pozycji mamy po dwie krotki*)
    in List.rev (stworz_wynik res_sorted []);;
(*Zlozonosc czasowa - O(nlogn)-sort wykonany stala liczbe razy+kilka przejsc po liscie (foldem badz rekurencyjnie)*)
(*Zlozonosc pamieciowa- O(n)- stworzylem stala liczbe dodatkowych list krotek o stalej liczbie elementow*)
