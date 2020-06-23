#strict 2

#appendto DWPF

public func Eat(pBy){}

public func Activate(clonk)
{
	if(clonk->~Who() == Role_Vampire || clonk->~Who() == Role_Lich)
	{
		clonk -> Feed(25,1);
		return(RemoveObject());
	}
	else
	{
		CreateContents(BONE,clonk,1);
		if(clonk->~Job() == J_HN)
			CreateContents(RMET,clonk,3);
		else
			CreateContents(RMET,clonk,2);
		RemoveObject(this);
	}
}
