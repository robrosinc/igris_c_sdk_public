/**
 * @file common.hpp
 * @brief Shared presentation helpers for the Cyclone DDS examples.
 *
 * Header-only and deliberately small: carries only what was being duplicated across the
 * examples - the startup banner, the in-place redraw escape, and the alternate-screen guard.
 * Mirrors examples/ros2/src/common.hpp, which plays the same role on the ROS lane.
 */
#pragma once

// Shared presentation helpers for the Cyclone DDS examples.
//
// Header-only and deliberately tiny: the examples are read as much as they are run, so this
// carries only what was being duplicated seven times - the startup banner and the in-place
// redraw escape. Mirrors examples/ros2/src/common.hpp, which plays the same role on the ROS
// lane. public.sh rsyncs this directory wholesale, so this ships with the deliverable.

#include <cstdio>
#include <igris_c_sdk/namespace_resolver.hpp>
#include <iostream>
#include <string>
#include <unistd.h>

namespace igris_c_sdk_examples {

// Clear the screen and park the cursor at the top-left.
//
// For examples that reprint the same block forever: writing it again below the last one grows
// the scrollback without bound, which makes the terminal unusable long before the data gets
// interesting. Callers redraw the banner too, since this wipes it.
inline constexpr const char *kRedraw = "\x1B[2J\x1B[H";

// Run a redrawing example on the terminal's ALTERNATE screen, like less/vim/ncurses do.
//
// kRedraw alone is not enough. It erases what is displayed but does not touch the scrollback,
// so every frame is still stacked up there - scrolling back after a minute shows sixty copies
// of the same block - and whatever the last frame happened to be is left sitting in the
// terminal after Ctrl+C, on top of whatever the user had before. The alternate screen has no
// scrollback of its own and is discarded on exit, so the shell comes back exactly as it was.
//
// RAII rather than a pair of calls: every exit path has to restore, including the ones that
// return early. Guarded on isatty so a piped or redirected run writes plain text instead of
// escape sequences.
class AltScreen {
  public:
    AltScreen() : active_(::isatty(::fileno(stdout)) != 0) {
        if (active_) {
            std::fputs("\x1B[?1049h\x1B[?25l", stdout);  // enter alt screen, hide cursor
            std::fflush(stdout);
        }
    }
    ~AltScreen() {
        if (active_) {
            std::fputs("\x1B[?25h\x1B[?1049l", stdout);  // show cursor, restore screen
            std::fflush(stdout);
        }
    }
    AltScreen(const AltScreen &)            = delete;
    AltScreen &operator=(const AltScreen &) = delete;

  private:
    bool active_;
};

// The one startup block every example prints, so a reader can tell at a glance which robot a
// run is talking to. The two ways an example sits silent while everything "looks" fine are a
// domain that does not match the robot and a topic layout that does not match it either -
// neither is an error, both are invisible, so both are printed rather than left to be guessed.
inline void printBanner(const std::string &title, int domain_id, const std::string &ns, igris_c_sdk::TopicNaming naming) {
    std::cout << "=== IGRIS-C SDK " << title << " (Cyclone DDS) ===\n";
    std::cout << "domain id      : " << domain_id << "\n";
    std::cout << "namespace      : " << (ns.empty() ? "(none)" : ns) << "\n";
    std::cout << "topic naming   : " << igris_c_sdk::to_config_string(naming) << "\n";
    std::cout << std::endl;
}

}  // namespace igris_c_sdk_examples
