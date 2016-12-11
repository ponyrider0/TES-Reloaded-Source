#if defined(OBLIVION)
#include "obse\CommandTable.h"
#elif defined(SKYRIM)
#include "skse\CommandTable.h"
#endif

static ParamInfo kParams_TwoInts[2] =
{
	{ "int", kParamType_Integer, 0 },
	{ "int", kParamType_Integer, 0 },
};

static ParamInfo kParams_TwoString[2] =
{
	{ "string",	kParamType_String, 0 },
	{ "string",	kParamType_String, 0 },
};

static ParamInfo kParams_TwoString_FourFloat[6] =
{
	{ "string",	kParamType_String, 0 },
	{ "string",	kParamType_String, 0 },
	{ "float",  kParamType_Float,  0 },
	{ "float",  kParamType_Float,  0 },
	{ "float",  kParamType_Float,  0 },
	{ "float",  kParamType_Float,  0 },
};

static ParamInfo kParams_OneObject[1] =
{
	{ "object",	kParamType_TESObject, 0 },
};

static ParamInfo kParams_ThreeFloats[3] =
{
	{ "float", kParamType_Float, 0 },
	{ "float", kParamType_Float, 0 },
	{ "float", kParamType_Float, 0 },
};

static ParamInfo kParams_OneActorThreeFloats[4] =
{
	{ "objectRef", kParamType_ObjectRef, 0 },
	{ "float", kParamType_Float, 0 },
	{ "float", kParamType_Float, 0 },
	{ "float", kParamType_Float, 0 },
};

static ParamInfo kParams_FourIntsOneString[5] =
{
	{ "int", kParamType_Integer, 0 },
	{ "int", kParamType_Integer, 0 },
	{ "int", kParamType_Integer, 0 },
	{ "int", kParamType_Integer, 0 },
	{ "string",	kParamType_String,	0 },
};