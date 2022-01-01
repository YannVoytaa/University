let bonifacy n lst=
  let rec pom poz n ls first second third=                                                (*pom w danym kroku oblicza element na pozycji poz*)(*first, second, third- kolejno pierwszy, drugi i trzeci element od konca (dokladniej x_{poz-1}, x_{poz-2}, x_{poz-3}), poz-indeks wartosci obliczanej w danym kroku, n-to samo co z tresci*)
    match ls with
      []->pom poz n lst first second third|                                               (*skonczyly sie elementy,wracamy na poczatek listy, rownoznaczne z wzieciem pozycji modulo k (bo chcemy wziac element k mod k z listy, a to = 0, czyli pierwszy element listy)*)
      h::t->if poz=n+1 then first                                                         (*chcac obliczyc x_{n+1} wiemy, ze pierwszym elementem od konca jest x_n czyli szukana wartosc (bo first=x_{poz-1}, czyli w tym przypadku x_{n+1-1}=x_n)*)
            else if poz=0 then pom (poz+1) n t 0 first second                             (*przypadki bazowe*)
            else if poz=1 then pom (poz+1) n t 1 first second                             (*przypadki bazowe*)
            else if poz=2 then pom (poz+1) n t 1 first second                             (*przypadki bazowe*)
            else if h=0  then pom (poz+1) n t (first+second) first second                 (*x_poz=... (wg wzoru rekurencyjnego), przechodzac do nastepnej pozycji owczesna ostatnia wartosc staje sie przedostatnia, a przedostatnia staje sie trzecia od konca(nowa ostatnia zostala wlasnie wyliczona), niezmiennik zachowany*)
            else (*h=1*) pom (poz+1) n t (first+third) first second                       (*tak samo jak w poprzednim przypadku, jest tylko inny wzor niz dla h=0*)
    in
    pom 0 n lst 0 0 0;;                                                                   (*wartosci first second i third sa prawdziwe dopiero dla poz>=3, ale dopiero od tego momentu ich uzywamy (wczesniej sa tylko przypadki bazowe dla ktorych od razu znamy wartosc)*)
