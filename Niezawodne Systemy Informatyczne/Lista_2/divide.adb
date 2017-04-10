pragma SPARK_Mode(on);

procedure DIVIDE(X: in Natural; Y: in Positive;
		 Q: out Natural; R: out Natural)
is
begin
   Q := 0;
   R := X;
   while R >= Y loop
      R := R - Y;
      Q := Q + 1;
      pragma Loop_Invariant(X = Q * Y + R);
   end loop;
end DIVIDE;
