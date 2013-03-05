/*-- Amulett --*/

#strict 2

local iMana;
local iEnergy;

protected func Initialize()
{
	iMana = 0;
	iEnergy = 0;
	return 1;
}

protected func Check()
{
	var pContained = Contained(this);
	if (pContained == 0)
		return 0;
	if (pContained->~IsHolyPlace())
	{
		if (iMana < 20)
			iMana++;
	}
	//Ist im Alchemist?
	if (pContained->GetRole() == J_AL)
	{
		if (iEnergy < 15)
			iEnergy += Random(4) / 3;
		if (iMana > 0)
		{
			//nicht volles Mana?
			if (pContained->GetMaxVal(cMana) != GetMagicEnergy(pContained))
			{
				DoMagicEnergy(Random(2), pContained);
				iMana--;
			}
		}
		if (iEnergy > 0)
		{
			if (pContained->GetMaxVal(cHealth) != GetEnergy(pContained))
			{
				DoEnergy(2, pContained);
				iEnergy--;
			}
		}
	}
}