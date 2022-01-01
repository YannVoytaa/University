let wysun ls=
  let rec pom ls odp1 odp2 last=match ls with                                                                   (*pom sprawdza, czy dany element jest unikalny i dodaje go do odpowiedniej listy, przez co tworzy odpowiedz*)(*odp1 - zawiera te elementy, ktore maja powtorzenia, odp2- zawiera te, ktore nie maja, last-wartosc poprzedniego elementu (lub cos roznego od pierwszego elementu na poczatku, zeby stwierdzac, ze element przed pierwszym jest rozny od pierwszego)*)
    []->List.rev odp1@List.rev odp2|                                                                            (*elementy do list wrzucalismy na poczatek aby otrzymac zlozonosc liniowa, a chcielibysmy wrzucac na koniec, element po elemencie. problem rozwiazuje jednorazowe odwrocenie odpowiedzi kosztujace nas czas liniowy (warto zauwazyc, ze jesli 2 sasiednie wartosci sa rowne to sa nierozroznialne, czyli nie obchodzi nas ktora z nich bedzie przed ktora)*)
    h::h2::t->if h=h2 then pom t (h::h2::odp1) odp2 h2                                                           (*dwa kolejne elementy sie powtarzaja- oba maja powtorzenia- trafiaja do listy z powtorzeniami*)
              else if h=last then pom (h2::t) (h::odp1) odp2 h                                                  (*element taki sam jak wczesniejszy*)
              else (*h!=last and h!=next => h jest unikalny*) pom (h2::t) odp1 (h::odp2) h|
    h::t->if h=last then pom t (h::odp1) odp2 h                                                                 (*ostatni element listy (inaczej mialby cos za soba, czyli bylby zmatchowany z poprzednim przypadkiem)*)
          else (*h!=last and h jest ostatnim elementem => jest unikalny*) pom t odp1 (h::odp2) h
  in match ls with
  []->[]|
  h::t->pom ls [] [] (h+1) (*co do last=h+1 - dostajac pierwszy element uznamy, ze nie jest on rowny z "poprzednim"*);;          (*niepusta lista- ma jakis poczatkowy element*)
