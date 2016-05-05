#include "ExtendedInventoryScreen.h"

#include "InventoryTransitions.h"
#include "../../creative/CreativeTab.h"

#include "com/mojang/minecraftpe/client/MinecraftClient.h"
#include "com/mojang/minecraftpe/client/gui/NinePatchLayer.h"
#include "com/mojang/minecraftpe/client/gui/IntRectangle.h"
#include "com/mojang/minecraftpe/client/gui/ImageWithBackground.h"
#include "com/mojang/minecraftpe/client/gui/InventoryTab.h"
#include "com/mojang/minecraftpe/client/settings/Options.h"
#include "com/mojang/minecraftpe/client/renderer/Tessellator.h"
#include "com/mojang/minecraftpe/client/renderer/texture/TextureGroup.h"
#include "com/mojang/minecraftpe/client/renderer/entity/ItemRenderer.h"

ExtendedInventoryScreen::ExtendedInventoryScreen(MinecraftClient& client, std::vector<CreativeTab*> creativeTabs)
	: Screen(client)
{
	closeButton = NULL;
	backgroundLayer = NULL;
	leftButtonLayer = NULL;
	rightButtonLayer = NULL;
	ownedTabs = creativeTabs;
}

bool ExtendedInventoryScreen::renderGameBehind() const
{
	return mcClient->getOptions()->getFancyGraphics();
}

bool ExtendedInventoryScreen::closeOnPlayerHurt() const
{
	return true;
}

void ExtendedInventoryScreen::init()
{
	if(!closeButton)
	{
		InventoryTransitions::init(this);
		
		NinePatchFactory factory (mcClient->getTextures(), "gui/spritesheet.png");
		
		backgroundLayer = std::shared_ptr<NinePatchLayer>(factory.createSymmetrical({34, 43, 14, 14}, 3, 3, 14.0F, 14.0F));
		leftButtonLayer = std::shared_ptr<NinePatchLayer>(factory.createSymmetrical({49, 43, 14, 14}, 3, 3, 14.0F, 14.0F));
		rightButtonLayer = std::shared_ptr<NinePatchLayer>(factory.createSymmetrical({65, 55, 14, 14}, 3, 3, 14.0F, 14.0F));
		
		closeButton = std::make_shared<ImageWithBackground>(2);
		closeButton->init(mcClient->getTextures(), 28, 28, {49, 43, 14, 14}, {49, 43, 14, 14}, 2, 2, "gui/spritesheet.png");
		closeButton->setImageDef({mcClient->getTextures()->getTexture("gui/spritesheet.png", TextureLocation::Default), 0, 1, 18.0F, 18.0F, {60, 0, 18, 18}, true}, true);
		
		for(int tab = 0; tab < ownedTabs.size(); tab++)
		{
			renderedTabs.emplace_back(createInventoryTab(tab + 3, ((tab < 4) ? false : true)));
		}
		
		selectedTabIndex = 0;
		
		buttonList.emplace_back(closeButton);
	}
}

void ExtendedInventoryScreen::render(int i1, int i2, float f1)
{
	if(renderGameBehind())
		renderBackground(1);
	else
		renderDirtBackground();
	
	renderToolBar(f1, 1.0F, false);
	
	for(int tab = 0; tab < renderedTabs.size(); tab++)
	{
		if(tab != selectedTabIndex)
		{
			renderedTabs[tab]->overlay = {0.75F, 0.75F, 0.75F, 1.0F};
			renderedTabs[tab]->renderBg(mcClient, i1, i2);
			drawTabIcon(ownedTabs[tab], renderedTabs[tab], renderedTabs[tab]->pressed);
		}
	}
	
	backgroundLayer->draw(Tessellator::instance, backgroundLayer->xPosition, backgroundLayer->yPosition);
	
	renderedTabs[selectedTabIndex]->overlay = Color::WHITE;
	renderedTabs[selectedTabIndex]->renderBg(mcClient, i1, i2);
	drawTabIcon(ownedTabs[selectedTabIndex], renderedTabs[selectedTabIndex], renderedTabs[selectedTabIndex]->pressed);	
	
	InventoryTransitions::render(this, i1, i2, f1);
	
	Screen::render(i1, i2, f1);
	
	renderOnSelectItemNameText(width, mcClient->getFont(), height - 41);
}

void ExtendedInventoryScreen::setupPositions()
{
	backgroundLayer->xPosition = 31;
	backgroundLayer->yPosition = 2;
	backgroundLayer->setSize((float)(width - 26 - 28) - 4.0F - 6.0F, ((float) height) - 25.0F);
	
	closeButton->xPosition = backgroundLayer->xPosition - 26;
	closeButton->yPosition = backgroundLayer->yPosition;
	
	closeButton->setSize(29, 28);
	
	for(int tab = 0; tab < renderedTabs.size(); tab++)
	{
		if(!renderedTabs[tab]->isRight)
		{
			renderedTabs[tab]->xPosition = backgroundLayer->xPosition - 26;
			renderedTabs[tab]->yPosition = height - 25 - 29 - (tab * 31);
		}
		else
		{
			renderedTabs[tab]->xPosition = width - 6 - 29;
			renderedTabs[tab]->yPosition = height - 25 - 29 - ((tab - 4) * 31);
		}
		renderedTabs[tab]->width = 29;
		renderedTabs[tab]->height = 29;
	}
	
	InventoryTransitions::setupPositions(this);
}

void ExtendedInventoryScreen::_buttonClicked(Button& button)
{
	InventoryTransitions::_buttonClicked(this, button);
	
	if(button.id == closeButton->id)
		InventoryTransitions::pushPreviousScreen(this);
}

void ExtendedInventoryScreen::_pointerReleased(int x, int y)
{
	Screen::_pointerReleased(x, y);
	
	for(int tab = 0; tab < renderedTabs.size(); tab++)
	{
		if(tab != selectedTabIndex && renderedTabs[tab]->isInside(x, y))
		{
			selectedTabIndex = tab;
		}
	}
}

void ExtendedInventoryScreen::handleBackEvent(bool b1)
{
	if(!b1)
		InventoryTransitions::pushPreviousScreen(this);
}

bool ExtendedInventoryScreen::isModal() const
{
	return true;
}

void ExtendedInventoryScreen::tick()
{
	
}

std::string ExtendedInventoryScreen::getScreenName()
{
	return "extended_creative_screen";
}

std::shared_ptr<InventoryTab> ExtendedInventoryScreen::createInventoryTab(int id, bool isRight)
{
	NinePatchLayer* buttonLayer = isRight ? rightButtonLayer.get() : leftButtonLayer.get();
	std::shared_ptr<InventoryTab> button = std::make_shared<InventoryTab>(id, "", buttonLayer, isRight);
	button->setOverrideScreenRendering(true);
	return button;
}

void ExtendedInventoryScreen::drawTabIcon(CreativeTab* ownedTab, std::shared_ptr<InventoryTab> imageButton, bool isPressed)
{
	ItemRenderer::getInstance()->renderGuiItemNew(ownedTab->getTabIcon(), 0, ((float)imageButton->xPosition + (float)((imageButton->width / 2) - 8) + (isPressed ? 1.0F : 0.7F)), ((float)imageButton->yPosition + (float)((imageButton->height / 2) - 8)), 1.0F, (isPressed ? 1.0F : 0.7F), (((float)imageButton->width) - (isPressed ? 2.0F : 0.0F)) * 0.04F, false);
}
