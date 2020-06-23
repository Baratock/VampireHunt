/*-- Schlüssel --*/

#strict 2

local iRefID;
/* Initialisierung */

protected func Initialize()
  {
  SetAction("Key1");
  return true;
  }

/* Effekte */

protected func Hit()
  {
  Sound("Crystal*");
  return true;
  }

/* Zuweisung */
public func SetRefID(iID, iClr)
{
  iRefID = iID;
  SetClrModulation(iClr);
}

/* Status */

public func IsKey() { return 1; }
