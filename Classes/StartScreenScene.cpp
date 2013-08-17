//THIS IS THE START SCREEN SCENE

#include "StartScreenScene.h"
#include "HelloWorldScene.h"

using namespace cocos2d;

CCScene* StartScreen::scene()
{
    // 'scene' is an autorelease object
     CCScene *scene = CCScene::create();

    // 'layer' is an autorelease object
    StartScreen *layer = StartScreen::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool StartScreen::init()
{
    bool bRet = false;
    do 
    {
        //////////////////////////////////////////////////////////////////////////
        // super init first
        //////////////////////////////////////////////////////////////////////////

        CC_BREAK_IF(! CCLayer::init());

        //////////////////////////////////////////////////////////////////////////
        // add your codes below...
        //////////////////////////////////////////////////////////////////////////

		//Get the sizes
		CCSize size = CCDirector::sharedDirector()->getWinSize();

		/////////////////////////******MENU ITEMS********/////////////////////////
	       // 1. Add a menu item with "X" image, which is clicked to quit the program.

        // Create a "close" menu item with close icon, it's an auto release object.
      CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
            "CloseNormal.png",
            "CloseSelected.png",
            this,
            menu_selector(StartScreen::menuCloseCallback));
        CC_BREAK_IF(! pCloseItem);

        // Place the menu item bottom-right conner.
        pCloseItem->setPosition(ccp(size.width/2,size.height/2));

        // Create a menu with the "close" menu item, it's an auto release object.
        CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
        pMenu->setPosition(CCPointZero);
        CC_BREAK_IF(! pMenu);

        // Add the menu to StartScreen layer as a child layer.
        this->addChild(pMenu, 1);

		// 1* Add a menu item with "->" image, which is clicked to go to the main game scene
		// Create a "start" menu item with start icon, it's an auto release object.
        CCMenuItemImage *pStartItem = CCMenuItemImage::create(
            "to.png",
            "to.png",
            this,
            menu_selector(StartScreen::menuStartCallback));
        CC_BREAK_IF(! pStartItem);

		//Place the start menu item
        pStartItem->setPosition(ccp(size.width/2,size.height/2+100));
		
        // Create a menu with the "start" menu item, it's an auto release object.
        CCMenu* pStart = CCMenu::create(pStartItem, NULL);
        pStart->setPosition(CCPointZero);
        CC_BREAK_IF(! pStart);

        // Add the menu to StartScreen layer as a child layer.
		this->addChild(pStart, 5);

/*
		// 1** Add a menu item with options image, which is clicked to go to the Preferences
		// Create a "Options" menu item with the "options" icon, it's an auto release object
		CCMenuItemImage *pPreferencesItem = CCMenuItemImage::create(
			"OptionsButton.png",
			"OptionsButtonSelected.png",
			this,
			menu_selector(StartScreen::menuPreferencesCallback));

		//Place the Preferences Item
		pPreferencesItem->setPosition(ccp(size.width - 90, 200));

		//Create a menu with the "preferences" menu item, it's an auto release object
		CCMenu* pPreferences = CCMenu::create(pPreferencesItem, NULL);
		pPreferences->setPosition(CCPointZero);
		CC_BREAK_IF(! pPreferences);

		//Add the menu item to StartScreen layer as a child layer
		this->addChild(pPreferences, 1);
*/
		//////////////////*****END OF MENU ITEMS***********/////////////////////
        bRet = true;
		
    } while (0);

	setTouchEnabled (true);

    return bRet;
}


//////////////***MENU CALLBACK***////////////////////////////////////////

void StartScreen::menuStartCallback(CCObject* pSender){
	CCDirector::sharedDirector()->replaceScene(HelloWorld::scene());
	CCDirector::sharedDirector()->resume();
}

/*
void StartScreen::menuPreferencesCallback(CCObject* pSender){
	CCDirector::sharedDirector()->replaceScene(Preferences::scene());
}
*/

void StartScreen::menuCloseCallback(CCObject* pSender){
    // "close" menu item clicked
	CCDirector::sharedDirector()->end();
}
