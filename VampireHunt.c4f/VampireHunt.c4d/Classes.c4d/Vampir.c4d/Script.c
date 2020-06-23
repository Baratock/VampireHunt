/*-- Vampir --*/

#strict 2

#include J_VG

public func MaxLevel()		{ return 1;			}
public func IsJob()       { return false; }
public func Health()      { return [180]; }
public func Pain()        { return [190]; }
public func ManaType()    { return 666;   }
public func MaxMana()     { return [200];   }
public func Requirements(){ return BED1;  }
public func Things()	  { return [TBLD,BOW1,ARWP]; }
public func Knowledge()	  { return [BARP,ARWP,TBLD]; }

public func Support(object pClonk)
{
	Init(pClonk);
	pClonk -> Sound("euer tod4");
	SetGraphics(0,pClonk,VAMP);
	PlayerMessage (GetOwner(pClonk),"$Vamp$");
}

//Blutmengenberechnung
public func Magic(object pClonk)
{
	var change = (pClonk -> Blood()) - (pClonk -> GetMagicEnergy());
	DoMagicEnergy(change,pClonk);
	return 1;
}
