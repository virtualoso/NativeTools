#include "CreativeTab.h"

#include "com/mojang/minecraftpe/item/Item.h"
#include "com/mojang/minecraftpe/block/Block.h"
#include "com/mojang/minecraftpe/item/ItemInstance.h"
#include "../client/screens/InventoryTransitions.h"
#include "../item/InvPlusItems.h"

CreativeTab::CreativeTab(ItemInstance* item)
{
	tabIcon = item;
}

CreativeTab::CreativeTab(Item* item, int data) : CreativeTab(new ItemInstance(*item, 1, data)) { }

CreativeTab::CreativeTab(Block* block, int data) : CreativeTab(new ItemInstance(*block, 1, data)) { }

CreativeTab::CreativeTab(int itemId, int data) : CreativeTab(Item::mItems[itemId], data) { }

CreativeTab::CreativeTab() : CreativeTab(InvPlusItems::barrier, 0) { }

void CreativeTab::setTabIcon(ItemInstance* item)
{
	tabIcon = item;
}

void CreativeTab::setTabIcon(Item* item, int data)
{
	setTabIcon(new ItemInstance(*item, 1, data));
}

void CreativeTab::setTabIcon(Block* block, int data)
{
	setTabIcon(new ItemInstance(*block, 1, data));
}

void CreativeTab::setTabIcon(int itemId, int data)
{
	setTabIcon(Item::mItems[itemId], data);
}

ItemInstance* CreativeTab::getTabIcon()
{
	return tabIcon;
}

void CreativeTab::addItem(ItemInstance* item)
{
	itemsInTab.emplace_back(item);
}

void CreativeTab::addItem(Item* item, int data)
{
	addItem(new ItemInstance(*item, 1, data));
}

void CreativeTab::addItem(int itemId, int data)
{
	addItem(Item::mItems[itemId], data);
}

void CreativeTab::addItem(Block* block, int data)
{
	addItem(new ItemInstance(*block, 1, data));
}

void CreativeTab::addToTabsList()
{
	InventoryTransitions::creativeTabs.emplace_back(this);	
}