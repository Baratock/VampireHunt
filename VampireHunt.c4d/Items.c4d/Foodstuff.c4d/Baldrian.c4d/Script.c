#strict 2

#include WMPF

protected func Eat(object pClonk)
{
	pClonk->Feed(2);
	pClonk->DoVal(cPain, -10);
	pClonk->DoVal(cSleep, 20);
	RemoveObject(this);
	return 1;
}