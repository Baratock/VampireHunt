/*-- Vampirjagd, allgemeiner Szenscript (übertragbar) --*/

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
