#strict 2

///---Callbacks---///

public func IsPotion(){	return true; }

///---Qualities---///

protected func Hit()
{
	Sound("RockHit*");
}

protected func Activate(object pClonk)
{
	Sound("Drink");
	pClonk->~Drink(this);
	DoAlcohol(pClonk,1000);
	if(pClonk->Who() == Role_Human)
	{
		pClonk->~Hunger(-40);
		// 1/4 less pain
		pClonk->~RestorePain(pClonk->~RestorePain()/4);
		// a bit tired 
		pClonk->~RestoreSleep(-10);
	}
	else
	{
		//if vamp -> don't like human food
		pClonk->~Blood(-30);
		//5/4 more pain
		pClonk->~RestorePain(pClonk->~RestorePain()/4+pClonk->~RestorePain());
	}
	RemoveObject(this);
	return 1;
}
