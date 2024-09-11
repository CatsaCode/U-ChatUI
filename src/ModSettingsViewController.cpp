#include "ModSettingsViewController.hpp"

#include "bsml/shared/BSML.hpp"

#include "HMUI/ViewController.hpp"
#include "HMUI/Touchable.hpp"

#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/Vector2.hpp"
#include "UnityEngine/Vector3.hpp"
#include "UnityEngine/Component.hpp"
#include "UnityEngine/Color.hpp"
#include "UnityEngine/RectTransform.hpp"
#include "UnityEngine/UI/HorizontalLayoutGroup.hpp"
#include "UnityEngine/UI/ContentSizeFitter.hpp"
#include "UnityEngine/UI/Image.hpp"

#include "System/Type.hpp"

#include "logging.hpp"
#include "ModConfig.hpp"

using namespace BSML;
using namespace UnityEngine;
using namespace UnityEngine::UI;
using namespace HMUI;

void InfoTree(GameObject* object) {
    std::vector<GameObject*> remainingObjects = {object};

    while(!remainingObjects.empty()) {
        GameObject* object = remainingObjects[0];

        int parentCount = 0;
        Transform* tmpTransform = object->transform;
        while(tmpTransform != nullptr) {
            tmpTransform = tmpTransform->parent;
            parentCount++;
        }

        INFO("{}G: {}", std::string(' ', parentCount * 2), object->name);

        ArrayW<Component*> components = object->GetComponents<Component*>();
        for(int i = 0; i < components->get_Length(); i++) {
            INFO("{}C: {}", std::string(' ', parentCount * 2 + 1), components[i]->GetType()->ToString());
        }

        for(int i = 0; i < object->transform->childCount; i++) remainingObjects.push_back(object->transform->GetChild(i)->gameObject);
        remainingObjects.erase(remainingObjects.begin());
    }
    // ArrayW<Component*> components = object->GetComponents<Component*>();
    // object->transform->childCount;
    // INFO("G: {}", object->name);
    // for(int i = 0; i < components->get_Length(); i++) {
    //     INFO("C: {}", components[i]->GetType()->ToString());
    // }
}

void HighlightUIComponent(GameObject* object, Color color = Color(1.0f, 1.0f, 1.0f, 1.0f)) {
    auto image = object->AddComponent<Image*>();
    image->color = color;
}

// Create a row of buttons directly side by side 
HorizontalLayoutGroup* CreateTabSelector(const BSML::Lite::TransformWrapper& parent, std::vector<std::string> labels, float width, std::function<void(int)> selectionFunc) {
    // Create a horizontal layout to automatically space out each tab button
    auto tabsRow = BSML::Lite::CreateHorizontalLayoutGroup(parent);
    // Set tabsRow to be a specific width
    tabsRow->GetComponent<ContentSizeFitter*>()->horizontalFit = ContentSizeFitter::FitMode::PreferredSize;
    tabsRow->GetComponent<LayoutElement*>()->preferredWidth = width;

    // Create a tab for each label
    for(int i = 0; i < labels.size(); i++) {
        // Create a button that will call selectionFunc with its index
        auto tabButton = BSML::Lite::CreateUIButton(tabsRow, labels[i], [selectionFunc, i](){if(selectionFunc != nullptr) selectionFunc(i);});
        // Make each button fill space evenly
        tabButton->GetComponent<ContentSizeFitter*>()->horizontalFit = ContentSizeFitter::FitMode::Unconstrained;
    }

    return tabsRow;
}

void DidActivate(ViewController* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
    // Only create UI when first loaded
    if(!firstActivation) return;

    // Container for the entire mod menu  
    auto mainContainer = BSML::Lite::CreateVerticalLayoutGroup(self->transform);
    mainContainer->childControlHeight = false;
    mainContainer->childForceExpandHeight = false;
    mainContainer->spacing = 1.0f;

    auto tabsRow = CreateTabSelector(mainContainer, {"General", "Panel", "Emote Rain"}, 90.0f, [](int selection){
        if(selection == 0) INFO("Tab: General");
        else if(selection == 1) INFO("Tab: Panel");
        else INFO("Tab: Emote Rain");
    });

    // InfoTree(tabsRow->gameObject);

    // // self->get_gameObject()->AddComponent<Touchable*>();

    // GameObject* container = BSML::Lite::CreateScrollableSettingsContainer(self->get_transform());
    // Transform* parent = container->get_transform();

    // auto layout = BSML::Lite::CreateHorizontalLayoutGroup(parent);
    // // layout->GetComponent<LayoutElement*>()->set_preferredWidth(50.0f);
    // // layout->set_childControlWidth(true);
    // auto layoutParent = layout->get_transform();
    
    // auto stringSetting = AddConfigValueInputString(layoutParent, getModConfig().Channel);

    // //Use underline from button because text doesn't work somehow with this width
    // auto underscoreButton = BSML::Lite::CreateUIButton(layoutParent, "", UnityEngine::Vector2(0.0f, 0.0f), UnityEngine::Vector2(4.0f, 8.0f), [stringSetting] {
    //     stringSetting->KeyboardKeyPressed('_');
    //     stringSetting->UpdateClearButton();
    // });
    // BSML::Lite::AddHoverHint(underscoreButton->get_gameObject(), "Adds a underscore");
    // // LayoutElement* layoutElement = underscoreButton->GetComponent<LayoutElement*>();
    // // layoutElement->set_minWidth(4.0f);

    // AddConfigValueIncrementVector3(parent, getModConfig().PositionMenu, 2, 0.05f);
    // AddConfigValueIncrementVector3(parent, getModConfig().RotationMenu, 0, 1.0f);
    // AddConfigValueIncrementVector2(parent, getModConfig().SizeMenu, 0, 1.0f);

    // AddConfigValueToggle(parent, getModConfig().ForceGame);

    // AddConfigValueIncrementVector3(parent, getModConfig().PositionGame, 2, 0.05f);
    // AddConfigValueIncrementVector3(parent, getModConfig().RotationGame, 0, 1.0f);
    // AddConfigValueIncrementVector2(parent, getModConfig().SizeGame, 0, 1.0f);
}