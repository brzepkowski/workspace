pragma SPARK_Mode(on);

procedure DIVIDE(X: in Natural; Y: in Positive;
		 Q: out Natural; R: out Natural)
  with
    Post => (X = Q * Y + R) and (R in 0 .. Y - 1);
