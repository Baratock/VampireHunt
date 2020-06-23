/*-- Jäger --*/
 
#strict 2

#include J_VG

public func OnlyOne()	  { return true;}
public func MaxLevel()    { return 4;}
public func Health()      { return [120,130,136,142];}
public func Pain()	      { return [115,120,125,135];}
public func MaxMana()     { return [20,23,27,31]; }
public func ManaType()    { return Magic_Type_Will;  }
public func Requirements(){ return HUT2;}
public func Things()	  { return [BOW1,ARWP,ARWP,DEHK,SPER, POIS,WEQ7];}
public func Knowledge()	  { return [];}
public func CanUseDash()  {return true;}

public func Support(object pClonk)
{
	Init(pClonk);
	SetGraphics(0,pClonk,HUNT);
}
