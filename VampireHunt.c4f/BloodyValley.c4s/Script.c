/*-- Vampirjagd, allgemeiner Szenscript (�bertragbar) --*/

#strict 2

protected func Initialize()
{
	InitScenario();
	return true;
}

protected func RelaunchPlayer(int iPlr, object pCrew)
{
	Relaunch(iPlr, pCrew);
}
