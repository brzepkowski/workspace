declare
local
   fun {IterReverse Reversed List}
      case List
      of nil then Reversed
      [] L|Lr then {IterReverse L|Reversed Lr}
      end
   end
in
   fun {Reverse L}
      {IterReverse nil L}
   end
end

{Browse {Reverse [1 2 237 14 8 3]}}