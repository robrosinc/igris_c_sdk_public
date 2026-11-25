"""Shared presentation helpers for the Python examples.

Deliberately tiny: the examples are read as much as they are run, so this carries only what was
being duplicated across them - the startup banner and the in-place redraw escape. Mirrors
examples/cyclonedds/common.hpp and examples/ros2/src/common.hpp, which play the same role on the
two C++ lanes. public.sh rsyncs this directory wholesale, so this ships with the deliverable.
"""

from __future__ import annotations

import contextlib
import sys
from collections.abc import Iterator

# Clear the screen and park the cursor at the top-left.
#
# For examples that reprint the same block forever: writing it again below the last one grows the
# scrollback without bound, which makes the terminal unusable long before the data gets
# interesting. Callers redraw the banner too, since this wipes it.
REDRAW = "\x1b[2J\x1b[H"


def print_banner(title: str, domain_id: int, namespace: str, topic_naming: str) -> None:
    """Print the one startup block every example shares.

    A reader should be able to tell at a glance which robot a run is talking to. The two ways an
    example sits silent while everything "looks" fine are a domain that does not match the robot
    and a topic layout that does not match it either - neither is an error, both are invisible,
    so both are printed rather than left to be guessed.
    """
    sys.stdout.write(f"=== IGRIS-C SDK {title} (Python) ===\n")
    sys.stdout.write(f"domain id      : {domain_id}\n")
    sys.stdout.write(f"namespace      : {namespace if namespace else '(none)'}\n")
    sys.stdout.write(f"topic naming   : {topic_naming}\n\n")
    sys.stdout.flush()


@contextlib.contextmanager
def alt_screen() -> Iterator[None]:
    """Run a redrawing example on the terminal's ALTERNATE screen, like less/vim/ncurses do.

    REDRAW alone is not enough. It erases what is displayed but does not touch the scrollback, so
    every frame is still stacked up there - scrolling back after a minute shows sixty copies of
    the same block - and whatever the last frame happened to be is left sitting in the terminal
    after Ctrl+C, on top of whatever the user had before. The alternate screen has no scrollback
    of its own and is discarded on exit, so the shell comes back exactly as it was.

    A context manager rather than a pair of calls: the restore has to run on every exit path,
    KeyboardInterrupt included. Skipped when stdout is not a terminal, so a piped or redirected
    run writes plain text instead of escape sequences.
    """
    active = sys.stdout.isatty()
    if active:
        sys.stdout.write("\x1b[?1049h\x1b[?25l")  # enter alt screen, hide cursor
        sys.stdout.flush()
    try:
        yield
    finally:
        if active:
            sys.stdout.write("\x1b[?25h\x1b[?1049l")  # show cursor, restore screen
            sys.stdout.flush()
