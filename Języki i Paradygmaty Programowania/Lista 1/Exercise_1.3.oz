declare
fun {Append L1 L2}
   {FoldR L1 fun {$ X Y} X|Y end L2}
end

fun {Reverse L}
   {FoldL L fun {$ X Y} Y|X end nil}
end
   
{Browse {Append [1 2 3 4 5] [7 8]}}

{Browse {Reverse [1 2 3 4 5]}}