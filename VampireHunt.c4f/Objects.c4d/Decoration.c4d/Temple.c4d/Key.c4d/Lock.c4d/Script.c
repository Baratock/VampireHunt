/* Schloss */

#strict 2

local iRefID;
local pTarget;

protected func Initialize()
{
	SetAction("None");
}

/* Steuerung */

public func ControlDig()
{
	[$MsgTurnKey$]
	// Steckt ein Schl�ssel?
	if (!Contents())
	{
		Message("$MsgNoKey$", this);
		Sound("Error");
		return 1;
	}
	// Passt der Schl�ssel?
	if (LocalN("iRefID", Contents()) != iRefID)
	{
		Message("$MsgWrongKey$", this);
		Sound("Error");
		return 1;
	}
	if (!pTarget)
		return Message("$MsgStuck$", this);
	// Schl�ssel drehen
	SetDir(1 - GetDir());
	// Angeschlossenes Objekt aktivieren
	pTarget->~Activate(this);
	return 1;
}
  
public func ControlThrow(object pClonk)
{
	[Schl�ssel reinstecken/rausnehmen]
	if (Contents())
	{
		Exit(Contents());
		SetDir(0);
		SetAction("None");
		return 1;
	}
	// Kann der Inhalt des Clonks angenommen werden?
	var obj;
	if (obj = Contents(0, pClonk))
		if (obj->~IsKey())
			if (LocalN("iRefID", obj))
			{
				if (!RejectCollect(GetID(obj), obj))
					Enter(this, obj);
				if (!GetClrModulation(obj))
					SetColorDw(RGB(255, 255, 255));
				else
					SetColorDw(GetClrModulation(obj));
				return 1;
			}
	return 1;
}

/* Collection */

protected func RejectCollect(idObj, pObj)
{
	// Nur Schl�ssel aufnehmen
	if (!pObj->~IsKey())
	{
		Message("$MsgNoKey$", this);
		Sound("Error");
		return 1;
	}
	// Nur, wenn sie einigerma�en passen
	if (LocalN("iRefID", pObj) != iRefID)
	{
		Message("$MsgWrongKey$", this);
		Sound("Error");
		return 1;
	}
	// Schl�ssel passt
	SetAction("Key");
	return 0;
}

public func IsSwitch()
{
	return true;
}