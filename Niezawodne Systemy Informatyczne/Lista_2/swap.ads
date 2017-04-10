pragma SPARK_Mode(on);

procedure SWAP(X: in out Integer; Y: in out Integer)
  with
    Pre => (X >= 0 and then Y <= Integer'Last - X )
  or else
    (X < 0 and then Y >= Integer'First - X),
    Post =>(X'Old = Y) and (Y'Old = X);
