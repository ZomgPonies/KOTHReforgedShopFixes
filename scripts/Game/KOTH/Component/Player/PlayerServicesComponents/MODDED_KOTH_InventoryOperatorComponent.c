modded class KOTH_InventoryOperatorComponent : KOTH_PlayerServicesComponent
{
	
	// This enables debugging logs which make the entire flow be logged if needed
    protected const bool KOTH_VERBOSE_INVENTORY_LOGGING = false;
	
	private ResourceName catEars = "{16340C6BB3EB1BEC}Prefabs/Items/Equipment/Accessories/Neko_Ears/Neko_Ears.et";

	override void ProcessLoadoutItemsStaged(KOTH_SessionPlayerLoadout loadout, int playerId)
	{
		if (!loadout || playerId <= 0)
			return;

		LogItemReplaceTrace(string.Format("Staged pipeline begin playerId=%1 stress_active=%2", playerId, IsLoadoutStressTestActive(playerId)));

		// Replace any in-flight loadout execution for this player to avoid mixed state.
		if (m_activeLoadoutExecutions.Contains(playerId))
		{
			LogItemReplaceDebug(string.Format("Staged pipeline replacing in-flight execution playerId=%1", playerId));
			FinalizeStagedLoadoutExecution(playerId, false);
		}

		KOTH_LoadoutExecutionContext context = new KOTH_LoadoutExecutionContext();
		context.m_executionId = m_nextLoadoutExecutionId++;
		context.m_playerId = playerId;
		context.m_isStressRun = IsLoadoutStressTestActive(playerId);

		AddLoadoutItemIfValid(context.m_clothes, loadout.m_headgear);
		AddLoadoutItemIfValid(context.m_clothes, loadout.m_jacket);
		AddLoadoutItemIfValid(context.m_clothes, loadout.m_pants);
		AddLoadoutItemIfValid(context.m_clothes, loadout.m_boots);
		AddLoadoutItemIfValid(context.m_clothes, loadout.m_gloves);
		AddLoadoutItemIfValid(context.m_clothes, loadout.m_backpack);
		AddLoadoutItemIfValid(context.m_clothes, loadout.m_mask);
		AddLoadoutItemIfValid(context.m_clothes, loadout.m_cloak);
		AddLoadoutItemIfValid(context.m_clothes, loadout.m_fullsuit);

		AddLoadoutItemIfValid(context.m_accessoriesMeds, loadout.m_rangeFinder);
		AddLoadoutItemIfValid(context.m_accessoriesMeds, loadout.m_nvg);
		AddLoadoutItemIfValid(context.m_accessoriesMeds, loadout.m_helmetAccessory);
		if (loadout.m_throwables)
		{
			foreach (KOTH_ShopItem throwable : loadout.m_throwables)
			{
				AddLoadoutItemIfValid(context.m_accessoriesMeds, throwable);
			}
		}

		AddLoadoutItemIfValid(context.m_weapons, loadout.m_launcher);
		AddLoadoutItemIfValid(context.m_weapons, loadout.m_primary);
		AddLoadoutItemIfValid(context.m_weapons, loadout.m_handgun);

		AddLoadoutItemIfValid(context.m_attachments, loadout.m_optic);
		AddLoadoutItemIfValid(context.m_attachments, loadout.m_muzzle);

		m_activeLoadoutExecutions.Set(playerId, context);
		LogItemReplaceTrace(string.Format(
			"Staged pipeline queued playerId=%1 exec=%2 stress_run=%3 stage_counts={clothes:%4,accessories:%5,weapons:%6,attachments:%7}",
			playerId,
			context.m_executionId,
			context.m_isStressRun,
			context.m_clothes.Count(),
			context.m_accessoriesMeds.Count(),
			context.m_weapons.Count(),
			context.m_attachments.Count()
		));
		ProcessNextStagedLoadoutItem(playerId);
	}

    override protected bool AddEquipmentItemWithCallback(KOTH_ShopItem item, SCR_InventoryStorageManagerComponent inventory, InventoryOperationCallback cb)
    {
        if (item && item.m_category == KOTH_ShopItemCategory.Binocular)
            return AddBinocularItemWithCallback(item, inventory, cb);
		
		if (item && item.m_itemResource == catEars)
			return AddCatEarsItem(item, inventory);

        bool started = inventory.TrySpawnPrefabToStorage(item.m_itemResource, null, -1, EStoragePurpose.PURPOSE_ANY, cb);
        LogItemReplaceTrace(string.Format("Equipment add request started=%1 %2", started, DescribeShopItemForReplaceLog(item)));
        return started;
    }	
	
}