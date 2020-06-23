#strict 2

///---Qualities---///

protected func Construction()
{			
	SetAction("Exist");
	//random graphic
	SetDir(Random(4));
}

public func Place(object pTree, int iGrowth)
{	
	SetCategory(C4D_StaticBack);
	SetActionTargets(pTree);
	// size
	if(!iGrowth)
	{ 
		iGrowth = Random(100)+1;
	}
	SetCon(iGrowth);
}

public func Drop()
{			
	// wenn nicht Vollständig: nö, löschen
	if(GetCon(this) < 100)
	{
		RemoveObject(this); 
		return 1;
	}
	SetActionTargets();
	SetCategory(C4D_Object);
}

protected func RejectEntrance()
{		
	if(GetCon(this) < 100)
	{
		return 1;
	}
}

protected func Entrance()
{			
	if(GetCategory(this) & C4D_Object)
	{ 
		Sound("Grab");
	}
	Drop();
}


public func Activate(object pClonk)
{
	[$TxtEat$]
	Eat(pClonk);
	return 1;
}

protected func Eat(object pClonk)
{
    pClonk->~Feed(5);
    pClonk->DoBreath(100,this);
    RemoveObject(this);
    return 1;
}

protected func Hit()
{
	Sound("WoodHit*");
}