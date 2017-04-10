with Ada.Text_IO; use Ada.Text_IO;
with Ada.Numerics.Discrete_Random;
with Ada.Numerics.Float_Random;
with Definitions; use Definitions;
with Synchronous_Fifo;

procedure Company is

   type Basic_Record is record
      Argument_1: Integer;
      Operator: Character;
      Argument_2: Integer;
      Product: Integer;
   end record;

   protected type Adding_Machine(ID: Integer) is
      procedure Store_Record(Task_Record: Basic_Record);
      procedure Push_Button;
      procedure Get_Product(Product: out Integer);
      procedure Is_Broken(IsBroken: out Boolean);
      procedure Repair;
   private
      My_Record : Basic_Record;
      Broken: Boolean := False;
   end Adding_Machine;

   protected type Multiplying_Machine(ID: Integer) is
      procedure Store_Record(Task_Record: Basic_Record);
      procedure Push_Button_One;
      procedure Push_Button_Two;
      procedure Get_Product(Product: out Integer);
      procedure Get_Business(Button_Pushed: out Boolean);
      procedure Is_Broken(IsBroken: out Boolean);
      procedure Repair;
   private
      My_Record : Basic_Record;
      Button_Two_Pushed : Boolean := False;
      Broken: Boolean := False;
   end Multiplying_Machine;

   task type Board is
      entry Pin_Task(Task_Record: Basic_Record);
      entry Get_Task(Task_Record: out Basic_Record; ID: Integer);
   end Board;

   task type Shop is
      entry Put_Product(Product: Integer; ID: Integer);
      entry Buy_Product;
   end Shop;

   task type President;
   task type Worker (ID: Integer);
   type Worker_Access is access Worker;
   task type Buyer;
   task type Listener;
   type Listener_Access is access Listener;
   type Adding_Machine_Access is access Adding_Machine;
   type Multiplying_Machine_Access is access Multiplying_Machine;

   --Tasks_Board is place, where will be stored Quests for Workers to solve
   type Array_Of_Tasks is array (Integer range <>) of Basic_Record;
   Tasks_Board : Array_Of_Tasks(0 .. Tasks_Board_Size - 1);

   --Magazine is place, where will be stored all Products created by workers
   Magazine: array (0 .. Magazine_Size - 1) of Integer;

   type Array_Of_Adding_Machines is array (Integer range <>) of Adding_Machine_Access;
   Adding_Machines : Array_Of_Adding_Machines(0 .. Number_Of_Adding_Machines - 1);

   type Array_Of_Multiplying_Machines is array (Integer range <>) of Multiplying_Machine_Access;
   Multiplying_Machines : Array_Of_Multiplying_Machines(0 .. Number_Of_Multiplying_Machines - 1);

   task type Free_Adding_Machines is
      entry Get_Adding_Machine(Add_Machine: out Adding_Machine_Access);
      entry Return_Adding_Machine(Add_Machine: Adding_Machine_Access);
      entry Store_Broken_Machine(Add_Machine: Adding_Machine_Access);
      entry Get_Broken_Machine(Add_Machine: out Adding_Machine_Access);
   end Free_Adding_Machines;

   task type Free_Multiplying_Machines is
      entry Get_Multiplying_Machine(Mult_Machine: out Multiplying_Machine_Access);
      entry Store_Busy_Machine(Mult_Machine: Multiplying_Machine_Access);
      entry Get_Busy_Machine(Mult_Machine: out Multiplying_Machine_Access);
      entry Return_Multiplying_Machine(Mult_Machine: Multiplying_Machine_Access);
      entry Get_Number_Of_Needed_Workers(Workers_Needed: out Integer);
      entry Store_Broken_Machine(Mult_Machine: Multiplying_Machine_Access);
      entry Get_Broken_Machine(Mult_Machine: out Multiplying_Machine_Access);
   end Free_Multiplying_Machines;

   task type Mechanic;



   Broken_Adding_Machines : Integer := 0;
   Broken_Multiplying_Machines : Integer := 0;
   Workers_Needed_By_Other_Machines : Integer := 0;
   Board_Pointer: Integer := 0;
   Magazine_Pointer: Integer := 0;

   -------implementations of the above types------

   procedure Random_Breaking(Broken: out Boolean) is
      --Type and package needed to random First and Second Arguments
      type Rand_Range is range 0..100;
      package Rand_Int is new Ada.Numerics.Discrete_Random(Rand_Range);
      seed : Rand_Int.Generator;
      Random: Integer;
   begin
      Rand_Int.Reset(seed);
      Random := Integer'Val(Rand_Int.Random(seed));
      if Random > 90 then
         Broken := True;
      else
         Broken := False;
      end if;
   end Random_Breaking;

   protected body Adding_Machine is
      procedure Store_Record(Task_Record: Basic_Record) is
      begin
         My_Record := Task_Record;
      end Store_Record;
      procedure Push_Button is
      begin
         Put_Line("Button pushed in Adding_Machine [" & Integer'Image(ID) & " ]");
         Random_Breaking(Broken);
         if Broken = True then
            Put_Line("--------------->Machine [" & Integer'Image(ID) & " ] is broken");
         else
         if My_Record.Operator = '-' then
            My_Record.Product := My_Record.Argument_1 - My_Record.Argument_2;
         else
            My_Record.Product := My_Record.Argument_1 + My_Record.Argument_2;
         end if;
            delay Adding_Machines_Delay;
         end if;
      end Push_Button;
      procedure Get_Product(Product: out Integer) is
      begin
         Product := My_Record.Product;
      end Get_Product;
      procedure Is_Broken(IsBroken: out Boolean) is
      begin
         IsBroken := Broken;
      end Is_Broken;
      procedure Repair is
      begin
         Put_Line("Machine is repaired");
         Broken := False;
      end Repair;
   end Adding_Machine;

   protected body Multiplying_Machine is
      procedure Store_Record(Task_Record: Basic_Record) is
      begin
         My_Record := Task_Record;
      end Store_Record;
      procedure Push_Button_One is
      begin
         Put_Line("Button 1 pushed in Multiplying_Machine [" & Integer'Image(ID) & " ]");
         Random_Breaking(Broken);
         if Broken = True then
            Put_Line("Machine [" & Integer'Image(ID) & " ] is broken");
         else
         --Multiplying_Machines_Which_Need_Second_Worker(ID - 1) := 1;
         --Workers_Needed_By_Other_Machines := Workers_Needed_By_Other_Machines + 1;
            Button_Two_Pushed := False;
         end if;
      end Push_Button_One;
      procedure Push_Button_Two is
      begin
         --Multiplying_Machines_Which_Need_Second_Worker(ID - 1) := 0; --!!!!!!!!!!!!!!!!!!!!!!!!!!!!1Może bedzie można usunąc
         My_Record.Product := My_Record.Argument_1 * My_Record.Argument_2;
         --Workers_Needed_By_Other_Machines := Workers_Needed_By_Other_Machines -1;
         Button_Two_Pushed := True;
         delay Multiplying_Machines_Delay;
         Put_Line("Button 2 pushed in Multiplying_Machine [" & Integer'Image(ID) & " ]");
      end Push_Button_Two;
      procedure Get_Product(Product: out Integer) is
      begin
         Product := My_Record.Product;
      end Get_Product;
      procedure Get_Business(Button_Pushed: out Boolean) is
      begin
         Button_Pushed := Button_Two_Pushed;
      end Get_Business;
      procedure Is_Broken(IsBroken: out Boolean) is
      begin
         IsBroken := Broken;
      end Is_Broken;
      procedure Repair is
      begin
         Put_Line("Machine is repaired");
         Broken := False;
      end Repair;
   end Multiplying_Machine;

   task body Free_Adding_Machines is
      --Free_Machines : Integer := Number_Of_Adding_Machines;
      Adding_Machines_Pointer : Integer := Number_Of_Adding_Machines;
      Broken_Machines_Pointer: Integer := 0;
      Broken_Machines : Array_Of_Adding_Machines(0 .. Number_Of_Adding_Machines - 1);
   begin
      loop
         select
            when Adding_Machines_Pointer > 0 =>
               accept Get_Adding_Machine(Add_Machine: out Adding_Machine_Access)
               do
                  Add_Machine := Adding_Machines(Adding_Machines_Pointer-1);
                  Put_Line("Pobrano maszynę dodającą [" & Integer'Image(Adding_Machines_Pointer) & " ]");
                  Adding_Machines_Pointer := Adding_Machines_Pointer - 1;
               end;
         or
              --Może trzeba bedzie dodać jakiś when
            accept Return_Adding_Machine(Add_Machine: Adding_Machine_Access)
            do
               Adding_Machines_Pointer := Adding_Machines_Pointer + 1;
               Put_Line("Oddano maszynę dodającą[" & Integer'Image(Adding_Machines_Pointer) & " ]");
               Adding_Machines(Adding_Machines_Pointer-1) := Add_Machine;
            end;
         or
            accept Store_Broken_Machine(Add_Machine: Adding_Machine_Access)
            do
               Broken_Machines(Broken_Machines_Pointer) := Add_Machine;
               Broken_Machines_Pointer := Broken_Machines_Pointer + 1;
               Broken_Adding_Machines := Broken_Adding_Machines + 1;
            end;
         or
            when Broken_Adding_Machines > 0 =>
            accept Get_Broken_Machine(Add_Machine: out Adding_Machine_Access)
            do
               Add_Machine := Broken_Machines(Broken_Machines_Pointer);
                  Broken_Machines_Pointer := Broken_Machines_Pointer -1;
               end;
         end select;
      end loop;
   end Free_Adding_Machines;

   Free_Add_Machines : Free_Adding_Machines;

   task body Free_Multiplying_Machines is
      --Free_Machines : Integer := Number_Of_Adding_Machines;
      Multiplying_Machines_Pointer : Integer := Number_Of_Multiplying_Machines;
      Busy_Machines_Pointer : Integer := 0;
      Busy_Machines : Array_Of_Multiplying_Machines(0 .. Number_Of_Adding_Machines - 1);
      Broken_Machines_Pointer: Integer := 0;
      Broken_Machines : Array_Of_Multiplying_Machines(0 .. Number_Of_Adding_Machines - 1);
   begin
      loop
         select
            when Multiplying_Machines_Pointer > 0 =>
               accept Get_Multiplying_Machine(Mult_Machine: out Multiplying_Machine_Access)
               do
                  Mult_Machine := Multiplying_Machines(Multiplying_Machines_Pointer-1);
                  Put_Line("Pobrano maszynę mnożącą [" & Integer'Image(Multiplying_Machines_Pointer) & " ]");
                  Multiplying_Machines_Pointer := Multiplying_Machines_Pointer - 1;
               end;
         or
            --when Busy_Machines_Pointer <= Number_Of_Multiplying_Machines => -- Może bedzie można to usunąć
               accept Store_Busy_Machine(Mult_Machine: Multiplying_Machine_Access) do
                  Busy_Machines_Pointer := Busy_Machines_Pointer + 1;
                  Busy_Machines(Busy_Machines_Pointer - 1) := Mult_Machine;
                  Workers_Needed_By_Other_Machines := Workers_Needed_By_Other_Machines + 1;
               end;
         or
            when Busy_Machines_Pointer > 0 =>
               accept Get_Busy_Machine(Mult_Machine: out Multiplying_Machine_Access)
               do
                  Mult_Machine := Busy_Machines(Busy_Machines_Pointer-1);
                  Busy_Machines_Pointer := Busy_Machines_Pointer - 1;
                  Workers_Needed_By_Other_Machines := Workers_Needed_By_Other_Machines -1;
               end;
         or
              --Może trzeba bedzie dodać jakiś when
            accept Return_Multiplying_Machine(Mult_Machine: Multiplying_Machine_Access)
            do
               Multiplying_Machines_Pointer := Multiplying_Machines_Pointer + 1;
               Put_Line("Oddano maszynę mnożącą [" & Integer'Image(Multiplying_Machines_Pointer) & " ]");
               Multiplying_Machines(Multiplying_Machines_Pointer-1) := Mult_Machine;
            end;
         or
            accept Get_Number_Of_Needed_Workers(Workers_Needed: out Integer)
            do
               Workers_Needed := Workers_Needed_By_Other_Machines;
            end;
         or
            accept Store_Broken_Machine(Mult_Machine: Multiplying_Machine_Access)
            do
               Broken_Machines(Broken_Machines_Pointer) := Mult_Machine;
               Broken_Machines_Pointer := Broken_Machines_Pointer + 1;
               Broken_Multiplying_Machines := Broken_Multiplying_Machines + 1;
            end;
         or
            when Broken_Multiplying_Machines > 0 =>
               accept Get_Broken_Machine(Mult_Machine: out Multiplying_Machine_Access)
               do
                  Mult_Machine := Broken_Machines(Broken_Machines_Pointer);
                  Broken_Machines_Pointer := Broken_Machines_Pointer -1;
               end;
         end select;
      end loop;
   end Free_Multiplying_Machines;

   Free_Mult_Machines : Free_Multiplying_Machines;


   task body Board is
      Board_Elements: Integer := 0;
      Board_Spaces: Integer := Tasks_Board_Size;
   begin
      loop
         select
            when Board_Spaces > 0 =>
               accept Pin_Task(Task_Record: Basic_Record)
               do
                  Tasks_Board(Board_Pointer) := Task_Record;
                  Board_Pointer := Board_Pointer + 1;
                  if Listeners_Mode = "Talkative" then
                     Put_Line ("President pined task on board: " & Integer'Image(Task_Record.Argument_1) & " |" & Integer'Image(Task_Record.Argument_2) & " | " & Task_Record.Operator);
                  end if;
                  Board_Elements := Board_Elements + 1;
                  Board_Spaces := Board_Spaces - 1;
               end;
         or
            when Board_Elements > 0 =>
               accept Get_Task(Task_Record: out Basic_Record; ID: Integer)
               do
                  Board_Pointer := Board_Pointer - 1; --We have to subtract 1, because at this point Board_Pointer is
                  --pointing on first place after last element in Tasks_Board
                  Board_Elements := Board_Elements - 1;
                  Board_Spaces := Board_Spaces + 1;
                  Task_Record := Tasks_Board(Board_Pointer);
                  if Listeners_Mode = "Talkative" then
                     Put_Line("Worker [" & Integer'Image(ID) & " ] got Task from board: " & Integer'Image(Task_Record.Argument_1) & "| " & Integer'Image(Task_Record.Argument_2) & "| " & Task_Record.Operator);
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
               accept Put_Product(Product: Integer; ID: Integer)
               do
                  Magazine(Magazine_Pointer) := Product;
                  Magazine_Pointer := Magazine_Pointer + 1;
                  if Listeners_Mode = "Talkative" then
                     Put_Line ("Worker [" & Integer'Image(ID) & " ] stored Product in magazine: " & Integer'Image(Product));
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


   procedure Produce(Task_Record: out Basic_Record) is
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
      Task_Record.Argument_1 := Integer'Val(Rand_Int.Random(seed));
      Task_Record.Argument_2 := Integer'Val(Rand_Int.Random(seed));
      Rnd.Reset(Gen);
      Task_Record.Operator := Pick_Random(Operators);
   end Produce;

   task body President is
      Created_Task: Basic_Record;
   begin
      loop
         Produce(Created_Task);
         Board_1.Pin_Task(Created_Task);
         delay Presidents_Delay;
      end loop;
   end President;

   task body Worker is
      My_Record: Basic_Record;
      My_Adding_Machine : Adding_Machine_Access;
      My_Multiplying_Machine : Multiplying_Machine_Access;
      Button_Two_Pushed : Boolean;
      Needed_Workers: Integer;
      Adding_Machine_Broken: Boolean;
      Multiplying_Machine_Broken: Boolean;
   begin
      Put_Line("Worker [" & Integer'Image(ID) & " ] started");
      loop
         delay Workers_Delay;
         Free_Mult_Machines.Get_Number_Of_Needed_Workers(Needed_Workers);
         if Needed_Workers > 0 then
            Free_Mult_Machines.Get_Busy_Machine(My_Multiplying_Machine);
            My_Multiplying_Machine.Push_Button_Two;
         else
         Board_1.Get_Task (My_Record, ID);
         ----------Creation of final Product--------
         if My_Record.Operator = '-' then
            <<One_Broken_Adding_Machine>>
            Free_Add_Machines.Get_Adding_Machine(My_Adding_Machine);
            My_Adding_Machine.Store_Record(My_Record);
            My_Adding_Machine.Push_Button;
               My_Adding_Machine.Is_Broken(Adding_Machine_Broken);
               if Adding_Machine_Broken = True then
                  Free_Add_Machines.Store_Broken_Machine(My_Adding_Machine);
                  goto One_Broken_Adding_Machine;
               else
                  My_Adding_Machine.Get_Product(My_Record.Product);
                  Free_Add_Machines.Return_Adding_Machine(My_Adding_Machine);
               end if;
         elsif My_Record.Operator = '+' then
            <<Two_Broken_Adding_Machine>>
            Free_Add_Machines.Get_Adding_Machine(My_Adding_Machine);
            My_Adding_Machine.Store_Record(My_Record);
            My_Adding_Machine.Push_Button;
               My_Adding_Machine.Is_Broken(Adding_Machine_Broken);
               if Adding_Machine_Broken = True then
                  Free_Add_Machines.Store_Broken_Machine(My_Adding_Machine);
                  goto Two_Broken_Adding_Machine;
               else
                  My_Adding_Machine.Get_Product(My_Record.Product);
                  Free_Add_Machines.Return_Adding_Machine(My_Adding_Machine);
               end if;
            elsif My_Record.Operator = '*' then
               <<Broken_Multiplying_Machine>>
               Free_Mult_Machines.Get_Multiplying_Machine(My_Multiplying_Machine);
               My_Multiplying_Machine.Store_Record(My_Record);
               My_Multiplying_Machine.Push_Button_One;
                  My_Multiplying_Machine.Is_Broken(Multiplying_Machine_Broken);
                  if Multiplying_Machine_Broken = True then
                     Free_Mult_Machines.Store_Broken_Machine(My_Multiplying_Machine);
                     goto Broken_Multiplying_Machine;
                  else
                     Free_Mult_Machines.Store_Busy_Machine(My_Multiplying_Machine);
                     My_Multiplying_Machine.Get_Business(Button_Two_Pushed);
                     while Button_Two_Pushed = False loop
                       --Put_Line("Worker [" & Integer'Image(ID) & " ] is waiting for other worker");
                        My_Multiplying_Machine.Get_Business(Button_Two_Pushed);
                     end loop;
                     My_Multiplying_Machine.Get_Product(My_Record.Product);
                     Free_Mult_Machines.Return_Multiplying_Machine(My_Multiplying_Machine);
                  end if;
               end if;
               Companys_Shop.Put_Product(My_Record.Product, ID);
         end if;
      end loop;
      end Worker;

   task body Mechanic is
      Broken_Adding_Machine : Adding_Machine_Access;
      Broken_Multiplying_Machine : Multiplying_Machine_Access;
      begin
      loop
         if Broken_Adding_Machines > 0 then
            Free_Add_Machines.Get_Broken_Machine(Broken_Adding_Machine);
            --Broken_Adding_Machine.Repair;
            Broken_Adding_Machines := Broken_Adding_Machines - 1;
            Free_Add_Machines.Return_Adding_Machine(Broken_Adding_Machine);
            Put_Line("------------------>Mechanic repaired Adding_Machine");
         elsif Broken_Multiplying_Machines > 0 then
            Free_Mult_Machines.Get_Broken_Machine(Broken_Multiplying_Machine);
            --Broken_Multiplying_Machine.Repair;
            Broken_Multiplying_Machines := Broken_Multiplying_Machines - 1;
            Free_Mult_Machines.Return_Multiplying_Machine(Broken_Multiplying_Machine);
            Put_Line("------------------>Mechanic repaired Multiplying_Machine");
         end if;
      end loop;
   end Mechanic;


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
               Put_Line(Integer'Image(Tasks_Board(i).Argument_1) & " | " & Integer'Image(Tasks_Board(i).Argument_2) & " | " & Tasks_Board(i).Operator);
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
   New_Mechanic:  Mechanic;
   New_Worker: Worker_Access;
   New_Buyer : Buyer;
   New_Listener: Listener_Access;
   New_Adding_Machine : Adding_Machine_Access;
   New_Multiplying_Machine : Multiplying_Machine_Access;

begin
   if Listeners_Mode = "Silent" then
      New_Listener := new Listener;
   end if;

   for i in 0 .. Number_Of_Adding_Machines-1 loop
      New_Multiplying_Machine := new Multiplying_Machine(i);
      Multiplying_Machines(i) := New_Multiplying_Machine;
   end loop;

   for i in 0 .. Number_Of_Multiplying_Machines-1 loop
      New_Adding_Machine := new Adding_Machine(i);
      Adding_Machines(i) := New_Adding_Machine;
   end loop;

   for i in 0 .. Number_Of_Workers loop
      New_Worker := new Worker(i);
   end loop;

end Company;
