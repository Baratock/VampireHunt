/*-- Status Hud --*/

#strict 2

local pOwner;

protected func Initialize()
{
	SetVisibility(VIS_None, this);
	SetPosition(90, 110, this);
	return true;
}

protected func Check()
{
	SetPosition(90, 110, this);
	if (!pOwner || !GetAlive(pOwner))
		RemoveObject(this);
	if (GetCursor(GetOwner(this)) == pOwner)
	{
		SetVisibility(VIS_Owner, this);
		//Farben, Hunger, Schmerz, Müdigkeit, Blut, Cooldown
		var dwClr = [RGB(150, 90, 10), RGB(160, 30, 160), RGB(30, 30, 240), RGB(160, 30, 30), RGB(190, 150, 30)];
		//Verschiebung
		var n = [-27, 16, -6, -48, 36];
		//Allgemein
		for (var imaxVal = 0, iVal = 0, i = 1; i < 6; i++)
		{
			//Wert abfragen
			iVal = pOwner->GetVal(2 ** (i - 1));
			//Maximum speichern
			imaxVal = pOwner->GetMaxVal(2 ** (i - 1));
			//darf nicht größer als das Maximum sein
			if (iVal > imaxVal)
				iVal = imaxVal;
			SetGraphics("Bar", this, GetID(this), i, 2, "Bottle");
			SetClrModulation(dwClr[i - 1], this, i);
			SetGraphics("Blood", this, GetID(this), i + 20, 2, "Bottle");
			SetObjDrawTransform(1000, 0, 1000 * n[i - 1], 0, 1000, -1000 * 4, this, i + 20);
			SetObjDrawTransform(600, 0, 1000 * n[i - 1], 0, 1000 * iVal / imaxVal, 1000 * 27 - 1000 * 33 * iVal / imaxVal, this, i);
		}
	}
	else
		SetVisibility(VIS_None, this);
}

protected func SetClonk(object pClnk)
{
	pOwner = pClnk;
	SetOwner(GetOwner(pOwner), this);
	SetVisibility(VIS_Owner, this);
	Check();
	return true;
}
