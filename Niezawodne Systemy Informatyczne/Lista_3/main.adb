with Ada.Text_IO; use Ada.Text_IO;
with Railway; use Railway;

procedure Main is
   OK : Boolean;
begin
   Open_Route (Route_Enter_Left,   OK); Put_Line(Boolean'Image(OK));
   Move_Train (Route_Enter_Left,   OK); Put_Line(Boolean'Image(OK));
   Open_Route (Route_Left_Middle,  OK); Put_Line(Boolean'Image(OK));
   Move_Train (Route_Left_Middle,  OK); Put_Line(Boolean'Image(OK));
   Open_Route (Route_Middle_Right, OK); Put_Line(Boolean'Image(OK));
   Move_Train (Route_Middle_Right, OK); Put_Line(Boolean'Image(OK));
   Open_Route (Route_Leave_Right,  OK); Put_Line(Boolean'Image(OK));
   Move_Train (Route_Leave_Right,  OK); Put_Line(Boolean'Image(OK));
end Main;
