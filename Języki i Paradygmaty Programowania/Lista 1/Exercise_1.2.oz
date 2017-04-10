declare
fun {Merge L1 L2 L3}
   case L1 # L2 # L3
   of nil # nil # nil then nil
   [] L1 # nil # nil then {MergeSort L1}
   [] nil # L2 # nil then {MergeSort L2}
   [] nil # nil # L3 then {MergeSort L3}
   [] L1 # L2 # nil then {MergeSort {Append L1 L2}}
   [] L1 # nil # L3 then {MergeSort {Append L1 L3}}
   [] nil # L2 # L3 then {MergeSort {Append L2 L3}}
   [] L1 # L2 # L3 then
      {MergeSort {Append L1 {Append L2 L3}}}      
   end
end

fun {Append Ls Ms}
   case Ls
   of nil then Ms
   [] X|Lr then X|{Append Lr Ms}
   end
end

proc {Split Xs ?Ys ?Zs}
   case Xs
   of nil then Ys=nil Zs=nil
   [] [X] then Ys=[X] Zs=nil
   [] X1|X2|Xr then Yr Zr in
      Ys=X1|Yr
      Zs=X2|Zr
      {Split Xr Yr Zr}
   end
end

fun {MergeSort Xs}
   case Xs
   of nil then nil
   [] [X] then [X]
   else Ys Zs in
      {Split Xs Ys Zs}
      {InnerMerge {MergeSort Ys} {MergeSort Zs}}
   end
end
   
fun {InnerMerge Xs Ys}
   case Xs # Ys
   of nil # Ys then Ys
   [] Xs # nil then Xs
   [] (X|Xr) # (Y|Yr) then
      if X<Y then X|{InnerMerge Xr Ys}
      else Y|{InnerMerge Xs Yr}
      end
   end
end

{Browse {Merge [1 3 5] [2 7 9] [14 2 15]}}