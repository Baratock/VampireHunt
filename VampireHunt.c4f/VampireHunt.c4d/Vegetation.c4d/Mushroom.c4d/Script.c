#strict 2

#include ELRT

///---Callbacks---///

private func ReproductionAreaSize() { return 60; }
private func ReproductionRate()     { return 13; }
private func MaxTreeCount()         { return 4;  }

public func IsAlchemContainer() { return true; }
public func AlchemProcessTime() { return 80; }

///---Qualities---///

//set a random mushroom colour
public func SetColour(dwColour)
{
	var dwRGB;
	var hue,sat,lightness,alpha;
	dwRGB = RGB2HSL(dwColour); 
	SplitRGBaValue(dwRGB,hue,sat,lightness,alpha);
	hue = hue+Random(20)-10;
	sat = BoundBy(sat+Random(30)-15,127,255);
	lightness = BoundBy(lightness+Random(20)-10,64,227);
	dwRGB = HSL(hue,sat,lightness);

	SetColorDw(dwRGB,this);
}

private func InitObject()
{
	var pShrm = inherited();
	if(pShrm)
	{
		pShrm->SetColour(GetColorDw(this));
	}
	return pShrm;
}

protected func Eat(object pClonk)
{
	pClonk->~Feed(2);
	RemoveObject(this);
	return 1;
}

