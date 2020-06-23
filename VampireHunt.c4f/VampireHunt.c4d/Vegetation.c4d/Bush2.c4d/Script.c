/*-- Busch --*/

#strict 2

#include TREE

///---Callbacks---///

private func ReproductionAreaSize() { return 400; }
private func ReproductionRate()     { return 500; }
private func MaxTreeCount()         { return 15;  }
private func TreeStrength()  { return 40;  }
private func FruitRate()     { return 15;  }
private func FruitID()       { return WMPF;}
private func MaxFruitCount() { return 4;   }

///---Qualities---///

// all 350 Frames
private func Seed()		
{
	if( IsStanding() && GetCon(this) == 100 && !Random(FruitRate()) )
	{ 
		Fruit();
	}
	return _inherited();
}

public func FindFruits()
{
	var w = GetDefWidth(GetID(this));
	var h = GetDefHeight(GetID(this));
	return FindObjects(Find_ID(FruitID()), Find_InRect(-w,-h,w*2,h*2));
} 

public func ChopDown()		
{
	for(var pFruit in FindFruits())
	{
		pFruit->~Drop();
	}
	return _inherited();
}

public func Incineration()	
{
	for(var pFruit in FindFruits())
	{
		RemoveObject(pFruit);
	}
	ChangeDef(BUSB);
}

/* function places fruits */

private func FruitRetangle(&iWidth, &iHeight, &iOffset)
{
	var iFireTop = GetDefFireTop(GetID(this));
	iWidth  =  GetDefWidth (GetID(this))*GetCon(this)/100;
	iHeight =  GetDefHeight(GetID(this))*GetCon(this)/100;
	iHeight -= iFireTop;

	// Frucht soll nicht "überhängen"
	iWidth  -= iWidth/10;
	iHeight -= iHeight/10;

	var iOffset = -iFireTop/2;
}

public func Fruit()
{
	if(GetLength(FindFruits()) < MaxFruitCount())
	{
		// Zielrechteck in dem die Früchte erstellt werden sollen
		var iWidth, iHeight, iOffset;
		FruitRetangle(iWidth, iHeight, iOffset);

		var iAngle = Random(360);
		// im Kreis: wirkt besser
		PlaceFruit(FruitID(),Cos(iAngle,RandomX(-iWidth/2,iWidth/2)),Sin(iAngle,RandomX(-iHeight/2,iHeight/2)),10);
		return 1;
	}
	// No space ;'(
	return 0;
}

private func PlaceFruit(id idFruit, int iX, int iY, int iGrowth)
{
	var pFruit = CreateObject(idFruit,iX,iY,NO_OWNER);
	pFruit->~Place(this,iGrowth);
}