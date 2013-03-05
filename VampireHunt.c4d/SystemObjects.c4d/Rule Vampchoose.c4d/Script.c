/*-- Wählt den Vamp :D --*/

#strict 2

local aPlayer;
local iHelper;

protected func Initialize()
{
	iHelper = -1;
}

protected func Activate(int iByPlayer)
{
	MessageWindow(GetDesc(this), iByPlayer);
	return 1;
}

//Timercall
protected func Check()
{
	var iCheck = 0;
	CheckProgress(iCheck, iHelper);
	//wenn iCheck 0 ist, dann kann weiter gemacht werden
	if (!iCheck)
		//weiter machen!
		GetInfo();
}

//Schleife für die verschiedenen Stufen
public func GetInfo()
{
	iHelper++;
	if (iHelper == 0)
	{
		aPlayer = [];
		SelectVamp(iHelper);
		return 1;
	}
	if (iHelper == 1)
	{
		SelectVamp(iHelper);
		aPlayer = [];
		return 1;
	}
	//----------wahlgewinner finden-----------
	
	var aData = [];
	//schauen welcher Player am meisten auftaucht
	for (var i = 0; i < GetLength(aPlayer); i += 2)
	{
		//keine Stimmen überspringen
		if (aPlayer[i] == -1)
			continue;
		aData[aPlayer[i]]++;
	}
	//wenn kein eindeutiges Ergebnis, entscheidet Zufall
	var iPlayer = Random(GetPlayerCount());
	//das Größte finden
	for (var i = 0, iEnd = 0; i < GetLength(aData); i++)
	{
		if (aData[i] > iEnd)
		{
			iEnd = aData[i];
			iPlayer = i;
		}
	}
	GetCursor(iPlayer)->SetRole(J_VA);
	Exit(GetCursor(iPlayer));
	J_VA->Support(GetCursor(iPlayer));
	//Mit der Regelschleife weiter machen
	FindObject2(Find_ID(VHRG))->Vampire();
}

private func SelectVamp(int f)
{
	for (var i = 0; i < GetPlayerCount(); i++)
	{
		var iPlayer = GetPlayerByIndex(i);
		var pClonk = GetCrew(iPlayer, 0);
		if (f == 0)
		{
			OpenIfVamp(pClonk);
		}
		if (f == 1)
		{
			OpenMenu(pClonk);
		}
	}
}

//Öffnet das Menü ob man Vampir werden möchte
private func OpenIfVamp(object pChooser)
{
	Message($VampA$);
	CreateMenu(MEDC, pChooser);
	AddMenuItem($Y$, Format("Answer(0,%d)", ObjectNumber(pChooser)), GetID(this), pChooser);
	AddMenuItem($N$, Format("Answer(1,%d)", ObjectNumber(pChooser)), GetID(this), pChooser);
	AddEffect("TimerMenu", pChooser, 100, 10, this, 0, Format("Answer(1,%d)", ObjectNumber(pChooser)));
}

//öffnet das Menü wo man wählen kann wer Vampir werden soll
private func OpenMenu(object pChooser)
{
	Message($Vote$);
	CreateMenu(MEDC, pChooser);
	//alle spieler durchgehen
	var i;
	for (i = 0; i < GetPlayerCount(); i++)
	{
		var iPlayer = GetPlayerByIndex(i);
		if (pChooser == GetCrew(iPlayer) || !GetCrew(iPlayer) || FindElement(GetCrew(iPlayer)))
			continue;
		AddMenuItem(Format("%s", GetPlayerName(iPlayer)), Format("Answer(%d,%d)", iPlayer, ObjectNumber(pChooser)), GetID(this), pChooser);
	}
	//Nichtwähler sind ein ernstes Problem... so muss man sich zumindest enthalten
	AddMenuItem("Niemand", Format("Answer(-1,%d)", ObjectNumber(pChooser)), GetID(this), pChooser);
	//denn wer nicht wählt enthält sich!
	AddEffect("TimerMenu", pChooser, 100, 10, this, 0, Format("Answer(-1,%d)", ObjectNumber(pChooser)));
}

private func FindElement(object p)
{
	for (var i; i < GetLength(aPlayer); i++)
		if (aPlayer[i] == p)
			return aPlayer[i - 1];
}

//setzt die Antwort
private func Answer(int iAnswer, int iUser)
{
	aPlayer[GetLength(aPlayer)] = iAnswer;
	aPlayer[GetLength(aPlayer)] = Object(iUser);
	CloseMenu(Object(iUser));
}

//nun sicher prüfen ob jeder Spieler in dem Feld enthalten ist und ein Fortschritt möglich ist
private func CheckProgress(& iCheck, int iPhase)
{
	if (iPhase == -1)
		return 1;
	if (iPhase == 0 || iPhase == 1)
		for (var pPlayer, i = 0; i < GetPlayerCount(); i++)
		{
			iCheck--;
			pPlayer = GetCrew(GetPlayerByIndex(i));
			//ist der Spieler im Feld?
			for (var i2 = 1; i2 < GetLength(aPlayer); i2 += 2)
			{
				if (pPlayer == aPlayer[i2])
				{
					//gut, er ist es
					iCheck++;
					break;
				}
			}
		}
	if (iPhase > 1)
		iCheck++;
}

protected func MenuQueryCancel(int iSelection, object pMenuObj)
{
	return true;
}