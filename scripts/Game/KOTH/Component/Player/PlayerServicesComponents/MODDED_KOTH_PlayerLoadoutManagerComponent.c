modded class KOTH_PlayerLoadoutManagerComponentClass : KOTH_PlayerServicesComponentClass {}
modded class KOTH_PlayerLoadoutManagerComponent : KOTH_PlayerServicesComponent
{


	[RplProp(onRplName: "OnSessionLoadoutRplChanged", condition: RplCondition.OwnerOnly)]
    protected ResourceName m_rplHelmetAccessory;


    

    override void SyncLoadoutToOwnerRplProps()
    {
        if (!Replication.IsServer())
            return;

        PlayerController playerController = PlayerController.Cast(GetOwner());
        if (!playerController)
            return;

        KOTH_SessionPlayerLoadout loadout = GetPlayerLoadout(playerController.GetPlayerId());
        if (!loadout)
        {
            ClearRplResources();
            Replication.BumpMe();
            return;
        }

        m_rplPrimary = KOTH_SessionPlayerLoadout.GetSlotResource(loadout.m_primary);
        m_rplOptic = KOTH_SessionPlayerLoadout.GetSlotResource(loadout.m_optic);
        m_rplMuzzle = KOTH_SessionPlayerLoadout.GetSlotResource(loadout.m_muzzle);
        m_rplLauncher = KOTH_SessionPlayerLoadout.GetSlotResource(loadout.m_launcher);
        m_rplHandgun = KOTH_SessionPlayerLoadout.GetSlotResource(loadout.m_handgun);
        m_rplMask = KOTH_SessionPlayerLoadout.GetSlotResource(loadout.m_mask);
        m_rplCloak = KOTH_SessionPlayerLoadout.GetSlotResource(loadout.m_cloak);
        m_rplPants = KOTH_SessionPlayerLoadout.GetSlotResource(loadout.m_pants);
        m_rplJacket = KOTH_SessionPlayerLoadout.GetSlotResource(loadout.m_jacket);
        m_rplGloves = KOTH_SessionPlayerLoadout.GetSlotResource(loadout.m_gloves);
        m_rplBoots = KOTH_SessionPlayerLoadout.GetSlotResource(loadout.m_boots);
        m_rplBackpack = KOTH_SessionPlayerLoadout.GetSlotResource(loadout.m_backpack);
        m_rplHeadgear = KOTH_SessionPlayerLoadout.GetSlotResource(loadout.m_headgear);
        m_rplFullsuit = KOTH_SessionPlayerLoadout.GetSlotResource(loadout.m_fullsuit);
        m_rplRangeFinder = KOTH_SessionPlayerLoadout.GetSlotResource(loadout.m_rangeFinder);
        m_rplNvg = KOTH_SessionPlayerLoadout.GetSlotResource(loadout.m_nvg);
		m_rplHelmetAccessory = KOTH_SessionPlayerLoadout.GetSlotResource(loadout.m_helmetAccessory);

        if (!m_rplThrowables)
            m_rplThrowables = new array<ResourceName>();
        m_rplThrowables.Clear();

        if (loadout.m_throwables)
        {
            foreach (KOTH_ShopItem throwable : loadout.m_throwables)
            {
                ResourceName throwableResource = KOTH_SessionPlayerLoadout.GetSlotResource(throwable);
                if (!throwableResource.IsEmpty())
                    m_rplThrowables.Insert(throwableResource);
            }
        }

        Replication.BumpMe();
    }

    override protected void ClearRplResources()
    {
        const ResourceName emptyResource;
        m_rplPrimary = emptyResource;
        m_rplOptic = emptyResource;
        m_rplMuzzle = emptyResource;
        m_rplLauncher = emptyResource;
        m_rplHandgun = emptyResource;
        m_rplMask = emptyResource;
        m_rplCloak = emptyResource;
        m_rplPants = emptyResource;
        m_rplJacket = emptyResource;
        m_rplGloves = emptyResource;
        m_rplBoots = emptyResource;
        m_rplBackpack = emptyResource;
        m_rplHeadgear = emptyResource;
        m_rplFullsuit = emptyResource;
        m_rplRangeFinder = emptyResource;
        m_rplNvg = emptyResource;
		m_rplHelmetAccessory = emptyResource;

        if (!m_rplThrowables)
            m_rplThrowables = new array<ResourceName>();
        else
            m_rplThrowables.Clear();
    }


    override protected void ApplySessionLoadoutFromRplProps()
    {
        if (!m_sessionLoadout)
            m_sessionLoadout = new KOTH_SessionPlayerLoadout();

        if (!m_shopConfig)
        {
            SCR_BaseGameMode gameMode = SCR_BaseGameMode.Cast(GetGame().GetGameMode());
            if (gameMode)
                m_shopConfig = KOTH_ShopConfigGameModeComponent.Cast(gameMode.FindComponent(KOTH_ShopConfigGameModeComponent));
        }

        if (!m_shopConfig)
            return;

        array<ResourceName> slotResources = new array<ResourceName>();
        slotResources.Insert(m_rplPrimary);
        slotResources.Insert(m_rplOptic);
        slotResources.Insert(m_rplMuzzle);
        slotResources.Insert(m_rplLauncher);
        slotResources.Insert(m_rplHandgun);
        slotResources.Insert(m_rplMask);
        slotResources.Insert(m_rplCloak);
        slotResources.Insert(m_rplPants);
        slotResources.Insert(m_rplJacket);
        slotResources.Insert(m_rplGloves);
        slotResources.Insert(m_rplBoots);
        slotResources.Insert(m_rplBackpack);
        slotResources.Insert(m_rplHeadgear);
        slotResources.Insert(m_rplFullsuit);
        slotResources.Insert(m_rplRangeFinder);
        slotResources.Insert(m_rplNvg);
		slotResources.Insert(m_rplHelmetAccessory);

        m_sessionLoadout.ApplyFromRplSlotResources(slotResources, m_rplThrowables, m_shopConfig);
    }


}
