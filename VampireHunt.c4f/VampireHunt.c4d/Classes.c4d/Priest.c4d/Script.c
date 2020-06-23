/*-- Priester --*/

#strict 2

#include J_VG

public func OnlyOne()	  {	return true; }
public func MaxLevel()    { return 4;}
public func Health()      { return [105,108,110,112];}
public func Pain()	      { return [95,100,103,106];} 
public func ManaType()    { return Magic_Type_Holyforce;  }
public func MaxMana()     { return [70,70,70,70]; }
public func Requirements(){ return CHUR;}
public func Things()	  { return [STAK,BAYO,BAYO];}
public func Knowledge()	  { return [STAK,PBAL,PELF,PR_B,PR_W,PR_G,PRSP,URPT,UROI];}

public func Support(object pClonk)
{
	Init(pClonk);
	pClonk -> Sound("mein glaube");
	SetGraphics(0,pClonk,PRIS);
	PlayerMessage (GetOwner(pClonk),"$Priest$");
}

//Regeneration:Heilige/Unheilige Orte
public func Magic(clonk)
{
	if(clonk ->~ Who() == 0 && Contained(clonk)) if(Contained(clonk) ->~ IsHolyPlace() > 0) DoMagicEnergy(1,clonk);
	if(clonk ->~ Who() == 1 && Contained(clonk)) if(Contained(clonk) ->~ IsHolyPlace() < 0) DoMagicEnergy(1,clonk);
	return(1);
}
