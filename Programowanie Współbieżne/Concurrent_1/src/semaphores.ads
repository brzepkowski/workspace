package Semaphores is
   protected type Semaphore (Initial : Natural := 0) is
      entry Wait;              -- P operation
      procedure Signal;        -- V operation
   private
      Value : Natural := Initial;
   end Semaphore;
end Semaphores;