/*-- Urticanettle --*/

#strict 2

#include ELRT

///---Qualities---///

public func Construction()
{
    SetAction("Exist");
    SetPosition(GetX(this),GetY(this)-5);
}

protected func Eat(object pClonk)
{
	var x = Local(18,pClonk);
    pClonk->~Feed(10/2**x);
    pClonk->~Pain(20/2**x);
    DoEnergy(25/2**x,pClonk);
    AddEffect("AteNettle",pClonk,1,37*5,0,GetID(this));
    Local(18,pClonk)++;
    RemoveObject(this);
    return 1;
}

protected func FxAteNettleTimer(object pTarget, int iNumber, int iTime) 
{
	Local(18,pTarget)--;
	return -1;
}