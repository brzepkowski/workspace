package beta is

   type Octet is mod 2**8;

   type Sextet is mod 2**6;

   type Data_Type is array (Positive range <>) of Octet;

   type Encoded_Data_Type is array (Positive range <>) of Sextet;

   function Encode (D : in Octet) return Sextet;
end beta;
