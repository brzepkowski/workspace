with Ada.Text_IO;
with Base64;

procedure Test is
   T1 : Base64.Data_Type := (65, 66, 65);
   T2 : Base64.Encoded_Data_Type (1..4);
begin
   Ada.Text_IO.Put("      T = ");
   for I in T1'Range loop
      Ada.Text_IO.Put(Base64.Octet'Image(T1(I)));
   end loop;
   Ada.Text_IO.New_Line;
   T2 := Base64.Encode (T1);
   Ada.Text_IO.Put("   E(T) = ");
   for I in T2'Range loop
      Ada.Text_IO.Put(Base64.Sextet'Image(T2(I)));
   end loop;
   Ada.Text_IO.New_Line;
   T1 := Base64.Decode (T2);
   Ada.Text_IO.Put("D(E(T)) = ");
   for I in T1'Range loop
      Ada.Text_IO.Put(Base64.Octet'Image(T1(I)));
   end loop;
   Ada.Text_IO.New_Line;
end Test;
