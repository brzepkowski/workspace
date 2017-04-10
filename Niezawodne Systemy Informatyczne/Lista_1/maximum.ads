package Maximum with SPARK_Mode is
	type Vector is array(Integer range <>) of Integer;
	function Maximum (A : in Vector) return Integer
		with
  		     Pre => A'Length > 0,
			Post =>
         			(for all i in A'Range => A(i) <= Maximum'Result)
         			and then
           				(for some i in A'Range => A(i) = Maximum'Result);

end Maximum;
