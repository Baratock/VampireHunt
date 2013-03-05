/*-- Diener erschaffen --*/

#strict 2

public func IsAbility()
{
	return true;
}

public func GetSpellCombo()
{
	return "242";
}

public func CanUse(object pCaller)
{
	if (pCaller->CanCreateSlaves())
		return 1;
}

public func Activate(object pCaller)
{
	CreateMenu(VAMP, pCaller, pCaller, 0, "$VHMakeSlave$", 0, 1);
	AddMenuItem("$VHMakeGhul$", "MakeSlave(J_GH)", VAMP, pCaller);
	AddMenuItem("$VHMakeLich$", "MakeSlave(J_LI)", LICH, pCaller);
	return true;
}