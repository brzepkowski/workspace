with Ada.Text_IO; use Ada.Text_IO;


package body base64 with SPARK_Mode is

   procedure Copy_Octet_To_Sextet(O : in out Octet; S : in out Sextet)
   is
      I : Integer := 5;
      Zero_Octet : Octet := 2#00000000#;
   begin
      while I >= 0 loop
         Zero_Octet := O xor 2**I;
         if Zero_Octet = 0 then
            S := S xor 2**I;
         end if;
         I := I - 1;
         Zero_Octet := 2#00000000#;
      end loop;
   end Copy_Octet_To_Sextet;

   procedure Copy_Sextet_To_Octet(S : in out Sextet; O : in out Octet)
   is
      I : Integer := 5;
      Zero_Sextet : Sextet := 2#000000#;
   begin
      while I >= 0 loop
         Zero_Sextet := S xor 2**I;
         if Zero_Sextet = 0 then
            O := O xor 2**(I+2); --Przesuniecie
         end if;
         I := I - 1;
         Zero_Sextet := 2#000000#;
      end loop;
   end Copy_Sextet_To_Octet;

   function Encode (D : in Data_Type) return Encoded_Data_Type
   is
      E_Length : Integer := (D'Length * 8) / 6;
      E : Encoded_Data_Type (1..E_Length);
      Buffer_Octet : Octet;
      Buffer_Sextet : Sextet := 2#000000#;
      Leavings : Sextet := 2#000000#;
      Count : Integer := 1;
      Power1 : Integer := 0;
      Power2 : Integer := 0;
      Power3 : Integer := 0;
      Additional_Index : Integer := 0;
   begin
      while Count <= D'Length loop

         --Kopiowanie elementu tablicy D
         Power1 := (Power1 + 2) mod 8;
         if Power1 = 0 then
            Power1 := 2;
         end if;
         Buffer_Octet := D(Count) / 2**Power1;

         Buffer_Sextet := 2#000000#;
         Copy_Octet_To_Sextet(Buffer_Octet, Buffer_Sextet);
         Buffer_Sextet := Leavings + Buffer_Sextet;

         E(Count + Additional_Index) := Buffer_Sextet;

         --Kopiowanie pozostalosci z oktetu
         Power2 := (Power2 + 6) mod 8;
         if Power2 = 0 then
            Power2 := 2;
         end if;
         Buffer_Octet := D(Count) * 2**Power2;
         Buffer_Octet := Buffer_Octet / 2**Power2;

         Leavings := 2#000000#;
         Copy_Octet_To_Sextet(Buffer_Octet, Leavings);

         Power3 := (Power3 + 4) mod 6;
         Leavings := Leavings * 2**Power3;

         if Count rem 3 = 0 then
            Additional_Index := Additional_Index + 1;
            E(Count + Additional_Index) := Leavings;
            Leavings := 2#000000#;
         end if;

         Count := Count + 1;
      end loop;

      return E;

   end Encode;


   function Decode (E : in Encoded_Data_Type) return Data_Type
   is
      D_Length : Integer := (E'Length * 6) / 8;
      D : Data_Type (1..D_Length);
      Buffer_Octet : Octet := 2#00000000#;
      Buffer_Sextet : Sextet := 2#000000#;
      Leavings : Octet := 2#00000000#;
      Power1 : Integer := -2;
      Power2 : Integer := 0;
      Power3 : Integer := 0;
      Count : Integer := 1;
      Additional_Index : Integer := 1;
   begin
      while Count <= D'Length loop
         Power1 := (Power1 + 2) mod 6;

         Buffer_Sextet := E(Count) * 2**Power1;

         Buffer_Octet := 2#00000000#;
         Copy_Sextet_To_Octet(Buffer_Sextet, Buffer_Octet);

         --Put_Line(Octet'Image(Buffer_Octet));
         --D(Count) := Buffer_Octet;

         --Kopiowanie "pozostalosci" kolejnego sekstetu
         Power2 := (Power2 + 4) mod 6;
         Buffer_Sextet := E(Count + Additional_Index) / 2**Power2;

         Leavings := 2#00000000#;
         Copy_Sextet_To_Octet(Buffer_Sextet, Leavings);
         --Put_Line(Octet'Image(Leavings));
         Leavings := Leavings / 2**2;
         --Put_Line(Octet'Image(Leavings));

         Buffer_Octet := Buffer_Octet + Leavings;
         D(Count) := Buffer_Octet;
         if Count rem 3 = 0 then
            Additional_Index := Additional_Index + 1;
         end if;

         Count := Count + 1;
         --Put_Line("_________");
      end loop;

      return D;
   end Decode;


end base64;
