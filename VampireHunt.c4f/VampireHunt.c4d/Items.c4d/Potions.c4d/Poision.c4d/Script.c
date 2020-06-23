/* bottle with poision */

#strict 2
#include PHWT

public func Color(){ return RGBa(100,255,100); }

protected func Initialize(){}

protected func Activate(object pClonk)
{
	// Sound
	Sound("Drink");
	// unhealty for humans
	if(pClonk->~Who() == Role_Human)
	{
		DoEnergy(-20,pClonk);
		pClonk->~Pain(15);
	}
	//easteregg: very healthy for undeads :>
	else
	{
		DoEnergy(30,pClonk);
		pClonk->RestorePain(20);
		pClonk->Feed(10,true);
	}
	//Action
	pClonk->~Drink(this);
	RemoveObject(this);
}

public func Affect(object pTarget,object pPoision)
{
	if(pPoision->~IsArrow() || pPoision->~IsBolt())
		AddEffect("PoisionFern",pTarget,1,37*4,0,GetID(this));
	else
		AddEffect("PoisionNah",pTarget,1,37*7,0,GetID(this));
}
protected func FxPoisionNahTimer() { return -1; }
protected func FxPoisionFernTimer() { return -1; }

