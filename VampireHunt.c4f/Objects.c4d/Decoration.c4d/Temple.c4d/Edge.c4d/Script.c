/*-- Ecke --*/

#strict 2

protected func Initialize()
{
	Snap();
	return 1;
}

public func Snap(int o, int p)
{
	SetPosition((GetX() + 10) / 10 * 10 - 5 + o, (GetY() + 10) / 10 * 10 - 5 + p);
}

public func SetP(int p)
{
	SetAction("Edge");
	SetPhase(p);
	SetSolidMask(p * 10, 0, 10, 10);
}
 
public func Timer()
{
	if (EditCursor() != this)
		return;
	var p = GBackSolid(0, -10) * 2 + GBackSolid(10);
	SetP(p);
	Snap();
}
