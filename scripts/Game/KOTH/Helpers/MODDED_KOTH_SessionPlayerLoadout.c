modded class KOTH_SessionPlayerLoadout : Managed
{

	ref KOTH_ShopItem m_helmetAccessory;
	private ResourceName catEars = "{16340C6BB3EB1BEC}Prefabs/Items/Equipment/Accessories/Neko_Ears/Neko_Ears.et";
	
	override bool IsEmpty()
    {
        if (m_primary)
            return false;

        if (m_optic)
            return false;

        if (m_muzzle)
            return false;

        if (m_launcher)
            return false;

        if (m_handgun)
            return false;

        if (m_headgear)
            return false;

        if (m_rangeFinder)
            return false;

        if (m_mask)
            return false;

        if (m_cloak)
            return false;

        if (m_pants)
            return false;

        if (m_jacket)
            return false;

        if (m_gloves)
            return false;

        if (m_boots)
            return false;

        if (m_backpack)
            return false;

        if (m_fullsuit)
            return false;

        if (m_nvg)
            return false;
		
		if (m_helmetAccessory)
			return false;

        if (!m_throwables.IsEmpty())
            return false;

        return true;
    }


    override void Clear()
    {
        m_primary = null;
        m_optic = null;
        m_muzzle = null;
        m_launcher = null;
        m_handgun = null;
        m_headgear = null;
        m_rangeFinder = null;
        m_mask  = null;
        m_pants = null;
        m_jacket = null;
        m_cloak = null;
        m_gloves = null;
        m_boots = null;
        m_backpack = null;
        m_fullsuit = null;
        m_nvg = null;
		m_helmetAccessory = null;
        m_throwables = {};
    }	
			
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
			case KOTH_ShopItemCategory.Accessory:
				 if(item.m_itemResource == catEars)
					if (m_helmetAccessory)
					{
						m_helmetAccessory = null;
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

	static const int LOADOUT_SLOT_COUNT = 17;

    override void ApplyFromRplSlotResources(array<ResourceName> slotResources, array<ResourceName> throwables, KOTH_ShopConfigGameModeComponent shopConfig)
    {
        if (!shopConfig || !slotResources || slotResources.Count() < LOADOUT_SLOT_COUNT)
            return;

        m_primary = ResolveShopItem(slotResources[0], shopConfig);
        m_optic = ResolveShopItem(slotResources[1], shopConfig);
        m_muzzle = ResolveShopItem(slotResources[2], shopConfig);
        m_launcher = ResolveShopItem(slotResources[3], shopConfig);
        m_handgun = ResolveShopItem(slotResources[4], shopConfig);
        m_mask = ResolveShopItem(slotResources[5], shopConfig);
        m_cloak = ResolveShopItem(slotResources[6], shopConfig);
        m_pants = ResolveShopItem(slotResources[7], shopConfig);
        m_jacket = ResolveShopItem(slotResources[8], shopConfig);
        m_gloves = ResolveShopItem(slotResources[9], shopConfig);
        m_boots = ResolveShopItem(slotResources[10], shopConfig);
        m_backpack = ResolveShopItem(slotResources[11], shopConfig);
        m_headgear = ResolveShopItem(slotResources[12], shopConfig);
        m_fullsuit = ResolveShopItem(slotResources[13], shopConfig);
        m_rangeFinder = ResolveShopItem(slotResources[14], shopConfig);
        m_nvg = ResolveShopItem(slotResources[15], shopConfig);
		m_helmetAccessory = ResolveShopItem(slotResources[16], shopConfig);

        m_throwables.Clear();
        if (throwables)
        {
            foreach (ResourceName throwableResource : throwables)
            {
                KOTH_ShopItem throwable = ResolveShopItem(throwableResource, shopConfig);
                if (throwable)
                    m_throwables.Insert(throwable);
            }
        }
    }
	
}
