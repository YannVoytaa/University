let p f n =
    let rec p2 f n a b b2 suma suma_max =
        if b2 > n then b-a+1
        else
        let nowa_suma=suma+f b2 in
        if nowa_suma<0 then p2 f n (b2+1) b (b2+1) 0 suma_max
        else if nowa_suma >= suma_max then p2 f n a b2 (b2+1) nowa_suma nowa_suma
        else p2 f n a b (b2+1) nowa_suma suma_max

    in
    p2 f n 0 0 0 0 0
    ;;
