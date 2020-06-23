/*-- Grosser Stein --*/

// Script v1.0 by Miniwipf
#strict

////////////////////////////// Script //////////////////////////////

protected func Initialize()
{
  // Standartaktion
  SetAction("Remain");
  var iPhase = Random(GetActMapVal("Length", "Remain", GetID()));
  SetPhase(iPhase);
  SetSolidMask(iPhase*GetDefWidth(), 0, GetDefWidth(), GetDefHeight());
  return();
}

protected func Damage()
{
  if (GetDamage() < 50) return();
  // Zersplittern
  for (var i=0, idDef, iCnt; (idDef = GetDefComponents(, i)) && (iCnt = GetDefComponents(, i+1)); i++)
    CastObjects(idDef, iCnt, 15,, -5);
  return(RemoveObject());
}

/*-- Status --*/

public func IsDecoration() { return(1); }
