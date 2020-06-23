#strict 2

#appendto J_FS
#appendto J_HN
#appendto J_VH

//Platzmangel nur eine Wohnung, deswegen zufällig in eine
public func Support(object pClonk)
{
	inherited(pClonk);
	var p = 0;
	for(p in FindObjects(Find_ID(SAWM)))
		if(GetX(p) > LandscapeWidth()/2)
			break;
	var r = [FindObject2(Find_ID(HUT2)),p];
	Enter(r[Random(2)],pClonk);
}