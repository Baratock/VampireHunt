/*-- Debugfunktionen um das Finden von fehlerm auch ingame Leichter zu machen --*/

#strict 2

//Funktion die den Vampir sucht und zurückgibt
//Gibt wenn vorhanden den Vampir zurück;
//Wenn fPlr true ist wird nicht der Vampir-Clonk sondern die Vampir-Spielernummer zurückgegeben
//Wenn kein Vampir vorhanden ist gibt die Funktion -1 aus
global func FindVampire(bool fPlr)
{
	var a = FindObjects(Find_ID(MEDC));
	for(var p in a)
	{
		if(LocalN("vampire",p) == Role_Vampire)
		{
			if(fPlr)
				return GetOwner(p);
			else
				return p;
		}
	}
	return -1;
}

//Hmm mal sehen was uns bei Betatests noch so aufn Sack geht...
