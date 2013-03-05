/*-- Kopf --*/

#strict 2

local name; //Wer isn das?
local empty; //voller blut?

protected func Hit()
{
	return Sound("Splat*");
}

protected func Activate(object pClonk)
{
	//Menschen: dummer Spruch
	if (pClonk->GetVal(cForm) == cRoleHuman)
	{
		Message("Ähm.. %s gehts dir gut? Du siehst irgendwie.. ääh krank aus.", pClonk, name);
	}
	else
		if (pClonk->GetRole() == cRoleGhul)
		{
			Sound("Splatter");
			pClonk->Feed(50);
			RemoveObject(this);
		}
		else
			if (!empty)
			{
				pClonk->Feed(30, true);
				empty = 1;
			}
}