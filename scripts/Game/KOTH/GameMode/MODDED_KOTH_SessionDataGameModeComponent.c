modded class KOTH_SessionDataGameModeComponentClass : SCR_BaseGameModeComponentClass {}
modded class KOTH_SessionDataGameModeComponent : SCR_BaseGameModeComponent
{	
	
	private ResourceName catEars = "{16340C6BB3EB1BEC}Prefabs/Items/Equipment/Accessories/Neko_Ears/Neko_Ears.et";
	
	override void SaveLoadoutChoiceInSession(KOTH_ShopItem item, int playerId)
	{
		string playerUID = KOTH_Helper.GetPlayerUID(playerId);
		KOTH_SessionPlayerLoadout playerLoadout = m_sessionPlayersLoadout.Get(playerUID);
		if (!playerLoadout)
			playerLoadout = new KOTH_SessionPlayerLoadout();

		switch (item.m_category)
		{
			case KOTH_ShopItemCategory.AR:
			case KOTH_ShopItemCategory.DMR:
			case KOTH_ShopItemCategory.SMG:
			case KOTH_ShopItemCategory.LMG:
			case KOTH_ShopItemCategory.Sniper:
			case KOTH_ShopItemCategory.Shotgun:
				playerLoadout.m_primary = item;
			break;
			case KOTH_ShopItemCategory.Optics:
				playerLoadout.m_optic = item;
			break;
			case KOTH_ShopItemCategory.Muzzle:
				playerLoadout.m_muzzle = item;
			break;
			case KOTH_ShopItemCategory.Launcher:
				playerLoadout.m_launcher = item;
			break;
			case KOTH_ShopItemCategory.Handgun:
				playerLoadout.m_handgun = item;
			break;
			case KOTH_ShopItemCategory.Headgear:
				playerLoadout.m_headgear = item;
			break;
			case KOTH_ShopItemCategory.Mask:
				playerLoadout.m_mask = item;
			break;
			case KOTH_ShopItemCategory.Cloak:
				playerLoadout.m_cloak = item;
			break;
			case KOTH_ShopItemCategory.Pants:
				playerLoadout.m_pants = item;
			break;
			case KOTH_ShopItemCategory.Jacket:
				playerLoadout.m_jacket = item;
			break;
			case KOTH_ShopItemCategory.Gloves:
				playerLoadout.m_gloves = item;
			break;
			case KOTH_ShopItemCategory.Boots:
				playerLoadout.m_boots = item;
			break;
			case KOTH_ShopItemCategory.Backpack:
				playerLoadout.m_backpack = item;
			break;
			case KOTH_ShopItemCategory.FullSuit:
				playerLoadout.m_fullsuit = item;
			break;
			case KOTH_ShopItemCategory.Binocular:
				playerLoadout.m_rangeFinder = item;
			break;
			case KOTH_ShopItemCategory.NVG:
				playerLoadout.m_nvg = item;
			break;
			case KOTH_ShopItemCategory.Accessory:
				if (item.m_itemResource == catEars)
					playerLoadout.m_neko = item;
			break;
			case KOTH_ShopItemCategory.Grenade:
				int nadeCounter = 0;
				foreach(KOTH_ShopItem throwable : playerLoadout.m_throwables)
				{
					if (throwable.m_category == KOTH_ShopItemCategory.Grenade)
						nadeCounter++;
				}

				if (nadeCounter < 5) 
					playerLoadout.m_throwables.Insert(item);
			break;
			case KOTH_ShopItemCategory.Smoke:
				int smokeCounter = 0;
				foreach(KOTH_ShopItem throwable : playerLoadout.m_throwables)
				{
					if (throwable.m_category == KOTH_ShopItemCategory.Smoke)
						smokeCounter++;
				}

				if (smokeCounter < 5) 
					playerLoadout.m_throwables.Insert(item);
			break;
			case KOTH_ShopItemCategory.Mine:
				int mineCounter = 0;
				foreach(KOTH_ShopItem throwable : playerLoadout.m_throwables)
				{
					if (throwable.m_category == KOTH_ShopItemCategory.Mine)
						mineCounter++;
				}

				if (mineCounter < 3) 
					playerLoadout.m_throwables.Insert(item);
			break;
			default:
			break;
		}
		
		m_sessionPlayersLoadout.Set(playerUID, playerLoadout);
		m_LastLoadoutChangeTime.Set(playerUID, GetGame().GetWorld().GetWorldTime());
	}


}
