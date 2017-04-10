package body Maximum is

   function Maximum(A: in Vector) return Integer
     with SPARK_Mode
   is
      Max : Integer := A(A'First);
   begin
      for I in A'Range
      loop
         if A(i) > Max then
            Max := A(i);
         end if;
         pragma Loop_Invariant(for all j in A'First .. I => A(j) <= Max);
         pragma Loop_Invariant(for some k in A'First .. I => A(k) = Max);
         pragma Loop_Variant(Increases => I);
      end loop;
      return Max;
  end Maximum;

end Maximum;
