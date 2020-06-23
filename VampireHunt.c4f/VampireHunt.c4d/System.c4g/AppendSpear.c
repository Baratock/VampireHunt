/*-- Speer --*/

#strict 2
#appendto SPER


///---Qualities---///

public func HitEffect(object pClonk)
{
	if(Abs(GetXDir(this)) > 30)
	{
		DoEnergy(-12,pClonk);
		pClonk->Pain(26+Random(1));  
		Sound("Splat*");
	}
}



