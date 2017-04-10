with Ada.Text_IO; use Ada.Text_IO;
with base64;

procedure Test2 is
   T1 : Base64.Data_Type := (65, 66, 65);
   T2 : Base64.Encoded_Data_Type (1..4);
begin

   T2 := base64.Encode (T1);
   --Put_Line(base64.Sextet'Image(T2(4)));
   T1 := base64.Decode(T2);
   for i in 1..3 loop
      Put_Line(base64.Octet'Image(T1(i)));
   end loop;


end Test2;
