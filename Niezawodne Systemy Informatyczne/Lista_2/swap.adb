pragma SPARK_Mode(on);

procedure SWAP(X: in out Integer; Y: in out Integer)
is
begin
	X:= X + Y;
	Y := X - Y;
	X := X - Y;
end SWAP;
