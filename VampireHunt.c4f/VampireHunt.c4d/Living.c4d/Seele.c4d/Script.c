#strict 2

local iBlock;

protected func Initialize()
{
	SetAction("Fly");
	SetEntrance(false);
	AddEffect("AutoRelunch",this,1,1000,this);
	return 1;
}

private func FxAutoRelunchTimer(object pTarget)
{
	if(GetWealth(GetOwner(Contents())))
		return -1;
	Message("Ich muss zurückkehren zu meinen Getreuen!",pTarget);
	Schedule("Relunch()",100);
	return -1;
}

///---Movement---///

private func CheckDir(int iComDir)
{
	if(GetComDir(this) != COMD_None)
	{
		SetYDir(0,this);
		SetXDir(0,this);
		return SetComDir(COMD_None);
	}
	else return SetComDir(iComDir,this);
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

///---Rejoin---///

private func Msg(bool f,int first)
{
	var Msgs = ["$Unholy$","$Attempt$","$nbdy$","$nbdy$","$nbdy$","$nbdy$"];
	var pClonk = Contents(0,this);
	if(f)
		PlayerMessage(GetOwner(pClonk),Msgs[iBlock-1]);
	else
	{
		Msgs = ["$Dazed$","$Force$"];
		PlayerMessage(GetOwner(pClonk),Msgs[first]);		
	}
}

public func BlockRelaunch(int iNum)
{
	return iBlock = iNum;
}

protected func ContainedDigDouble()
{
	[Rejoin]
	
	if(iBlock == 1)
	{
		Msg(true);
		return 1;
	}

	if(iBlock == 2)
	{
		Msg(true);
		return 1;
	}
	if(iBlock >= 3)
	{
		Msg(true);
		return 1;
	}

	//Kurze Pause
	if(GetActTime() < 700)
	{
		Msg(false,0);
		return 1;
	}
	Relunch();
	return 1;
}

private func Relunch()
{
	var pClonk = Contents(0,this);
	Exit(pClonk);
	Enter(FindObject(VHRG),pClonk);
	FindObject(VHRG)->Vampire();
	CastParticles ("NoGravSpark", 50, 30,0,0, 30, 60, GetColorDw(pClonk), GetColorDw(pClonk)); 
	RemoveObject(this);	
}
