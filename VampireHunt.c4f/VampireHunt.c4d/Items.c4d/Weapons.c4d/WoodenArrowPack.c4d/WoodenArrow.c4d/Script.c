/*-- Brandpfeil --*/

#strict 2
#include ARRW

///---Callbacks---///

public func PackTo() { return(WRP2); }
public func GetCalcedPain(){ return 6;}
public func ArrowStrength() { return 4; }

///---Qualities---///

public func HitEffect(object pTarget) //Direkte Wirkung beim Treffer
{
	if(GetAction(this) != "Shot")
	{ 
		return 0;
	}
	DoEnergy(-ArrowStrength(),pTarget);
	pTarget->~Pain(GetCalcedPain());
	if(GetID(pTarget) == WIPF)
	{ 
		pTarget -> Kill();
	}
	UsePoision(this,poision,pTarget);
	RemoveObject(this);
}

