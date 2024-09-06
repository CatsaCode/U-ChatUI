#pragma once
#include "config-utils/shared/config-utils.hpp"

#include "UnityEngine/Vector3.hpp"
#include "UnityEngine/Vector2.hpp"

DECLARE_CONFIG(ModConfig,

    // Values for plugin CHAT

    CONFIG_VALUE(Channel, std::string, "Twitch Channel", "darknight1050");

    CONFIG_VALUE(PositionMenu, UnityEngine::Vector3, "Menu Position", UnityEngine::Vector3(0.0f, 4.4f, 4.0f));
    CONFIG_VALUE(RotationMenu, UnityEngine::Vector3, "Menu Rotation", UnityEngine::Vector3(-42.0f, 0.0f, 0.0f));
    CONFIG_VALUE(SizeMenu, UnityEngine::Vector2, "Menu Size", UnityEngine::Vector2(68.0f, 88.0f));

    CONFIG_VALUE(ForceGame, bool, "Force Game Values in Menu", false);

    CONFIG_VALUE(PositionGame, UnityEngine::Vector3, "Game Position", UnityEngine::Vector3(0.0f, 4.4f, 4.0f));
    CONFIG_VALUE(RotationGame, UnityEngine::Vector3, "Game Rotation", UnityEngine::Vector3(-42.0f, 0.0f, 0.0f));
    CONFIG_VALUE(SizeGame, UnityEngine::Vector2, "Game Size", UnityEngine::Vector2(68.0f, 88.0f));

    CONFIG_VALUE(FontSizeChat, UnityEngine::Vector2, "Font Size", UnityEngine::Vector2(3.0f));
    CONFIG_VALUE(BackGroundTransChat, UnityEngine::Vector2, "Background Transparency", UnityEngine::Vector2(3.0f));

    // Not for V2 Update

    CONFIG_VALUE(Youtube_Channel, std::string, "Youtube Channel", "Youtube");
    CONFIG_VALUE(TwitchYoutube_ShowViewers, bool, "Show current viewers", false);

    // Values for V3 Update
    // I would say just make it deactivated so the people can't use it
    // Why not hide it? I just wanna show what I'm planning to do ;))

    CONFIG_VALUE(Chat, bool, "Chat", true);
    CONFIG_VALUE(ChatIntegrations, bool, "ChatIntegrations", false);
    CONFIG_VALUE(ChatEmoteRain, bool, "ChatEmoteRain", false);
    CONFIG_VALUE(SongOverlay, bool, "SongOverlay", false);
    CONFIG_VALUE(WebsocketServer, bool, "WebsocketServer", false);

    // Values for SongOverlay

    CONFIG_VALUE(SongOverlay_Enabled, bool, "Enable Song Overlay", false);

    // Values for ChatEmoteRain

    CONFIG_VALUE(ChatEmoteRain_MaxEmoteCount, float, "Max Emote Count", 20);
    CONFIG_VALUE(ChatEmoteRain_EmoteSize, UnityEngine::Vector2, "Emote Size", UnityEngine::Vector2(4.0f));
    CONFIG_VALUE(ChatEmoteRain_ActivateCommand, bool, "Activate !er command", false);
    CONFIG_VALUE(ChatEmoteRain_MODCommand, bool, "Give permissions to MOD", false);
    CONFIG_VALUE(ChatEmoteRain_VIPCommand, bool, "Give permissions to VIP", false);
    CONFIG_VALUE(ChatEmoteRain_SUBCommand, bool, "Give permissions to SUB", false);
    CONFIG_VALUE(ChatEmoteRain_USERCommand, bool, "Give permissions to USER", false);

    // Values for WebsocketServer

    CONFIG_VALUE(WebsocketServer_Enabled, bool, "Enable Websocket Server", false);




)