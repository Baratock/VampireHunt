/*-- Beistand --*/

#strict 2

local user;
local phase;

//Wird als Gebet erkannt
public func IsPrayer() { return true; }

//Voraussetzungen
public func Requirements(caller)
{
	//Nur Menschen + im Freien + laufen + energy
	if(caller ->~ Who() != Role_Human || Contained(caller) || 
		GetAction(caller) != "Walk" || GetMagicEnergy(caller) < 20)
	{ 
		return false;
	}
	return true;
}

public func Apply()
{
	if(Contained(this))
	{
		user = Contained(this);
		user -> Exit(this);
	}
	SetComDir(COMD_None,user);
	SetXDir(0,user);
	user -> SetAction("Pray");
}

private func Phase()
{
	if(!phase)
		Message("In nomino dominus",user);
	 if(phase == 1)
	 	Message("Is res sacer redeo",user);
	 if(phase == 2)
	 	Message("       AMEN       ",user);
	user->DoMagicEnergy(-5);
	phase++;	 
}

//Ablauf
public func Check()
{
	//Wenn er stirbt oder getroffen wird
	if(!user || GetAction(user) != "Pray") 
		return RemoveObject(this);

	SetPosition(GetX(user),GetY(user));

	if(phase == 2)
	{
		user->DoMagicEnergy(-5);
		user->SetAction("KneelUp");
		CreateContents(PHWT,user);
		CreateContents(PHWT,user);
		Phase();
		return RemoveObject(this);
	}
	Phase();
}
