/*-- Groﬂer Stein --*/

#strict

protected func Initialize()
{
	SetAction("Rock");
	var phase;
	SetPhase(phase = Random(6));
	SetSolidMask(phase * 32, 0, 32, 32);
	return true;
}

protected func Damage()
{
	if (GetDamage() < 50)
		return ;
	CastObjects(ROCK, 4, 15, 0, -5);
	return RemoveObject();
}
