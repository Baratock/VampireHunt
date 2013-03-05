/*-- Helfer für die Imprägnierung --*/

#strict 2

global func ImpGetID(object pObject)
{
	var idObj = GetID(pObject);
	if (idObj->~UnpackTo())
		idObj = idObj->UnpackTo();
	var idPoision = 0;
	if (pObject->~CheckPoision())
		idPoision = pObject->~CheckPoision();
	return Format("%i%i", idObj, idPoision);
} 

global func ImpGetImp(string szID)
{
	//Imprägnierungs ID auslesen
	var idPoision = "";
	for (var i = 4; i < 8; i++)
		idPoision = Format("%s%c", idPoision, GetChar(szID, i));
	return eval(idPoision);
}

global func ImpGetObj(string szID)
{
	//Waffen ID auslesen
	var idWeapon = "";
	for (var i = 0; i < 4; i++)
		idWeapon = Format("%s%c", idWeapon, GetChar(szID, i));
	return eval(idWeapon);
}