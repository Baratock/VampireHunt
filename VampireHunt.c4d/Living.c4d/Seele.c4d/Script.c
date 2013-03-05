#strict 2

protected func Initialize()
{
	SetAction("Fly");
	SetEntrance(false);
	AddEffect("AutoRelunch", this, 1, 1000, this);
	return 1;
}

private func FxAutoRelunchTimer(object pTarget)
{
	Message("Ich muss zurückkehren zu meinen Getreuen!", pTarget);
	Schedule("ContainedDigDouble()", 100);
	return -1;
}

private func CheckDir(int iComDir)
{
	if (GetComDir(this) != COMD_None)
	{
		SetYDir(0, this);
		SetXDir(0, this);
		return SetComDir(COMD_None);
	}
	else
		return SetComDir(iComDir, this);
}

protected func ContainedRight()
{
	return CheckDir(COMD_Right);
}
protected func ContainedLeft()
{
	return CheckDir(COMD_Left);
}
protected func ContainedUp()
{
	return CheckDir(COMD_Up);
}
protected func ContainedDown()
{
	return CheckDir(COMD_Down);
}

protected func ContainedDigDouble()
{
	[Rejoin]
	//Kurze Pause
	if (GetActTime() < 700)
	{
		PlayerMessage(GetOwner(pClonk), "$Dazed$");
		return 1;
	}
	var pClonk = Contents(0, this);
	Exit(pClonk);
	Enter(FindObject(VHRG), pClonk);
	FindObject(VHRG)->Vampire();
	CastParticles("NoGravSpark", 50, 30, 0, 0, 30, 60, GetColorDw(pClonk), GetColorDw(pClonk));
	RemoveObject(this);
}
