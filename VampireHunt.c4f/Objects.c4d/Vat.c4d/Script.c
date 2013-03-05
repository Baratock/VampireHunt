/*-- WeinFass --*/

#strict

public func RejectCollect(ID, object pObj)
{
	if (ObjectDistance(this(), pObj) > 37 * GetCon() / 100)
		return 1;
	if (ID != AXT_)
		return 1;
	if (Abs(GetXDir(pObj)) < 10)
		return 1;
	var angle = Angle(GetX(), GetY(), GetX(pObj), GetY(pObj));
	//Message("%d, %d, %d",this(),angle, GetR(pObj),GetR(pObj)-angle-90);
	if (Abs(GetR(pObj) - angle + 90) < 360)
		if (Abs(GetR(pObj) - angle + 90) > 30)
			return 1;
	if (Abs(GetR(pObj) - angle + 90) > 360)
		if (Abs(GetR(pObj) - angle + 90) - 360 > 30)
			return 1;
	SetCategory(3601, pObj);
	pObj->SetAction("Idle");
	Sound("Chop*");
	SetSpeed(0, 0, pObj);
	Leck(AbsX(GetX(pObj)), AbsY(GetY(pObj)));
	return 1;
}

public func Leck(int iX, int iY)
{
	Schedule(Format("CreateParticle(\"PxSpark\",%d,%d,%d+RandomX(-10,10),%d+RandomX(-10,10),30,RGB(169,39,85))", iX, iY, iX, iY), 1, 60);
	return 1;
}
