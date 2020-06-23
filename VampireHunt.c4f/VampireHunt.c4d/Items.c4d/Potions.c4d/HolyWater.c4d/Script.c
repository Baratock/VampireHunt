/* Holywater */

#strict 2

//-------------!System---------------------

///---Callbacks---///

public func IsPoision() { return true; }

///---Qualities---///

protected func Hit()
{ 
    Sound("Crystal*");
}

//-------------!Holywater---------------------

///---Callbacks---///

public func Color()     { return(RGBa(230,230,230)); }

///---Qualities---///

protected func Initialize()
{
	SetClrModulation(RGBa(200,200,200,90));
}

protected func Activate(clonk)
{
	// Sound
	Sound("Drink");
	// unhealty for undeads
	if(clonk ->~ Who() != Role_Human)
	{
		DoEnergy(-6,clonk,false,FX_Call_EngFire);
		clonk -> Pain(30,true);
	}
	// but healty for humans!
	if(clonk ->~ Who() == Role_Human)
	{
		clonk -> DoEnergy(25);
		clonk -> DoMagicEnergy(5);
	}
	//Action
	clonk->~Drink(this);
	RemoveObject(this);
	return 1;
}


//arrow-effect

public func Affect(object pTarget)
{
	//extrapain for Vampires
	if(pTarget->~ Who() != Role_Human) pTarget ->~ Pain(RandomX(4,16),true);
}
