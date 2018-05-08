/* input.hh */
#ifndef INPUT_HH
#define INPUT_HH

/* The controls are as follows:
 * [+]           :    Zoom in
 * [-]           :    Zoom out
 * [UPARROW/W]   :    Move up
 * [RIGHTARROW/D]:    Move right
 * [DOWNARROW/S] :    Move down
 * [LEFTARROW/A] :    Move left
 * [R]           :    Render again
 * [Z]           :    Toggle fractal type (next)
 * [X]           :    Toggle fractal type (previous)
 * [H]           :    Toggle help display
 * [G]           :    Toggle debug display
 * [LEFTCTRL]    :    Toggle interface display
 * [O]           :    Decrement iterations
 * [I]           :    Increment iterations
 * [E]           :    Decrement threads
 * [Q]           :    Increment threads
 * [SPACE]       :    Take screenshot
 * [F11]         :    Toggle fullscreen
 */
namespace input 
{
	void *freeze(void *);
	void poll(void);
}

#endif /* INPUT_HH */
