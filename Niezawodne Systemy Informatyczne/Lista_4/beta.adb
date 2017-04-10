with Ada.Text_IO; use Ada.Text_IO;
with Interfaces; use Interfaces;


package body beta is

   type Bit is mod 2**1;


   function Encode (D : in Octet) return Sextet
   is
      E: Sextet := 2#011100#;
      F: Sextet;
   begin
      F := E * 4;
      Put_Line(Sextet'Image(F));

   return E;

end Encode;

end beta;
