with Ada.Text_IO; use Ada.Text_IO;
with Semaphores; use Semaphores;
procedure company is

   Product: Integer := 0;

   N: constant Integer := 5;
   Board: array(0..N-1) of Integer;
   --Magazine: array (0 .. 100) of Integer;
   --Pointer: Integer := 0;
   In_Ptr, Out_Ptr: Integer := 0;
   Board_Elements: Semaphore(0);
   Board_Spaces: Semaphore(N);

 --  type Array_Of_Tasks is array (Integer range <>,
 --                               Integer range <>) of Character;

   --Board : Array_Of_Tasks (0 .. 9, 0 .. 2);

   task type President;
   task type Worker (ID: Integer);
   type Worker_Access is access Worker;
   task type Buyer;

   -------implementations of the above types------

   procedure Produce is
   begin
      Product := Product + 1;
   end Produce;

   task body President is
   begin
      loop
         Produce;
         Board_Spaces.Wait;
         Board(In_Ptr) := Product;
         In_Ptr := (In_Ptr + 1) mod N;
         Put_Line("Inserted element: " & Integer'Image(Product) & " || Pointer = " & Integer'Image(In_Ptr));
         Board_Elements.Signal;
         delay 5.0;
      end loop;
   end President;

   task body Worker is
      I: Integer;
   begin
      Put_Line("Worker: " & Integer'Image(ID) & " started");
      loop
         delay 2.0;
         Board_Elements.Wait;
         I := Board(Out_Ptr);
         Out_Ptr := (Out_Ptr + 1) mod N;
         Board_Spaces.Signal;
         Put_Line("Worker [" & Integer'Image(ID) & " ] Removed element: " & Integer'Image(I) & " || Pointer = " & Integer'Image(Out_Ptr));
      end loop;
   end Worker;


   task body Buyer is
   begin
      loop
         Put_Line("Buyer");
         delay 3.0;
      end loop;
   end Buyer;



   New_President : President;
   New_Worker: Worker_Access;
   --New_Buyer : Buyer;

begin
   for i in 0 .. 1 loop
      New_Worker := new Worker(i);
   end loop;

end company;
