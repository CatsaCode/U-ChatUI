#include "HMUI/ViewController.hpp"
#include "HMUI/Touchable.hpp"
#include "bsml/shared/BSML.hpp"
#include "UnityEngine/UI/ContentSizeFitter.hpp"
#include "UnityEngine/RectOffset.hpp"
#include "UnityEngine/RectTransform.hpp"
#include "UnityEngine/UI/RectMask2D.hpp"
#include "HMUI/CurvedCanvasSettings.hpp"
#include "TMPro/TextMeshProUGUI.hpp"
#include "VRUIControls/VRGraphicRaycaster.hpp"
#include "bsml/shared/BSML/Components/Backgroundable.hpp"

#include "ChatBuilder.hpp"
#include "logging.hpp"

using namespace BSML;
using namespace UnityEngine;
using namespace UnityEngine::UI;
using namespace HMUI;
using namespace TMPro;

// this is only for the song request button

//void CreateSongRequestButton() {
//    UnityEngine::UI::HorizontalLayoutGroup* layout = BSML::Lite::CreateHorizontalLayoutGroup(layout->get_transform());
//    Button* button = BSML::Lite::CreateUIButton(layout->get_transform(), "", "Chat Requests", nullptr);
//    button->get_transform()->set_localScale({2.0f, 3.0f, 2.0f});
//    button->set_interactable(false);
//
//
//}