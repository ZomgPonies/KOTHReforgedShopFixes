modded class KOTH_SessionPlayerLoadout : Managed
{
	
	override bool RemoveItem(KOTH_ShopItem item)
	{
		if (!item)
			return false;

		switch (item.m_category)
		{
			case KOTH_ShopItemCategory.AR:
			case KOTH_ShopItemCategory.DMR:
			case KOTH_ShopItemCategory.SMG:
			case KOTH_ShopItemCategory.LMG:
			case KOTH_ShopItemCategory.Sniper:
			case KOTH_ShopItemCategory.Shotgun:
				if (m_primary)
				{
					m_primary = null;
					return true;
				}
				return false;
			case KOTH_ShopItemCategory.Optics:
				if (m_optic)
				{
					m_optic = null;
					return true;
				}
				return false;
			case KOTH_ShopItemCategory.Muzzle:
				if (m_muzzle)
				{
					m_muzzle = null;
					return true;
				}
				return false;
			case KOTH_ShopItemCategory.Launcher:
				if (m_launcher)
				{
					m_launcher = null;
					return true;
				}
				return false;
			case KOTH_ShopItemCategory.Handgun:
				if (m_handgun)
				{
					m_handgun = null;
					return true;
				}
				return false;
			case KOTH_ShopItemCategory.Headgear:
				if (m_headgear)
				{
					m_headgear = null;
					return true;
				}
				return false;
			case KOTH_ShopItemCategory.Mask:
				if (m_mask)
				{
					m_mask = null;
					return true;
				}
				return false;
			case KOTH_ShopItemCategory.Cloak:
				if (m_cloak)
				{
					m_cloak = null;
					return true;
				}
				return false;
			case KOTH_ShopItemCategory.Pants:
				if (m_pants)
				{
					m_pants = null;
					return true;
				}
				return false;
			case KOTH_ShopItemCategory.Jacket:
				if (m_jacket)
				{
					m_jacket = null;
					return true;
				}
				return false;
			case KOTH_ShopItemCategory.Gloves:
				if (m_gloves)
				{
					m_gloves = null;
					return true;
				}
				return false;
			case KOTH_ShopItemCategory.Boots:
				if (m_boots)
				{
					m_boots = null;
					return true;
				}
				return false;
			case KOTH_ShopItemCategory.Backpack:
				if (m_backpack)
				{
					m_backpack = null;
					return true;
				}
				return false;
			case KOTH_ShopItemCategory.FullSuit:
				if (m_fullsuit)
				{
					m_fullsuit = null;
					return true;
				}
				return false;
			case KOTH_ShopItemCategory.Binocular:
				if (m_rangeFinder)
				{
					m_rangeFinder = null;
					return true;
				}
				return false;
			case KOTH_ShopItemCategory.NVG:
				if (m_nvg)
				{
					m_nvg = null;
					return true;
				}
				return false;
			case KOTH_ShopItemCategory.Grenade:
			case KOTH_ShopItemCategory.Smoke:
			case KOTH_ShopItemCategory.Mine:
				return RemoveThrowable(item);
		}

		return false;
	}


}
