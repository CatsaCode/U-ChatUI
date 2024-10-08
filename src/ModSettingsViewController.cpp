#include "ModSettingsViewController.hpp"

#include "bsml/shared/BSML.hpp"
#include "bsml/shared/BSML/Components/ExternalComponents.hpp"

#include "HMUI/ViewController.hpp"
#include "HMUI/TextSegmentedControl.hpp"

#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/Vector2.hpp"
#include "UnityEngine/Component.hpp"
#include "UnityEngine/RectTransform.hpp"
#include "UnityEngine/UI/HorizontalLayoutGroup.hpp"
#include "UnityEngine/UI/ContentSizeFitter.hpp"
#include "UnityEngine/UI/Image.hpp"

#include "logging.hpp"
#include "ModConfig.hpp"

using namespace BSML;
using namespace UnityEngine;
using namespace UnityEngine::UI;
using namespace HMUI;

// Create a VerticalLayoutGroup that doesn't try to equally space children
UI::VerticalLayoutGroup* CreateVerticalList(const BSML::Lite::TransformWrapper& parent) {
    // Crate component that will space other components vertically
    auto verticalLayout = BSML::Lite::CreateVerticalLayoutGroup(parent);
    // Place components one on top of the other; don't try to space them out evenly
    verticalLayout->childControlHeight = false;
    verticalLayout->childForceExpandHeight = false;

    return verticalLayout;
}

