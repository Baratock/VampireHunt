/*-- Speer --*/

#strict 2
#appendto SPER

public func HitEffect(object pClonk)
{
	if (Abs(GetXDir(this)) > 30)
	{
		Sound("Splat*");
	}
	return 1;
}



