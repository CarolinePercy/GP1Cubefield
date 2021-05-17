enum class State
{
	Gameplay,
	StartScreen,
	Dying,
	Pause
};

class ScreenController
{
public:
	static State screen;
};