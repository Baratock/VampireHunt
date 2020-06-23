/*-- Beistand --*/

#strict

//Wird nicht als Gebet erkannt
public func IsPrayer() { return(0); }

//keine Voraussetzungen
public func Requirements(caller)
{
 return(1);
}

local user;
local phase;

func Apply()
{
 if(Contained())
 {
  user=Contained();
  user -> Exit(this());
 }
 else return(RemoveObject());
}

//Ablauf
func Check()
{
 SetPosition(GetX(user),GetY(user));

 if(phase < 3)
 {
  phase++;
  return(1);
 }

//Amen...
return(RemoveObject());
}