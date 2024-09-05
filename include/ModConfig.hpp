#pragma once
#include "config-utils/shared/config-utils.hpp"

#include "UnityEngine/Vector3.hpp"
#include "UnityEngine/Vector2.hpp"

DECLARE_CONFIG(ModConfig,

    CONFIG_VALUE(Channel, std::string, "Twitch Channel", "darknight1050");

    CONFIG_VALUE(PositionMenu, UnityEngine::Vector3, "Menu Position", UnityEngine::Vector3(0.0f, 4.4f, 4.0f));
    CONFIG_VALUE(RotationMenu, UnityEngine::Vector3, "Menu Rotation", UnityEngine::Vector3(-42.0f, 0.0f, 0.0f));
    CONFIG_VALUE(SizeMenu, UnityEngine::Vector2, "Menu Size", UnityEngine::Vector2(68.0f, 88.0f));

    CONFIG_VALUE(ForceGame, bool, "Force Game Values in Menu", false);

    CONFIG_VALUE(PositionGame, UnityEngine::Vector3, "Game Position", UnityEngine::Vector3(0.0f, 4.4f, 4.0f));
    CONFIG_VALUE(RotationGame, UnityEngine::Vector3, "Game Rotation", UnityEngine::Vector3(-42.0f, 0.0f, 0.0f));
    CONFIG_VALUE(SizeGame, UnityEngine::Vector2, "Game Size", UnityEngine::Vector2(68.0f, 88.0f));

)