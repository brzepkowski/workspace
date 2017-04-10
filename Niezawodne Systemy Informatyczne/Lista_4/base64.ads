pragma SPARK_Mode(on);

package Base64 is

   type Octet is mod 2**8;

   type Sextet is mod 2**6;

   type Data_Type is array (Positive range <>) of Octet;

   type Encoded_Data_Type is array (Positive range <>) of Sextet;

   function Encode (D : in Data_Type) return Encoded_Data_Type
     with
       Pre => D'Length rem 3 = 0 and D'Last < Positive'Last / 4,
     Post => Encode'Result'Length = 4 * (D'Length / 3) and Decode(Encode'Result) = D;

  function Decode (E : in Encoded_Data_Type) return Data_Type
     with
       Pre => E'Length rem 4 = 0 and E'Last < Positive'Last / 3,
     Post => Decode'Result'Length = 3 * (E'Length / 4) and Encode(Decode'Result) = E;

end Base64;
