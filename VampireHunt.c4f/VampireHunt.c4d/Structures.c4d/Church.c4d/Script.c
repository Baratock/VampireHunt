/*-- Church --*/

#strict 2

#include BAS8
#include DOOR

///---Callbacks---///

public func IsHolyPlace() { return true; }

///---Qualities---///

//doorcontrol
private func SoundOpenDoor()
{
	Sound("GateOpen");
}
  
private func SoundCloseDoor()
{
	Sound("GateClose");
}

public func CanEntrance(bool freturnVal,bool fEntrance)
{
	static fFirst;
	static fEnter;
	if(!fFirst)
	{
		fFirst = true;
		fEnter = true;
	}
	if(freturnVal)
		return fEnter;
	else
		fEnter = fEntrance;
}

protected func ActivateEntrance(object pClonk)
{
	if (!CanEntrance(true))
		return 0;
	return inherited(pClonk);
}

