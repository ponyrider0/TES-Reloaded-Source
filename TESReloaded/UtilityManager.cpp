#if defined(OBLIVION)
#include "obse\GameData.h"
#elif defined(SKYRIM)
#include "skse\GameMenus.h"
#endif

UtilityManager::UtilityManager()
{
	
	_MESSAGE("Starting the utility manager...");
	Screenshot = false;

}

float UtilityManager::GetGameTime()
{

#if defined(OBLIVION)
	return TimeGlobals::GameHour() * 60.0f * 60.0f;
#elif defined(SKYRIM)
	class TimeGlobals
	{
	public:
		UInt32		unk00;			// 00
		TESGlobal*	GameYear;		// 04
		TESGlobal*	GameMonth;		// 08
		TESGlobal*	GameDay;		// 0C
		TESGlobal*	GameHour;		// 10
		TESGlobal*	GameDaysPassed;	// 14
		TESGlobal*	TimeScale;		// 18
		UInt32		unk1C;			// 1C
		UInt32		unk20;			// 20
	};
	TimeGlobals* Globals = (TimeGlobals*)0x012E5690;
	return Globals->GameHour->data * 60.0f * 60.0f;
#endif

}

bool UtilityManager::IsMenuMode(int MenuType)
{
	
#if defined(OBLIVION)
	bool (__cdecl * MenuMode)(int) = (bool(*)(int))0x0057C140;
	return MenuMode(MenuType);
#elif defined(SKYRIM)
	if (MenuType == 1009)
		return MenuManager::GetSingleton()->IsMenuOpen(&UIStringHolder::GetSingleton()->dialogueMenu);
	else
		return false;
#endif

}

bool UtilityManager::IsVanityView()
{

#if defined(OBLIVION)
	return *(bool*)0x00B3BB04;
#elif defined(SKYRIM)
	return false;
#endif

}

void UtilityManager::MatrixMultiply(NiMatrix33* ss, NiMatrix33* sm1, NiMatrix33* sm2) {

	NiMatrix33L* s = (NiMatrix33L*)ss;
	NiMatrix33L* m1 = (NiMatrix33L*)sm1;
	NiMatrix33L* m2 = (NiMatrix33L*)sm2;

	for (int y = 0; y < 3; y++) {
		for (int x = 0; x < 3; x++) {
			s->data[3 * y + x] = 0;
			for (int i = 0; i < 3; i++) {
				s->data[3 * y + x] += m1->data[3 * y + i] * m2->data[3 * i + x];
			}
		}
	}

}

void UtilityManager::MatrixVectorMultiply(NiPoint3* r, NiMatrix33* sm, NiPoint3* v) {
	
	NiMatrix33L* m = (NiMatrix33L*)sm;

	r->x = m->data[0] * v->x + m->data[1] * v->y + m->data[2] * v->z;
	r->y = m->data[3] * v->x + m->data[4] * v->y + m->data[5] * v->z;
	r->z = m->data[6] * v->x + m->data[7] * v->y + m->data[8] * v->z;

}

void UtilityManager::GenerateRotationMatrixZXY(NiMatrix33* sm, float a, float b, float c, bool degrees) {
	
	NiMatrix33L* m = (NiMatrix33L*)sm;

	if (degrees) {
		a = DegreesToRadians(a);
		b = DegreesToRadians(b);
		c = DegreesToRadians(c);
	}
	m->data[0] = cos(a) * cos(c) - sin(a) * sin(b) * sin(c);
	m->data[1] = -cos(b) * sin(a);
	m->data[2] = cos(a) * sin(c) + cos(c) * sin(a) * sin(b);
	m->data[3] = cos(c) * sin(a) + cos(a) * sin(b) * sin(c);
	m->data[4] = cos(a) * cos(b);
	m->data[5] = sin(a) * sin(c) - cos(a) * cos(c) * sin(b);
	m->data[6] = -cos(b) * sin(c);
	m->data[7] = sin(b);
	m->data[8] = cos(b) * cos(c);

}

void UtilityManager::GenerateRotationMatrixXYZ(NiMatrix33* sm, float a, float b, float c, bool degrees) {
	
	NiMatrix33L* m = (NiMatrix33L*)sm;

	if (degrees) {
		a = DegreesToRadians(a);
		b = DegreesToRadians(b);
		c = DegreesToRadians(c);
	}
	m->data[0] = cos(b) * cos(c);
	m->data[1] = -cos(b) * sin(c);
	m->data[2] = sin(b);
	m->data[3] = cos(a) * sin(c) + cos(c) * sin(a) * sin(b);
	m->data[4] = cos(a) * cos(c) - sin(a) * sin(b) * sin(c);
	m->data[5] = -cos(b) * sin(a);
	m->data[6] = sin(a) * sin(c) - cos(a) * cos(c) * sin(b);
	m->data[7] = cos(c) * sin(a) + cos(a) * sin(b) * sin(c);
	m->data[8] = cos(a) * cos(b);

}

float UtilityManager::RadiansToDegrees(float radians)
{

	return radians * 180 / M_PI;

}

float UtilityManager::DegreesToRadians(float degrees)
{

	return degrees * M_PI / 180;

}

void  UtilityManager::NormalizeVector3(D3DXVECTOR4 &vector) {
	
	float len = sqrt((vector.x * vector.x) + (vector.y * vector.y) + (vector.z * vector.z));
	if (len) {
		vector.x = vector.x / len;
		vector.y = vector.y / len;
		vector.z = vector.z / len;
	}
	else {
		vector.x = 0.0f;
		vector.y = 0.0f;
		vector.z = 1.0f;
	}

}