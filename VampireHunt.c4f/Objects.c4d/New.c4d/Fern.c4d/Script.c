/*-- Farn --*/

// Script v1.0 by Miniwipf
#strict

////////////////////////////// Script //////////////////////////////

protected func Initialize()
{
  // Standartaktion
  SetAction("Remain");
  SetPhase(Random(GetActMapVal("Length", "Remain", GetID())));
  return();
}

/*-- Status --*/

public func IsDecoration() { return(1); }
