/*-- Man darf seine Rolle nicht ver�ndern --*/

#strict 2

protected func Initialize()
{
	//Mit der Regelschleife weiter machen
	FindObject2(Find_ID(VHRG))->Vampire();
}

protected func Activate(iByPlayer)
{
	MessageWindow(GetDesc(this), iByPlayer);
	return 1;
}

