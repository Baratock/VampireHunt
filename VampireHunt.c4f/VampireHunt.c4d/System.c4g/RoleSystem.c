/*-- Neues Objekt --*/

#strict 2

static const Role_Human = 0; //Mensch
static const Role_Vampire = 1; //Vampir
static const Role_Slave = 2; //Diener des Vampirs
static const Role_Lich = 3;

static const Magic_Type_Will = 0;
static const Magic_Type_Holyforce = 1;
static const Magic_Type_Mana = 2;

static aRoles; //Rollen für die einzelnen Spieler

global func SetRole(int iPlr, int iRole)
{
	aRoles[GetPlayerID(iPlr)] = iRole;
	return true;
}

global func GetRole(int iPlr)
{
	return aRoles[GetPlayerID(iPlr)];
}

global func LivingRoleCount(int iRole)
{
	var iCount = 0;
	for(var i = 0;i < GetPlayerCount();i++)
	{
	if(GetRole(GetPlayerByIndex(i)) == iRole && GetID(Contained(GetCursor(GetPlayerByIndex(i)))) != SELE)
		iCount++;
	}
	return iCount;
}
