#include "ExtendedInventoryScreen.h"

#include "InventoryTransitions.h"

#include "com/mojang/minecraftpe/client/MinecraftClient.h"
#include "com/mojang/minecraftpe/client/gui/Gui.h"

ExtendedInventoryScreen::ExtendedInventoryScreen(MinecraftClient& client)
	: Screen(client)
{
	
}

bool ExtendedInventoryScreen::renderGameBehind() const
{
	return true;
}

bool ExtendedInventoryScreen::closeOnPlayerHurt() const
{
	return true;
}

void ExtendedInventoryScreen::render(int i1, int i2, float f1)
{
	mcClient->getGui()->renderToolBar(f1, 1.0F, true);
	
	InventoryTransitions::render(this, i1, i2, f1);
}

void ExtendedInventoryScreen::init()
{
	InventoryTransitions::init(this);
}

void ExtendedInventoryScreen::setupPositions()
{
	InventoryTransitions::setupPositions(this);
}

void ExtendedInventoryScreen::_buttonClicked(Button& button)
{
	InventoryTransitions::_buttonClicked(this, button);
}