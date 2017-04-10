with Ada.Text_IO; use Ada.Text_IO;
with Ada.Numerics.Discrete_Random;
with Ada.Numerics.Float_Random;
with Definitions; use Definitions;

procedure Company is

   --Tasks_Board is place, where will be stored Quests for Workers to solve
   type Array_Of_Tasks is array (Integer range <>, Integer range <>) of Character;
   Tasks_Board : Array_Of_Tasks(0 .. Tasks_Board_Size - 1, 0 .. 2) := (others => (others => '-'));

   --Magazine is place, where will be stored all Products created by workers
   Magazine: array (0 .. Magazine_Size - 1) of Integer;


   task type Board is
      entry Pin_Task(First_Argument: Integer; Second_Argument: Integer; Operator: Character);
      entry Get_Task(First_Argument: out Integer; Second_Argument: out Integer; Operator: out Character);
   end Board;

   task type Shop is
      entry Put_Product(Product: Integer);
      entry Buy_Product;
   end Shop;

   task type President;
   task type Worker (ID: Integer);
   type Worker_Access is access Worker;
   task type Buyer;
   task type Listener;
   type Listener_Access is access Listener;

   Board_Pointer: Integer := 0;
   Magazine_Pointer: Integer := 0;

   -------implementations of the above types------

   task body Board is
      Board_Elements: Integer := 0;
      Board_Spaces: Integer := Tasks_Board_Size;
   begin
      loop
         select
            when Board_Spaces > 0 =>
               accept Pin_Task(First_Argument: Integer; Second_Argument: Integer; Operator: Character)
               do
                  Tasks_Board(Board_Pointer, 0) := Character'Val(First_Argument);
                  Tasks_Board(Board_Pointer, 1) := Character'Val(Second_Argument);
                  Tasks_Board(Board_Pointer, 2) := Operator;
                  Board_Pointer := Board_Pointer + 1;
                  if Listeners_Mode = "Talkative" then
                     Put_Line ("President pined task on board: " & Integer'Image(First_Argument) & " |" & Integer'Image(Second_Argument) & " | " & Character'Image(Operator));
                  end if;
                  Board_Elements := Board_Elements + 1;
                  Board_Spaces := Board_Spaces - 1;
               end;
         or
            when Board_Elements > 0 =>
               accept Get_Task(First_Argument: out Integer; Second_Argument: out Integer; Operator: out Character)
               do
                  Board_Pointer := Board_Pointer - 1; --We have to substract 1, because at this point Board_Pointer is
                                                      --pointing on first place after last element in Tasks_Board
                  Board_Elements := Board_Elements - 1;
                  Board_Spaces := Board_Spaces + 1;
                  First_Argument := Character'Pos(Tasks_Board(Board_Pointer, 0));
                  Second_Argument := Character'Pos(Tasks_Board(Board_Pointer, 1));
                  Operator := Tasks_Board(Board_Pointer, 2);
                  if Listeners_Mode = "Talkative" then
                     Put_Line("Worker got Task from board: " & Integer'Image(First_Argument) & "| " & Integer'Image(Second_Argument) & "| " & Operator);
                  end if;
               end;
         or
            terminate;
         end select;
      end loop;
   end Board;

   Board_1: Board;

   task body Shop is
      Magazine_Elements: Integer := 0;
      Magazine_Spaces: Integer := Magazine_Size;
   begin
      loop
         select
            when Magazine_Spaces > 0 =>
               accept Put_Product(Product: Integer)
               do
                  Magazine(Magazine_Pointer) := Product;
                  Magazine_Pointer := Magazine_Pointer + 1;
                  if Listeners_Mode = "Talkative" then
                     Put_Line ("Worker stored Product in magazine: " & Integer'Image(Product));
                  end if;
                  Magazine_Elements := Magazine_Elements + 1;
                  Magazine_Spaces := Magazine_Spaces - 1;
               end;
         or
            when Magazine_Elements > 0 =>
               accept Buy_Product
               do
                  Magazine_Pointer := Magazine_Pointer - 1;
                  if Listeners_Mode = "Talkative" then
                     Put_Line ("===>Buyer bought Product: " & Integer'Image(Magazine(Magazine_Pointer)));
                  end if;
                  Magazine_Elements := Magazine_Elements - 1;
                  Magazine_Spaces := Magazine_Spaces + 1;
               end;
         or
            terminate;
         end select;
      end loop;
   end Shop;

   Companys_Shop: Shop;


   procedure Produce(First_Argument: out Integer; Second_Argument: out Integer; Operator: out Character) is
      --Type and package needed to random First and Second Arguments
      type Rand_Range is range 0..100;
      package Rand_Int is new Ada.Numerics.Discrete_Random(Rand_Range);
      seed : Rand_Int.Generator;

      --Type and package needed to random Character
      package Rnd renames Ada.Numerics.Float_Random;
      Gen: Rnd.Generator;
      type Character_Array is array (Natural range <>) of Character;

      function Pick_Random(Char_Arr: Character_Array) return Character is
         -- Chooses one of the characters of Charr_Arr (uniformly distributed)
      begin
         return Char_Arr(Char_Arr'First + Natural(Rnd.Random(Gen) * Float(Char_Arr'Last)));
      end Pick_Random;

      Operators: constant Character_Array := ('+', '-', '*');
   begin
      Rand_Int.Reset(seed);
      First_Argument := Integer'Val(Rand_Int.Random(seed));
      Second_Argument := Integer'Val(Rand_Int.Random(seed));
      Rnd.Reset(Gen);
      Operator := Pick_Random(Operators);
   end Produce;

   task body President is
      First_Argument, Second_Argument: Integer;
      Operator: Character;
   begin
      loop
         Produce(First_Argument, Second_Argument, Operator);
         Board_1.Pin_Task(First_Argument, Second_Argument, Operator);
         delay Presidents_Delay;
      end loop;
   end President;

   task body Worker is
      First_Argument, Second_Argument: Integer;
      Operator: Character;
      Product: Integer;
   begin
      Put_Line("Worker [" & Integer'Image(ID) & " ] started");
      loop
         delay Workers_Delay;
         Board_1.Get_Task (First_Argument, Second_Argument, Operator);
         ----------Creation of final Product--------
         if Operator = '-' then
            Product := First_Argument - Second_Argument;
         elsif Operator = '+' then
            Product := First_Argument + Second_Argument;
         elsif Operator = '*' then
            Product := First_Argument * Second_Argument;
         end if;
         Companys_Shop.Put_Product(Product);
      end loop;
   end Worker;


   task body Buyer is
   begin
      loop
         Companys_Shop.Buy_Product;
         delay Buyers_Delay;
      end loop;
   end Buyer;

   task body Listener is
      Option: String(1 .. 14);
      Last: Natural;
   begin
      loop
         Option := "              ";
         Put_Line("Available options:");
         Put_Line("-Show Board");
         Put_Line("-Show Magazine");
         Get_Line(Option, Last);
         if Option = "Show Board    " then
            for i in 0 .. Board_Pointer - 1 loop
               Put_Line(Integer'Image(Character'Pos(Tasks_Board(i, 0))) & " | " & Integer'Image(Character'Pos(Tasks_Board(i, 1))) & " | " & Character'Image(Tasks_Board(i, 2)));
            end loop;
         elsif Option = "Show Magazine " then
            for i in 0 .. Magazine_Pointer - 1 loop
               Put_Line(Integer'Image(Magazine(i)));
            end loop;
         else
            Put_Line("Incorrect option.");
         end if;
         Put_Line("---------------------------------");
      end loop;
   end Listener;


   New_President : President;
   New_Worker: Worker_Access;
   New_Buyer : Buyer;
   New_Listener: Listener_Access;

begin
   if Listeners_Mode = "Silent" then
      New_Listener := new Listener;
   end if;

   for i in 0 .. Number_Of_Workers loop
      New_Worker := new Worker(i);
   end loop;

end Company;
