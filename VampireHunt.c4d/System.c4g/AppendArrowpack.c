#strict 2

#appendto ARWP

local poision; //ID des aufgetragenen Giftes...

public func Unpack(object pTo)
{
	if (!pTo)
	{
		pTo = Contained(this);
	}
	// Effekt zum Deaktivieren des Pfeilpackens
	AddEffect("IntNoPacking", pTo, 1);
	// Pfeile erzeugen
	for (var i = 0; i < PackCount(); i++)
		CreateContents(UnpackTo(), pTo)->Impregnate(poision);
	// Effekt wieder entfernen
	RemoveEffect("IntNoPacking", pTo);
	// Fertig
	RemoveObject(this);
	return 1;
}

public func GetItem()
{
	// Effekt zum Deaktivieren des Pfeilpackens
	AddEffect("IntNoPacking", Contained(), 1);
	// Pfeil ausgeben
	var pObj = CreateContents(UnpackTo(), Contained());
	pObj->Impregnate(poision);
	// Effekt wieder entfernen
	RemoveEffect("IntNoPacking", Contained());
	// Einer weniger
	DoPackCount(-1);
	return pObj;
}

public func JoinPack(object pClonk)
{
	// In vorhandene Pfeilpakete integrieren
	var obj, iChange, iAmount;
	// Alle gleichen Pakete im Clonk durchsuchen
	for (pObj in FindObjects(Find_Exclude(this()), Find_Container(pClonk), Find_ID(GetID(this)))) 
	{
		//Nur bei gleichem Gift
		if (pObj->~CheckPoision() != CheckPoision())
			return 0;
		iAmount = PackCount();
		// Wie viel Platz ist im neuen Paket?
		iChange = MaxPackCount() - pObj->PackCount();
		// Wir können aber höchstens so viele abgeben wie wir haben
		if (iChange > iAmount)
			iChange = iAmount;
		// Pfeile abgeben
		pObj->DoPackCount(iChange);
		// Wenn das Paket leer ist aufhören
		if (iAmount - iChange <= 0)
			return RemoveObject();
		DoPackCount(-iChange);
	}
	return 0;
}

public func IsPoisionCarrier()
{
	if (!poision)
		return true;
}

public func CheckPoision()
{
	return poision;
}

public func Impregnate(id IDdef)
{
	if (!IDdef || !IDdef->IsPoision())
	{
		return 0;
	}
	var dwClr = IDdef->~Color();
	SetClrModulation(dwClr);
	var name = Format("%s-%s", GetName(0, IDdef), GetName(this));
	SetName(name);
	poision = IDdef;
}
