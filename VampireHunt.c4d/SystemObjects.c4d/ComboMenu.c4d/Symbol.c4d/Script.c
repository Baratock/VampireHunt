/* Symbol */

#strict 2

local idThisSpell, pMagician;

protected func AttachTargetLost()
{
	return RemoveObject(this);
}

public func SetSymbolGfx(string szAction, id idSpell, object pTarget, object pMagi)
{
	// Standardaktivität: Symbol
	if (!szAction)
		szAction = "Symbol";
	// Grafik und Namen setzen
	if (idThisSpell = idSpell)
	{
		SetGraphics(0, this, idSpell, GFX_Overlay, GFXOV_MODE_IngamePicture);
		SetClrModulation(16777215, 0, GFX_Overlay);
		SetObjectBlitMode(0, 0, GFX_Overlay);
		SetName(GetName(0, idSpell));
		// Anwählbar
		SetCategory(GetCategory() | C4D_MouseSelect);
		// Zauberer sichern fürs Symbolupdate
		pMagician = pMagi;
	}
	// Aktivität setzen
	if (!SetAction(szAction, pTarget))
		return AttachTargetLost();
	// Auswahlgröße: Immer 35x35
	SetShape(-18, -18, 35, 35);
	// Fertig
	return 1;
}

public func GetSpellID()
{
	return idThisSpell;
}

public func GetControlChar()
{
	if (GetAction() == "Throw")
		return "4";
	if (GetAction() == "Up")
		return "5";
	if (GetAction() == "Dig")
		return "6";
	if (GetAction() == "Left")
		return "L";
	if (GetAction() == "Down")
		return "2";
	if (GetAction() == "Right")
		return "R";
}

protected func MouseSelection(iByPlr)
{
	return GetActionTarget()->MouseSelection(this);
}
