type 'a option = None | Some of 'a
type 'a elem = {v: 'a; mutable prev: 'a lista; mutable next: 'a lista}
and 'a lista = 'a elem option;;
exception Input_Error;;
let poodwracaj lista_elem ls=
    let last=ref None (*trzymamy wskaznik na element, ktory jest na koncu listy, ktora do tej pory odwrocilismy*)
    and
    last_new=ref None (*Przyszly ostatni wskaznik*)
    in
    let rec odwroc lista_elem ls=
        let rec pom ls_elem ilosc= (*Obracanie (n=ilosc) kolejnych elementow*)
            match ls_elem with
            | None->None
            | Some(l1)->
                if ilosc=1 then begin (*Obracamy ostatni, wiec bedziemy go laczyc z ostatnim wczesniejszym*)
                    let n=l1.next in
                    l1.next<-l1.prev;
                    l1.prev<-(!last); (*Przed aktualnym bedzie ostatni wczesniejszy*)
                    begin
                    match (!last) with
                    | None->()
                    | Some(lst)->(lst.next<-ls_elem) (*Nastepnym ostatniego wczesniejszego bedzie aktualny, bo aktualny po odwroceniu bedzie pierwszy*)
                    end;
                    last:=(!last_new); (*Nowy ostatni to pierwszy z aktualnie rozpatrywanych*)
                    last_new:=None; (*Ustalamy na None, zeby nastepnym razem "zlapac" kolejny pierwszy rozpatrywany*)
                    n
                end
                else begin
                    let n=l1.prev in
                    l1.prev<-l1.next;
                    if !last_new=None then begin (*Jest on pierwszy z aktualnie rozpatrywanych, czyli bedzie ostatni*)
                        last_new:=ls_elem;
                        l1.next<-None; (*Nie wiemy jeszcze, kto (i czy ktos) bedzie po nim skoro ma on na ten moment byc ostatni*)
                    end
                    else begin
                        l1.next<-n;
                    end;
                    pom (l1.prev) (ilosc-1); (*l1.prev to tak naprawde nastepny element w poczatkowej kolejnosci w tej sytuacji*)
                end;
        and
        przesun_jeden ls=match ls with
        | None->raise Input_Error (*Wartosci sumuja sie do m*)
        | Some(l1)->
            begin
            match (!last) with
            | None->()
            | Some(lst)->(lst.next<-ls) (*Nastepnym ostatniego wczesniejszego bedzie aktualny, bo aktualny po odwroceniu bedzie pierwszy*)
            end;
            last:=ls;
            l1.next
        in
        match ls with
        | []->()
        | 0::t->odwroc lista_elem t (*Gdy mamy obrocic 0 lub 1 to nic nie musimy robic*)
        | 1::t->odwroc (przesun_jeden lista_elem) t
        | h::t->odwroc (pom lista_elem h ) t
    in
    odwroc lista_elem ls;
    lista_elem;;
(*Obie zlozonosci liniowe od dlugosci listy dwukierunkowej- O(m)*)
