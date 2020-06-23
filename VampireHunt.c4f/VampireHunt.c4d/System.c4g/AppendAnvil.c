/*Amboss-Appendto*/

#strict 2
#appendto ANVL

private func Forging() {
	// Noch nicht fertig
	if (GetActTime() < 150) return 0;
	
	var pProduct = ComposeContents(product);
	// Objekt auswerfen
	if(pProduct)
		Exit(pProduct);
	var i = 0, obj;
	while (obj = Contents(i++))
		if (GetOCF(obj) & OCF_CrewMember)
			obj->~OnSmithSuccess();
	// Aktion beenden und Clonk auswerfen
	ProductionCancel();
	return 1;
}
