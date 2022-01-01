let podzial ls=
  let (_,res,res_last)=List.fold_right
    (fun x (last,odp,akt) ->
      if last=Some x then (Some x,akt::odp,[x])
      else (Some x,odp,x::akt)) ls (None, [],[])
  in
  if res_last=[] then res
  else res_last::res;;



  type kolor = Czerwona | Zielona;;

  let kulki ls=
    let (ilosc_czerwonych,ilosc_zielonych)=List.fold_left
      (fun (red,green) x -> if x=Czerwona then (red+1,green) else (red,green+1)) (0,0) ls
    in
    let dominujacy= if ilosc_czerwonych>=ilosc_zielonych then Czerwona else Zielona
    in
    let (_,zle_stojace)=List.fold_left
      (fun (poz,zle) x ->
        if poz >= (min ilosc_czerwonych ilosc_zielonych)*2 && x=dominujacy then (poz+1,zle)
        else if poz mod 2=1 && x=Zielona then (poz+1, zle)
        else if poz mod 2=0 && x=Czerwona then (poz+1, zle)
        else (poz+1,zle+1)) (1,0) ls
    in
    zle_stojace/2;;
