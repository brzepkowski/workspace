declare
fun {Swap Xs N Y}
    case Xs of nil then nil
    [] _|Xr then
        if N==1 then Y|Xr
        else {Swap Xr N-1 Y} end
    end
end

proc {SwapCells A B}
     B := A := @B
  end


fun {Append Ls Ms}
   case Ls
   of nil then Ms
   [] X|Lr then X|{Append Lr Ms}
   end
end



/*

fun {MySwap X1 X2 N Y}
    case X1
    of Xh|Xr then
       if N==1 then
	  {Browse Y|Xr}
	  {Browse X2}
       else
	   {MySwap Xr {Append X2 Xh} N-1 Y} end
    end
end
*//*
proc {Permutacje L K}
   X Y L1 L2
in
   if K==1 then {Browse L}
   else
      for I in 1..K do
	 X = {List.nth L I}
	 Y = {List.nth L K}
	 {Permutacje {SwapCells {List.nth L I} {List.nth L K}} K-1}
      end
   end
end
*/
declare T X Y
T = [1 2 3]
{Browse T}
X = {NewCell a}
Y = {NewCell b}
{Browse @X}
{Browse @Y}
{Assign X {List.nth T 1}}
{Assign Y {List.nth T 2}}
{SwapCells X Y}
{Browse @X}
{Browse T}
   
/*
X = {List.nth T 1}
Y = {List.nth T 3}
T1 = {Swap T 3 X}
T2 = {Swap T1 1 Y}
{Browse T1}*/

{Permutacje T 3}
