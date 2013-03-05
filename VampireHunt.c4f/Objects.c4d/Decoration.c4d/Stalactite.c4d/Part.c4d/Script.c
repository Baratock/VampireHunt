/*-- Bruchteil eines Stalaktiten --*/

#strict

local alpha;

/* Langsam verschwinden */
public func FadeMeOut()
{
	SetClrModulation(RGBa(255, 255, 255, alpha++));
	if (alpha == 255)
		return RemoveObject();
	return 1;
}

public func Hit()
{
	return Sound("RockHit*");
}
