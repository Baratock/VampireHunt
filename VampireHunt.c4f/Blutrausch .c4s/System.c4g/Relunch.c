#strict 2

#appendto SELE

private func Relunch()
{
	var pClonk = Contents(0, this);
	Exit(pClonk);
	//rechts
	if (GetPlayerTeam(GetOwner(pClonk)) == 2)
	{
		Enter(chooserr, pClonk);
		chooserr->Vampire();
	}
	//links
	if (GetPlayerTeam(GetOwner(pClonk)) == 1)
	{
		Enter(chooserl, pClonk);
		chooserl->Vampire();
	}
	CastParticles("NoGravSpark", 50, 30, 0, 0, 30, 60, GetColorDw(pClonk), GetColorDw(pClonk));
	RemoveObject(this);
}