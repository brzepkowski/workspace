declare
    proc {Insert Key Value TreeIn ?TreeOut}
       if TreeIn == leaf then TreeOut = tree(Key Value leaf leaf)
       else  
          local tree(K1 V1 T1 T2) = TreeIn in 
             if Value == V1 then TreeOut = tree(Key Value T1 T2)
             elseif Value < V1 then 
                 local T in 
                    TreeOut = tree(K1 V1 T T2)
                    {Insert Key Value T1 T}
                 end 
             else 
                 local T in 
                    TreeOut = tree(K1 V1 T1 T)
                    {Insert Key Value T2 T}
                 end  
             end 
          end 
       end 
    end

    proc {DFS T V List}
       case T
       of leaf then skip
       [] tree(key:Key value:Val left:L right:R) then
	  {DFS L V List}
	  if (Val==V) then
	     {Browse Key#Val}
	  end
	  {DFS R V Key|List}
       end
    end
    

declare T0 T1 T2
{Insert bob 10 leaf T0}
{Insert alex 2 T0 T1}
{Insert aalex 15 T1 T2}
%{Browse T2}

%{DFS T2 15}
    

T3 = tree(key:a value:111
left:tree(key:b value:55 left:tree(key:c value:55 left:leaf right:leaf) right:leaf) right:leaf)
{Browse T3}
{DFS T3 55 nil}
/*
T4 = tree(key:a value:111
left:tree(key:b value:55
left:tree(key:x value:100
left:tree(key:z value:56 left:leaf right:leaf)
right:tree(key:w value:23 left:leaf right:leaf))
right:tree(key:y value:105 left:leaf
right:tree(key:r value:77 left:leaf right:leaf)))
right:tree(key:c value:123
left:tree(key:d value:119
left:tree(key:g value:44 left:leaf right:leaf)
right:tree(key:h value:50
left:tree(key:i value:5 left:leaf right:leaf)
right:tree(key:j value:6 left:leaf right:leaf)))
	   right:tree(key:e value:133 left:leaf right:leaf)))
{DFS T4 133}*/