void DidActivate(ViewController* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
    // Only create UI when first loaded
    if(!firstActivation) return;

    // Container for the entire mod menu  
    auto mainContainer = CreateVerticalList(self->transform);

    // Pre-create each tab GameObject so they can be referenced in tabsRow
    auto generalTab = CreateVerticalList(mainContainer);
    auto panelTab = CreateVerticalList(mainContainer);
    auto emoteTab = CreateVerticalList(mainContainer);
    auto apiTab = CreateVerticalList(mainContainer);
    auto creditsTab = CreateVerticalList(mainContainer);
    std::vector<GameObject*> tabs = {
        generalTab->gameObject,
        panelTab->gameObject,
        emoteTab->gameObject,
        apiTab->gameObject,
        creditsTab->gameObject
    };

    // Create a tab selector to enable each tab
    std::function<void(int)> ShowTab = [tabs](int selection){
        // Disable all tabs
        for(int i = 0; i < tabs.size(); i++) tabs[i]->gameObject->SetActive(false);
        // Enable the selected tab
        tabs[selection]->gameObject->SetActive(true);
    };
    std::string_view tabLabels[] = {"General", "Panel", "Emotes", "API", "Credits"};
    auto tabsRow = BSML::Lite::CreateTextSegmentedControl(mainContainer, Vector2(0.0f, 0.0f), Vector2(120.0f, 7.0f), std::span<std::string_view>(tabLabels), ShowTab);
    tabsRow->_hideCellBackground = false;
    tabsRow->_fontSize = 4;
    tabsRow->ReloadData();
    tabsRow->gameObject->AddComponent<LayoutElement*>()->preferredWidth = 120.0f;
    // Make sure that the tab selector appears above the tab panels themselves
    tabsRow->transform->SetAsFirstSibling();
    // Hide all tabs but the general tab
    ShowTab(0);
    


    // General tab

    auto channelInput = AddConfigValueInputString(generalTab, getModConfig().Channel);
    auto songOverlayEnabledToggle = AddConfigValueToggle(generalTab, getModConfig().SongOverlay_Enabled);
    
    // Currently disabled settings
    BSML::Lite::CreateText(generalTab, "", Vector2(0.0f, 0.0f), Vector2(1.0f, 5.0f)); // Spacer
    BSML::Lite::CreateText(generalTab, "Coming Soon", Vector2(0.0f, 0.0f), Vector2(1.0f, 5.0f))->horizontalAlignment = TMPro::HorizontalAlignmentOptions::Center;
    auto youtubeChannelInput = AddConfigValueInputString(generalTab, getModConfig().Youtube_Channel);
    auto chatToggle = AddConfigValueToggle(generalTab, getModConfig().Chat);
    auto twitchYoutubeShowViewersToggle = AddConfigValueToggle(generalTab, getModConfig().TwitchYoutube_ShowViewers);
    auto songOverlayToggle = AddConfigValueToggle(generalTab, getModConfig().SongOverlay);
    auto chatIntegrationsToggle = AddConfigValueToggle(generalTab, getModConfig().ChatIntegrations);

    // Disable the UI for the disabled settings
    youtubeChannelInput->set_interactable(false);
    chatToggle->set_interactable(false);
    twitchYoutubeShowViewersToggle->set_interactable(false);
    songOverlayToggle->set_interactable(false);
    chatIntegrationsToggle->set_interactable(false);



    // Panel tab

    // Popup window for adusting the panel transform in menu
    BSML::ModalView* panelMenuPositionModal = BSML::Lite::CreateModal(self->transform, Vector2(0.0f, 5.0f), Vector2(80.0f, 66.0f), nullptr);
    GameObject* panelMenuPositionModalContainer = BSML::Lite::CreateScrollableModalContainer(panelMenuPositionModal);
    panelMenuPositionModalContainer->GetComponent<BSML::ExternalComponents*>()->Get<RectTransform*>()->set_anchoredPosition(Vector2(4.0f, 0.0f));
    AddConfigValueIncrementVector3(panelMenuPositionModalContainer, getModConfig().PositionMenu, 2, 0.05f);
    AddConfigValueIncrementVector3(panelMenuPositionModalContainer, getModConfig().RotationMenu, 0, 1.0f);
    AddConfigValueIncrementVector2(panelMenuPositionModalContainer, getModConfig().SizeMenu, 0, 1.0f);
    
    // Popup window for adusting the panel transform in game
    BSML::ModalView* panelGamePositionModal = BSML::Lite::CreateModal(self->transform, Vector2(0.0f, 5.0f), Vector2(80.0f, 66.0f), [](){getModConfig().ForceGame.SetValue(false);});
    GameObject* panelGamePositionModalContainer = BSML::Lite::CreateScrollableModalContainer(panelGamePositionModal);
    panelGamePositionModalContainer->GetComponent<BSML::ExternalComponents*>()->Get<RectTransform*>()->set_anchoredPosition(Vector2(4.0f, 0.0f));
    AddConfigValueIncrementVector3(panelGamePositionModalContainer, getModConfig().PositionGame, 2, 0.05f);
    AddConfigValueIncrementVector3(panelGamePositionModalContainer, getModConfig().RotationGame, 0, 1.0f);
    AddConfigValueIncrementVector2(panelGamePositionModalContainer, getModConfig().SizeGame, 0, 1.0f);

    // Buttons for opening the panel transform modals
    auto panelPositionButtonRow = BSML::Lite::CreateHorizontalLayoutGroup(panelTab);
    auto panelMenuPositionModalButton = BSML::Lite::CreateUIButton(panelPositionButtonRow, "Menu Position", [panelMenuPositionModal](){panelMenuPositionModal->Show();});
    auto panelGamePositionModalButton = BSML::Lite::CreateUIButton(panelPositionButtonRow, "Game Position", [panelGamePositionModal](){panelGamePositionModal->Show(); getModConfig().ForceGame.SetValue(true);});
    // TODO This row will appear out of place until the pointer touches it... Seems SetDirty will only work when the object is enabled??
    // panelPositionButtonRow->SetDirty();
    // panelPositionButtonRow->GetComponent<LayoutElement*>()->preferredHeight = 7.74;
    // panelPositionButtonRow->GetComponent<ContentSizeFitter*>()->SetDirty();
    // panelPositionButtonRow->GetComponent<LayoutElement*>()->SetDirty();
    // panelPositionButtonRow->SetDirty();

    AddConfigValueIncrementVector2(panelTab, getModConfig().FontSizeChat, 1, 0.1f);
    AddConfigValueIncrementVector2(panelTab, getModConfig().BackGroundTransChat, 1, 0.1f);
    


    // Emotes tab

    // Move settings here once implemented

    // Currently disabled settings
    BSML::Lite::CreateText(emoteTab, "Coming Soon", Vector2(0.0f, 0.0f), Vector2(1.0f, 5.0f))->horizontalAlignment = TMPro::HorizontalAlignmentOptions::Center;
    auto chatEmoteRainToggle = AddConfigValueToggle(emoteTab, getModConfig().ChatEmoteRain);
    auto chatEmoteRainMaxEmoteCountIncrement = AddConfigValueIncrementFloat(emoteTab, getModConfig().ChatEmoteRain_MaxEmoteCount, 0, 5.0f, 0.0f, 100.0f);
    auto chatEmoteRainEmoteSizeIncrement = AddConfigValueIncrementVector2(emoteTab, getModConfig().ChatEmoteRain_EmoteSize, 1, 0.1f);
    auto chatEmoteRainActivateCommandToggle = AddConfigValueToggle(emoteTab, getModConfig().ChatEmoteRain_ActivateCommand);
    auto chatEmoteRainModCommandToggle = AddConfigValueToggle(emoteTab, getModConfig().ChatEmoteRain_MODCommand);
    auto chatEmoteRainVipCommandToggle = AddConfigValueToggle(emoteTab, getModConfig().ChatEmoteRain_VIPCommand);
    auto chatEmoteRainSubCommandToggle = AddConfigValueToggle(emoteTab, getModConfig().ChatEmoteRain_SUBCommand);
    auto chatEmoteRainUserCommandToggle = AddConfigValueToggle(emoteTab, getModConfig().ChatEmoteRain_USERCommand);

    // Disable the UI for the disabled settings
    chatEmoteRainToggle->set_interactable(false);
    chatEmoteRainMaxEmoteCountIncrement->set_interactable(false);
    chatEmoteRainEmoteSizeIncrement[0]->set_interactable(false);
    chatEmoteRainEmoteSizeIncrement[1]->set_interactable(false);
    chatEmoteRainActivateCommandToggle->set_interactable(false);
    chatEmoteRainModCommandToggle->set_interactable(false);
    chatEmoteRainVipCommandToggle->set_interactable(false);
    chatEmoteRainSubCommandToggle->set_interactable(false);
    chatEmoteRainUserCommandToggle->set_interactable(false);

    

    // API tab

    AddConfigValueToggle(apiTab, getModConfig().WebsocketServer_Enabled);
    AddConfigValueToggle(apiTab, getModConfig().WebsocketServer);



    // Credits tab
    BSML::Lite::CreateText(creditsTab, "", Vector2(0.0f, 0.0f), Vector2(1.0f, 7.0f)); // Spacer
    BSML::Lite::CreateText(creditsTab, "darknight1050", Vector2(0.0f, 0.0f), Vector2(1.0f, 10.0f))->fontSize = 8.0f;
    BSML::Lite::CreateText(creditsTab, "vcmikuu", Vector2(0.0f, 0.0f), Vector2(1.0f, 10.0f))->fontSize = 8.0f;
    BSML::Lite::CreateText(creditsTab, "FrozenAlex", Vector2(0.0f, 0.0f), Vector2(1.0f, 10.0f))->fontSize = 8.0f;
    BSML::Lite::CreateText(creditsTab, "DanTheMan827", Vector2(0.0f, 0.0f), Vector2(1.0f, 10.0f))->fontSize = 8.0f;
    BSML::Lite::CreateText(creditsTab, "CatsaCode", Vector2(0.0f, 0.0f), Vector2(1.0f, 10.0f))->fontSize = 8.0f;
}