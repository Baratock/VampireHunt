//Vampirjäger
#strict 2

#include J_VG

public func OnlyOne()	  { return true;}
public func MaxLevel()    { return 4;}
public func Health()      { return [120,125,127,130];}
public func Pain()	      { return [125,130,136,143];} 
public func ManaType()    { return Magic_Type_Will;  }
public func Requirements(){ return HUT2;}
public func Things()	  { return [CRBW,SBRE,BOTP, BOTP];}
public func Knowledge()	  { return [BOTP];}

public func CanUse(id IDdef)
{
	for(var i = 0; i < GetLength(Things()); i++)
		if(IDdef == Things()[i])
			return true;
	return false;
}

public func Support(object pClonk)
{
	Init(pClonk);
	SetGraphics(0,pClonk,VHHU);
}
