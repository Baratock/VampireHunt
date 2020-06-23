/*-- Boomerang --*/

#strict 2
#appendto _BMR


///---Qualities---///

public func HitEffect(object pClonk)
{
	if(Abs(GetXDir(this)) > 15)
	{
		DoEnergy(-15,pClonk);
		pClonk->Pain(27+Random(4));  
		Sound("splat*");
	}
}



