/*-- Wächter --*/

#strict 2

#include J_VG

public func OnlyOne()	  { return true;}
public func MaxLevel()    { return 4;}
public func Health()      { return [120,130,135,140];}
public func Pain()	      { return [140,146,152,160];} 
public func ManaType()    { return Magic_Type_Will;  }
public func MaxMana()     { return [20,20,20,20]; }
public func Requirements(){ return CST2;}
public func Things()	  { return [BOW1,FARP,FARP,SWOR,_BMR];}
public func Knowledge()	  { return [ARWP,STAK,FARP];}

public func Support(object pClonk)
{
	Init(pClonk);
	pClonk -> Sound("vernichten wir");
	SetGraphics(0,pClonk,WTER);
	PlayerMessage (GetOwner(pClonk),"$Guard$");
}
