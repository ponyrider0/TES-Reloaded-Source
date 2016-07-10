class EventManager
{

public:
	EventManager();
	~EventManager();

	static EventManager*	GetSingleton();
	static EventManager*	Singleton;
	void					OnHitByPlayer();

	//TESObjectREFR*			Target;
	TESObjectWEAP*			Weapon;

};

void CreateEventHook();