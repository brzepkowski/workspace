declare
fun {NewPortObject Fun}
   Sin in
   thread for Msg in Sin do {Fun Msg} end end
   {NewPort Sin}
end

fun {Rozmowca Fun Interlokutor}
   {NewPortObject
    proc {$ Msg}
       {Browse {Fun Msg}}
       {Delay 1000}
       {Send Interlokutor {Fun Msg}}
    end}
end

declare P1 P2 in
P1 = {Rozmowca fun {$ X} X+1 end P2}
P2 = {Rozmowca fun {$ X} X-1 end P1}

{Send P1 0}
   