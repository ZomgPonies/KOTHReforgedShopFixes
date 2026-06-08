modded class KOTH_SessionLoadoutUI : ChimeraMenuBase
{
    override protected void RebuildItemList()
    {
        if (!m_contentContainer)
            return;

        ClearItemList();

        if (!m_pLoadoutManagerComp || !m_pLoadoutManagerComp.m_sessionLoadout)
        {
            SetTotalPrice(0);
            return;
        }

        KOTH_SessionPlayerLoadout loadoutData = m_pLoadoutManagerComp.m_sessionLoadout;

        int total = 0;
        if (loadoutData.m_primary)
            total = total + AddTextItem(loadoutData.m_primary);
        if (loadoutData.m_handgun)
            total = total + AddTextItem(loadoutData.m_handgun);
        if (loadoutData.m_launcher)
            total = total + AddTextItem(loadoutData.m_launcher);
        if (loadoutData.m_optic)
            total = total + AddTextItem(loadoutData.m_optic);
        if (loadoutData.m_muzzle)
            total = total + AddTextItem(loadoutData.m_muzzle);
        if (loadoutData.m_headgear)
            total = total + AddTextItem(loadoutData.m_headgear);
        if (loadoutData.m_nvg)
            total = total + AddTextItem(loadoutData.m_nvg);
        if (loadoutData.m_rangeFinder)
            total = total + AddTextItem(loadoutData.m_rangeFinder);
        if (loadoutData.m_mask)
            total = total + AddTextItem(loadoutData.m_mask);
        if (loadoutData.m_cloak)
            total = total + AddTextItem(loadoutData.m_cloak);
        if (loadoutData.m_pants)
            total = total + AddTextItem(loadoutData.m_pants);
        if (loadoutData.m_jacket)
            total = total + AddTextItem(loadoutData.m_jacket);
        if (loadoutData.m_gloves)
            total = total + AddTextItem(loadoutData.m_gloves);
        if (loadoutData.m_boots)
            total = total + AddTextItem(loadoutData.m_boots);
        if (loadoutData.m_backpack)
            total = total + AddTextItem(loadoutData.m_backpack);
        if (loadoutData.m_fullsuit)
            total = total + AddTextItem(loadoutData.m_fullsuit);
		if (loadoutData.m_neko)
			total = total + AddTextItem(loadoutData.m_neko);

        if (loadoutData.m_throwables)
        {
            foreach (KOTH_ShopItem item : loadoutData.m_throwables)
            {
                total = total + AddTextItem(item);
            }
        }

        SetTotalPrice(total);
    }
}
