#include "pch.h"
#include "Sort.h"

namespace Utils
{
	bool Sort::sortGameObject(GameObject* obj1, GameObject* obj2)
	{
		if (obj1->GetSortLayer() != obj2->GetSortLayer())
		{
			return obj1->GetSortLayer() < obj2->GetSortLayer();
		}
		return obj1->GetSortOrder() < obj2->GetSortOrder();
	}
}
