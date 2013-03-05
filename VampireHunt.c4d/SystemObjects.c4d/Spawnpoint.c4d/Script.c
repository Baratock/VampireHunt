/*-- Spawnpoint, nicht "Aldiversion" sondern spartanisch,rustikal! --*/

#strict 2

local id;

public func Timer()
{
	if (!id)
	{
		return 0;
	}
	else
	{
		SetVisibility(VIS_God, this);
	}
	if (!FindObject2(Find_ID(id), Find_InRect(-200, -200, 400, 400)))
	{
		CreateObject(id, 0, 0, NO_OWNER);
	}
}
