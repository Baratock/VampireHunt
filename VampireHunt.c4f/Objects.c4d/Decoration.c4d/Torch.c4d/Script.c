/*-- Fackel --*/

#strict 2

public func Activate()
{
	if (GetAction() == "Burning")
		return Extinguishsh();
	SetAction("Burning");
	return this;
}
  
private func Extinguishsh()
{
	SetAction("Idle");
	Sound("Pshshsh");
	return 1;
}

