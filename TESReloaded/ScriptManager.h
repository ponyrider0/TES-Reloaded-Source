#pragma once
#if defined(OBLIVION)
class BaseScript
{
public:
	BaseScript();

	virtual void	Run() = 0;

	float			ElapsedTime;
};

class LowHSoundScript : public BaseScript // Never disposed
{
public:
	LowHSoundScript();

	void			Run();

	TESSound*		HeartSlow;
};

class LowFSoundScript : public BaseScript // Never disposed
{
public:
	LowFSoundScript();

	void			Run();

	TESSound*		BreathingF;
	TESSound*		BreathingM;
	float			BreathingTime;
};

class PurgerScript : public BaseScript // Never disposed
{
public:
	PurgerScript();

	void			Run();
};

class ScriptManager // Never disposed
{
public:
	ScriptManager();

	void				Run();

	LowHSoundScript*	LowHSound;
	LowFSoundScript*	LowFSound;
	PurgerScript*		Purger;
};
#elif defined(SKYRIM)
class ScriptManager // Never disposed
{
public:
	ScriptManager();
};
#endif