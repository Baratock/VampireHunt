/*-- Wasserfall --*/

#strict 2

local amount;
local drain;
local iMaterial;
local fNoFountain;
protected func Initialize()
{
	SetAction("Drain");
	amount = 2;
	iMaterial = 1;
	return 1;
}

private func Draining()
{
	if (GBackSolid())
		SetPosition(GetX(), GetY() - 1);
	var i;
	var mat;
	// Je nach Index Material setzen
	if (iMaterial == 1)
		mat = Material("Water");
	else
		if (iMaterial == 2)
			mat = Material("Acid");
		else
			if (iMaterial == 3)
				mat = Material("DuroLava");
			else
				if (iMaterial == 4)
					mat = Material("Oil");
				else
					mat = GameCall("GetWaterFallMaterial", iMaterial);
	if (!drain)
	{
		i = amount;
		if (!fNoFountain)
			InsertMaterial(mat, 0, -1, RandomX(-6, 6), -40);
		while (i--)
			InsertMaterial(mat);
		return;
	}
	i = drain->ExtractMaterialAmount(0, 0, drain->GetMaterial(), amount);
	while (i--)
		InsertMaterial(mat);
	return 1;
}
