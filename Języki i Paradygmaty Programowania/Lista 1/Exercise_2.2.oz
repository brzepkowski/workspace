declare P1 P2
proc {Id Name L}
   case L
   of nil then nil
   [] X|Xr then
      if (X=='ping') then
	 {Browse Name#'pong'}
	 {Delay 1000}
	 'pong'|{Id Name Xr}
      else
	 {Browse Name#'ping'}
	 {Delay 1000}
	 'ping'|{Id Name Xr}
      end
   end
end

proc {Game P1 P2}
   thread P1={Id 'P1' P2} end
   thread P2='pong'|{Id 'P2' P1} end
end

{Game P1 P2}