#include <string>
#include <sstream>
#include <fstream>
#include "Hooking\detours\detours.h"
#include "SettingManager.h"

template <typename T> std::string ToString(const T& Value) {

	std::ostringstream os;

	os << Value;
	return " " + os.str();

}

SettingManager::SettingManager() {

	char* Filename = SettingsMain.MainSettingsFullFile;
	char value[255];
	
	_MESSAGE("Starting the settings manager...");

	GetCurrentDirectoryA(MAX_PATH,CurrentPath);
	strcpy(Filename, CurrentPath);
	strcat(Filename, MainSettingsFile);

	SettingsMain.WindowedMode = GetPrivateProfileIntA("Main","WindowedMode",1,Filename);
	GetPrivateProfileStringA("Main","FoV","90.0",value,255,Filename);
	SettingsMain.FoV = atof(value);
	SettingsMain.WaterReflectionMapSize = GetPrivateProfileIntA("Main","WaterReflectionMapSize",512,Filename);
	SettingsMain.WaterManagement = GetPrivateProfileIntA("Main","WaterManagement",1,Filename);
	GetPrivateProfileStringA("Main","FarPlaneDistance","0.0",value,255,Filename);
	SettingsMain.FarPlaneDistance = atof(value);
	GetPrivateProfileStringA("Main","ScreenshotPath",CurrentPath,value,255,Filename);
	if (value[0] == '\\') {
		strcpy(SettingsMain.ScreenshotPath,CurrentPath);
		strcat(SettingsMain.ScreenshotPath,value);
	}
	else
		strcpy(SettingsMain.ScreenshotPath,value);
	if (SettingsMain.ScreenshotPath[strlen(SettingsMain.ScreenshotPath)-1] != '\\')
		strcat(SettingsMain.ScreenshotPath, "\\");
	SettingsMain.ScreenshotType = GetPrivateProfileIntA("Main","ScreenshotType",1,Filename);
	SettingsMain.CustomEffects = GetPrivateProfileIntA("Main","CustomEffects",0,Filename);
	SettingsMain.FrameRate = GetPrivateProfileIntA("Main","FrameRate",0,Filename);
	SettingsMain.SaveSettings = GetPrivateProfileIntA("Main","SaveSettings",1,Filename);
	SettingsMain.CombatMode = GetPrivateProfileIntA("Main","CombatMode",0,Filename);
	SettingsMain.CameraMode = GetPrivateProfileIntA("Main","CameraMode",0,Filename);
	SettingsMain.EquipmentMode = GetPrivateProfileIntA("Main","EquipmentMode",0,Filename);
	SettingsMain.SleepingMode = GetPrivateProfileIntA("Main","SleepingMode",0,Filename);
	SettingsMain.GrassMode = GetPrivateProfileIntA("Main", "GrassMode", 0, Filename);

	SettingsMain.FrameRateAverage = GetPrivateProfileIntA("FrameRate","Average",30,Filename);
	SettingsMain.FrameRateGap = GetPrivateProfileIntA("FrameRate","Gap",3,Filename);
	SettingsMain.FrameRateDelay = GetPrivateProfileIntA("FrameRate","Delay",10,Filename);
	GetPrivateProfileStringA("FrameRate","FadeStep","0.5",value,255,Filename);
	SettingsMain.FrameRateFadeStep = atof(value);
	SettingsMain.FrameRateFadeMinObjects = GetPrivateProfileIntA("FrameRate","FadeMinObjects",10,Filename);
	SettingsMain.FrameRateFadeMinActors = GetPrivateProfileIntA("FrameRate","FadeMinActors",15,Filename);
	SettingsMain.FrameRateGridStep = GetPrivateProfileIntA("FrameRate","GridStep",2,Filename);
	SettingsMain.FrameRateGridMin = GetPrivateProfileIntA("FrameRate","GridMin",5,Filename);

	SettingsMain.CameraModeHUDReticle = GetPrivateProfileIntA("CameraMode","HUDReticle",2,Filename);
	SettingsMain.CameraModeChasingFirst = GetPrivateProfileIntA("CameraMode","ChasingFirst",0,Filename);
	SettingsMain.CameraModeChasingThird = GetPrivateProfileIntA("CameraMode","ChasingThird",0,Filename);
	GetPrivateProfileStringA("CameraMode","OffsetX","0.0",value,255,Filename);
	SettingsMain.CameraModeOffset.x = atof(value);
	GetPrivateProfileStringA("CameraMode","OffsetY","16.0",value,255,Filename);
	SettingsMain.CameraModeOffset.y = atof(value);
	GetPrivateProfileStringA("CameraMode","OffsetZ","3.0",value,255,Filename);
	SettingsMain.CameraModeOffset.z = atof(value);
	GetPrivateProfileStringA("CameraMode","NearDistanceFirst","2.0",value,255,Filename);
	SettingsMain.CameraModeNearDistanceFirst = atof(value);
	GetPrivateProfileStringA("CameraMode","NearDistanceThird","8.0",value,255,Filename);
	SettingsMain.CameraModeNearDistanceThird = atof(value);
	SettingsMain.CameraModeDialogFirst = GetPrivateProfileIntA("CameraMode","DialogFirst",1,Filename);
	SettingsMain.CameraModeDialogThird = GetPrivateProfileIntA("CameraMode","DialogThird",2,Filename);
	GetPrivateProfileStringA("CameraMode","DialogOffsetX","50.0",value,255,Filename);
	SettingsMain.CameraModeDialogOffset.x = atof(value);
	GetPrivateProfileStringA("CameraMode","DialogOffsetY","-10.0",value,255,Filename);
	SettingsMain.CameraModeDialogOffset.y = atof(value);
	GetPrivateProfileStringA("CameraMode","DialogOffsetZ","10.0",value,255,Filename);
	SettingsMain.CameraModeDialogOffset.z = atof(value);

	SettingsMain.SleepingModeRest = GetPrivateProfileIntA("SleepingMode","Rest",1,Filename);
	GetPrivateProfileStringA("SleepingMode","RestMessage","You must be in a bed to rest.",value,255,Filename);
	strcpy(SettingsMain.SleepingModeRestMessage,value);

	SettingsMain.EnableWater = GetPrivateProfileIntA("Shaders","EnableWater",0,Filename);
	SettingsMain.EnableGrass = GetPrivateProfileIntA("Shaders","EnableGrass",0,Filename);
	SettingsMain.EnablePrecipitations = GetPrivateProfileIntA("Shaders","EnablePrecipitations",0,Filename);
	SettingsMain.EnableHDR = GetPrivateProfileIntA("Shaders","EnableHDR",0,Filename);
	SettingsMain.EnablePOM = GetPrivateProfileIntA("Shaders","EnablePOM",0,Filename);
	SettingsMain.EnableSkin = GetPrivateProfileIntA("Shaders","EnableSkin",0,Filename);
	SettingsMain.EnableTerrain = GetPrivateProfileIntA("Shaders","EnableTerrain",0,Filename);
	SettingsMain.EnableBlood = GetPrivateProfileIntA("Shaders","EnableBlood",0,Filename);
	SettingsMain.EnableShadows = GetPrivateProfileIntA("Shaders","EnableShadows",0,Filename);

	SettingsMain.EnableUnderwater = GetPrivateProfileIntA("Effects","EnableUnderwater",0,Filename);
	SettingsMain.EnableWaterLens = GetPrivateProfileIntA("Effects","EnableWaterLens",0,Filename);
	SettingsMain.EnableGodRays = GetPrivateProfileIntA("Effects","EnableGodRays",0,Filename);
	SettingsMain.EnableDepthOfField = GetPrivateProfileIntA("Effects","EnableDepthOfField",0,Filename);
	SettingsMain.EnableAmbientOcclusion = GetPrivateProfileIntA("Effects","EnableAmbientOcclusion",0,Filename);
	SettingsMain.EnableColoring = GetPrivateProfileIntA("Effects","EnableColoring",0,Filename);
	SettingsMain.EnableCinema = GetPrivateProfileIntA("Effects","EnableCinema",0,Filename);
	SettingsMain.EnableBloom = GetPrivateProfileIntA("Effects","EnableBloom",0,Filename);
	SettingsMain.EnableSnowAccumulation = GetPrivateProfileIntA("Effects","EnableSnowAccumulation",0,Filename);
	SettingsMain.EnableBloodLens = GetPrivateProfileIntA("Effects","EnableBloodLens",0,Filename);
	SettingsMain.EnableMotionBlur = GetPrivateProfileIntA("Effects","EnableMotionBlur",0,Filename);
	SettingsMain.EnableLowHF = GetPrivateProfileIntA("Effects","EnableLowHF",0,Filename);
	SettingsMain.EnableWetWorld = GetPrivateProfileIntA("Effects","EnableWetWorld",0,Filename);
	SettingsMain.EnableSharpening = GetPrivateProfileIntA("Effects","EnableSharpening",0,Filename);
	SettingsMain.EnableSMAA = GetPrivateProfileIntA("Effects","EnableSMAA",0,Filename);

	SettingsMain.DevelopCompileShaders = GetPrivateProfileIntA("Develop","CompileShaders",0,Filename);
	SettingsMain.DevelopCompileEffects = GetPrivateProfileIntA("Develop", "CompileEffects", 0, Filename);
	SettingsMain.DevelopShadersFolder = GetPrivateProfileIntA("Develop","ShadersFolder",0,Filename);
	SettingsMain.DevelopTraceShaders = GetPrivateProfileIntA("Develop","TraceShaders",0,Filename);
	SettingsMain.DevelopTraceVanillaShaders = GetPrivateProfileIntA("Develop", "TraceVanillaShaders", 0, Filename);
	SettingsMain.DevelopTraceCulling = GetPrivateProfileIntA("Develop","TraceCulling",0,Filename);

	InitCamera();

	GameLoading = false;

}

void SettingManager::LoadSettings() {

	char Filename[MAX_PATH];
	char value[255];
	char Sections[65535];
	char* pNextSection = NULL;
	SettingsWaterStruct SW;
	SettingsDepthOfFieldStruct SD;
	SettingsAmbientOcclusionStruct SA;
	SettingsColoringStruct SC;
	SettingsBloomStruct SB;
	SettingsMotionBlurStruct SM;

	SettingsWater.clear();
	SettingsDepthOfField.clear();
	SettingsAmbientOcclusion.clear();
	SettingsColoring.clear();
	SettingsBloom.clear();
	SettingsMotionBlur.clear();

	strcpy(Filename, CurrentPath);
	strcat(Filename, SettingsPath);
	strcat(Filename,"Water\\Water.ini");
	GetPrivateProfileStringA("Default","IceEnabled","1",Water_IceEnabled,255,Filename);
	SW.IceEnabled = atof(Water_IceEnabled);
	GetPrivateProfileStringA("Default","choppiness","0.8",Water_choppiness,255,Filename);
	SW.choppiness = atof(Water_choppiness);
	GetPrivateProfileStringA("Default","waveWidth","1.8",Water_waveWidth,255,Filename);
	SW.waveWidth = atof(Water_waveWidth);
	GetPrivateProfileStringA("Default","waveSpeed","0.6",Water_waveSpeed,255,Filename);
	SW.waveSpeed = atof(Water_waveSpeed);
	GetPrivateProfileStringA("Default","reflectivity","0.8",Water_reflectivity,255,Filename);
	SW.reflectivity = atof(Water_reflectivity);
	GetPrivateProfileStringA("Default","causticsStrength","0.5",Water_causticsStrength,255,Filename);
	SW.causticsStrength = atof(Water_causticsStrength);
	GetPrivateProfileStringA("Default", "causticsStrengthS", "1.0", Water_causticsStrengthS, 255, Filename);
	SW.causticsStrengthS = atof(Water_causticsStrengthS);
	GetPrivateProfileStringA("Default","shoreFactor","1.2",Water_shoreFactor,255,Filename);
	SW.shoreFactor = atof(Water_shoreFactor);
	GetPrivateProfileStringA("Default","turbidity","0.72",Water_turbidity,255,Filename);
	SW.turbidity = atof(Water_turbidity);
	GetPrivateProfileStringA("Default","inScattCoeff","0.12",Water_inScattCoeff,255,Filename);
	SW.inScattCoeff = atof(Water_inScattCoeff);
	GetPrivateProfileStringA("Default","inExtCoeff_R","0.40",Water_inExtCoeff_R,255,Filename);
	SW.inExtCoeff_R = atof(Water_inExtCoeff_R);
	GetPrivateProfileStringA("Default","inExtCoeff_G","0.26",Water_inExtCoeff_G,255,Filename);
	SW.inExtCoeff_G = atof(Water_inExtCoeff_G);
	GetPrivateProfileStringA("Default","inExtCoeff_B","0.30",Water_inExtCoeff_B,255,Filename);
	SW.inExtCoeff_B = atof(Water_inExtCoeff_B);
	GetPrivateProfileStringA("Default","IceinExtCoeff_R","0.22",Water_IceinExtCoeff_R,255,Filename);
	SW.IceinExtCoeff_R = atof(Water_IceinExtCoeff_R);
	GetPrivateProfileStringA("Default","IceinExtCoeff_G","0.18",Water_IceinExtCoeff_G,255,Filename);
	SW.IceinExtCoeff_G = atof(Water_IceinExtCoeff_G);
	GetPrivateProfileStringA("Default","IceinExtCoeff_B","0.18",Water_IceinExtCoeff_B,255,Filename);
	SW.IceinExtCoeff_B = atof(Water_IceinExtCoeff_B);
	GetPrivateProfileStringA("Default","Icechoppiness","0.2",Water_Icechoppiness,255,Filename);
	SW.Icechoppiness = atof(Water_Icechoppiness);
	GetPrivateProfileStringA("Default","IcewaveWidth","3.6",Water_IcewaveWidth,255,Filename);
	SW.IcewaveWidth = atof(Water_IcewaveWidth);
	GetPrivateProfileStringA("Default","Icereflectivity","1.6",Water_Icereflectivity,255,Filename);
	SW.Icereflectivity = atof(Water_Icereflectivity);
	GetPrivateProfileStringA("Default","IceshoreFactor","0.4",Water_IceshoreFactor,255,Filename);
	SW.IceshoreFactor = atof(Water_IceshoreFactor);
	GetPrivateProfileStringA("Default","Iceturbidity","1.4",Water_Iceturbidity,255,Filename);
	SW.Iceturbidity = atof(Water_Iceturbidity);
	GetPrivateProfileStringA("Default","depthDarkness","0.1",Water_depthDarkness,255,Filename);
	SW.depthDarkness = atof(Water_depthDarkness);
	GetPrivateProfileStringA("Default","LODdistance","1.0",Water_LODdistance,255,Filename);
	SW.LODdistance = atof(Water_LODdistance);
	GetPrivateProfileStringA("Default","MinLOD","0.0",Water_MinLOD,255,Filename);
	SW.MinLOD = atof(Water_MinLOD);
	GetPrivateProfileStringA("Default","LensTimeMultA","0.1",Water_LensTimeMultA,255,Filename);
	SW.LensTimeMultA = atof(Water_LensTimeMultA);
	GetPrivateProfileStringA("Default","LensTimeMultB","0.2",Water_LensTimeMultB,255,Filename);
	SW.LensTimeMultB = atof(Water_LensTimeMultB);
	GetPrivateProfileStringA("Default","LensTime","80.0",Water_LensTime,255,Filename);
	SW.LensTime = atof(Water_LensTime);
	GetPrivateProfileStringA("Default","LensAmount","0.1",Water_LensAmount,255,Filename);
	SW.LensAmount = atof(Water_LensAmount);
	GetPrivateProfileStringA("Default","LensViscosity","0.05",Water_LensViscosity,255,Filename);
	SW.LensViscosity = atof(Water_LensViscosity);
	SettingsWater["Default"] = SW;

	GetPrivateProfileSectionNamesA(Sections,32767,Filename);
	pNextSection = Sections;
	while (*pNextSection != NULL)
	{
		if (strcmp(pNextSection,"Default") != 0) {
			GetPrivateProfileStringA(pNextSection,"IceEnabled",Water_IceEnabled,value,255,Filename);
			SW.IceEnabled = atof(value);
			GetPrivateProfileStringA(pNextSection,"choppiness",Water_choppiness,value,255,Filename);
			SW.choppiness = atof(value);
			GetPrivateProfileStringA(pNextSection,"waveWidth",Water_waveWidth,value,255,Filename);
			SW.waveWidth = atof(value);
			GetPrivateProfileStringA(pNextSection,"waveSpeed",Water_waveSpeed,value,255,Filename);
			SW.waveSpeed = atof(value);
			GetPrivateProfileStringA(pNextSection,"reflectivity",Water_reflectivity,value,255,Filename);
			SW.reflectivity = atof(value);
			GetPrivateProfileStringA(pNextSection,"causticsStrength",Water_causticsStrength,value,255,Filename);
			SW.causticsStrength = atof(value);
			GetPrivateProfileStringA(pNextSection, "causticsStrengthS", Water_causticsStrengthS, value, 255, Filename);
			SW.causticsStrengthS = atof(value);
			GetPrivateProfileStringA(pNextSection,"shoreFactor",Water_shoreFactor,value,255,Filename);
			SW.shoreFactor = atof(value);
			GetPrivateProfileStringA(pNextSection,"turbidity",Water_turbidity,value,255,Filename);
			SW.turbidity = atof(value);
			GetPrivateProfileStringA(pNextSection,"inScattCoeff",Water_inScattCoeff,value,255,Filename);
			SW.inScattCoeff = atof(value);
			GetPrivateProfileStringA(pNextSection,"inExtCoeff_R",Water_inExtCoeff_R,value,255,Filename);
			SW.inExtCoeff_R = atof(value);
			GetPrivateProfileStringA(pNextSection,"inExtCoeff_G",Water_inExtCoeff_G,value,255,Filename);
			SW.inExtCoeff_G = atof(value);
			GetPrivateProfileStringA(pNextSection,"inExtCoeff_B",Water_inExtCoeff_B,value,255,Filename);
			SW.inExtCoeff_B = atof(value);
			GetPrivateProfileStringA(pNextSection,"IceinExtCoeff_R",Water_IceinExtCoeff_R,value,255,Filename);
			SW.IceinExtCoeff_R = atof(value);
			GetPrivateProfileStringA(pNextSection,"IceinExtCoeff_G",Water_IceinExtCoeff_G,value,255,Filename);
			SW.IceinExtCoeff_G = atof(value);
			GetPrivateProfileStringA(pNextSection,"IceinExtCoeff_B",Water_IceinExtCoeff_B,value,255,Filename);
			SW.IceinExtCoeff_B = atof(value);
			GetPrivateProfileStringA(pNextSection,"Icechoppiness",Water_Icechoppiness,value,255,Filename);
			SW.Icechoppiness = atof(value);
			GetPrivateProfileStringA(pNextSection,"IcewaveWidth",Water_IcewaveWidth,value,255,Filename);
			SW.IcewaveWidth = atof(value);
			GetPrivateProfileStringA(pNextSection,"Icereflectivity",Water_Icereflectivity,value,255,Filename);
			SW.Icereflectivity = atof(value);
			GetPrivateProfileStringA(pNextSection,"IceshoreFactor",Water_IceshoreFactor,value,255,Filename);
			SW.IceshoreFactor = atof(value);
			GetPrivateProfileStringA(pNextSection,"Iceturbidity",Water_Iceturbidity,value,255,Filename);
			SW.Iceturbidity = atof(value);
			GetPrivateProfileStringA(pNextSection,"depthDarkness",Water_depthDarkness,value,255,Filename);
			SW.depthDarkness = atof(value);
			GetPrivateProfileStringA(pNextSection,"LODdistance",Water_LODdistance,value,255,Filename);
			SW.LODdistance = atof(value);
			GetPrivateProfileStringA(pNextSection,"MinLOD",Water_MinLOD,value,255,Filename);
			SW.MinLOD = atof(value);
			GetPrivateProfileStringA(pNextSection,"LensTimeMultA",Water_LensTimeMultA,value,255,Filename);
			SW.LensTimeMultA = atof(value);
			GetPrivateProfileStringA(pNextSection,"LensTimeMultB",Water_LensTimeMultB,value,255,Filename);
			SW.LensTimeMultB = atof(value);
			GetPrivateProfileStringA(pNextSection,"LensTime",Water_LensTime,value,255,Filename);
			SW.LensTime = atof(value);
			GetPrivateProfileStringA(pNextSection,"LensAmount",Water_LensAmount,value,255,Filename);
			SW.LensAmount = atof(value);
			GetPrivateProfileStringA(pNextSection,"LensViscosity",Water_LensViscosity,value,255,Filename);
			SW.LensViscosity = atof(value);
			SettingsWater[std::string(pNextSection)] = SW;
		}
		pNextSection = pNextSection + strlen(pNextSection) + 1;
	}

	strcpy(Filename, CurrentPath);
	strcat(Filename, SettingsPath);
	strcat(Filename,"Grass\\Grass.ini");
	GetPrivateProfileStringA("Default","WindEnabled","1",value,255,Filename);
	SettingsGrass.WindEnabled = atoi(value);
	GetPrivateProfileStringA("Default","GrassDensity","6",value,255,Filename);
	SettingsGrass.GrassDensity = atoi(value);
	GetPrivateProfileStringA("Default","WindCoefficient","100",value,255,Filename);
	SettingsGrass.WindCoefficient = atof(value);
	GetPrivateProfileStringA("Default","ScaleX","2.0",value,255,Filename);
	SettingsGrass.ScaleX = atof(value);
	GetPrivateProfileStringA("Default","ScaleY","2.0",value,255,Filename);
	SettingsGrass.ScaleY = atof(value);
	GetPrivateProfileStringA("Default","ScaleZ","1.0",value,255,Filename);
	SettingsGrass.ScaleZ = atof(value);
	GetPrivateProfileStringA("Default","MinHeight","0.8",value,255,Filename);
	SettingsGrass.MinHeight = atof(value);
	GetPrivateProfileStringA("Default","MinDistance","11000.0",value,255,Filename);
	SettingsGrass.MinDistance = atof(value);
	GetPrivateProfileStringA("Default","MaxDistance","12000.0",value,255,Filename);
	SettingsGrass.MaxDistance = atof(value);

	strcpy(Filename, CurrentPath);
	strcat(Filename, SettingsPath);
	strcat(Filename,"HDR\\HDR.ini");
	GetPrivateProfileStringA("Default","ToneMapping","1.8",value,255,Filename);
	SettingsHDR.ToneMapping = atof(value);
	GetPrivateProfileStringA("Default","ToneMappingBlur","-0.1",value,255,Filename);
	SettingsHDR.ToneMappingBlur = atof(value);
	GetPrivateProfileStringA("Default","ToneMappingColor","0",value,255,Filename);
	SettingsHDR.ToneMappingColor = atof(value);

	strcpy(Filename, CurrentPath);
	strcat(Filename, SettingsPath);
	strcat(Filename,"POM\\POM.ini");
	GetPrivateProfileStringA("Default","HeightMapScale","0.05",value,255,Filename);
	SettingsPOM.HeightMapScale = atof(value);
	GetPrivateProfileStringA("Default","ShadowSoftening","0.58",value,255,Filename);
	SettingsPOM.ShadowSoftening = atof(value);
	GetPrivateProfileStringA("Default","MinSamples","16",value,255,Filename);
	SettingsPOM.MinSamples = atof(value);
	GetPrivateProfileStringA("Default","MaxSamples","160",value,255,Filename);
	SettingsPOM.MaxSamples = atof(value);

	strcpy(Filename, CurrentPath);
	strcat(Filename, SettingsPath);
	strcat(Filename,"Terrain\\Terrain.ini");
	GetPrivateProfileStringA("Default","DistantSpecular","0.0",value,255,Filename);
	SettingsTerrain.DistantSpecular = atof(value);
	GetPrivateProfileStringA("Default","DistantNoise","0.4",value,255,Filename);
	SettingsTerrain.DistantNoise = atof(value);
	GetPrivateProfileStringA("Default","NearSpecular","0.0",value,255,Filename);
	SettingsTerrain.NearSpecular = atof(value);
	GetPrivateProfileStringA("Default","MiddleSpecular","0.0",value,255,Filename);
	SettingsTerrain.MiddleSpecular = atof(value);

	strcpy(Filename, CurrentPath);
	strcat(Filename, SettingsPath);
	strcat(Filename,"Skin\\Skin.ini");
	GetPrivateProfileStringA("Default","Attenuation","1.0",value,255,Filename);
	SettingsSkin.Attenuation = atof(value);
	GetPrivateProfileStringA("Default","SpecularPower","1.0",value,255,Filename);
	SettingsSkin.SpecularPower = atof(value);
	GetPrivateProfileStringA("Default","MaterialThickness","0.25",value,255,Filename);
	SettingsSkin.MaterialThickness = atof(value);
	GetPrivateProfileStringA("Default","RimScalar","1.0",value,255,Filename);
	SettingsSkin.RimScalar = atof(value);
	GetPrivateProfileStringA("Default","ExtCoeffRed","0.70",value,255,Filename);
	SettingsSkin.ExtCoeffRed = atof(value);
	GetPrivateProfileStringA("Default","ExtCoeffGreen","0.62",value,255,Filename);
	SettingsSkin.ExtCoeffGreen = atof(value);
	GetPrivateProfileStringA("Default","ExtCoeffBlue","0.50",value,255,Filename);
	SettingsSkin.ExtCoeffBlue = atof(value);

	strcpy(Filename, CurrentPath);
	strcat(Filename, SettingsPath);
	strcat(Filename,"GodRays\\GodRays.ini");
	GetPrivateProfileStringA("Default","SunGlareEnabled","1",value,255,Filename);
	SettingsGodRays.SunGlareEnabled = atoi(value);
	GetPrivateProfileStringA("Default","LightShaftPasses","50",value,255,Filename);
	SettingsGodRays.LightShaftPasses = atoi(value);
	GetPrivateProfileStringA("Default","RayIntensity","3.2",value,255,Filename);
	SettingsGodRays.RayIntensity = atof(value);
	GetPrivateProfileStringA("Default","RayLength","1.0",value,255,Filename);
	SettingsGodRays.RayLength = atof(value);
	GetPrivateProfileStringA("Default","RayDensity","0.8",value,255,Filename);
	SettingsGodRays.RayDensity = atof(value);
	GetPrivateProfileStringA("Default","RayVisibility","0.4",value,255,Filename);
	SettingsGodRays.RayVisibility = atof(value);
	GetPrivateProfileStringA("Default", "Luminance", "0.4", value, 255, Filename);
	SettingsGodRays.Luminance = atof(value);
	GetPrivateProfileStringA("Default", "GlobalMultiplier", "1.5", value, 255, Filename);
	SettingsGodRays.GlobalMultiplier = atof(value);
	GetPrivateProfileStringA("Default", "RayR", "0.5", value, 255, Filename);
	SettingsGodRays.RayR = atof(value);
	GetPrivateProfileStringA("Default", "RayG", "0.9", value, 255, Filename);
	SettingsGodRays.RayG = atof(value);
	GetPrivateProfileStringA("Default", "RayB", "0.8", value, 255, Filename);
	SettingsGodRays.RayB = atof(value);
	GetPrivateProfileStringA("Default","Saturate","0.55",value, 255, Filename);
	SettingsGodRays.Saturate = atof(value);

	strcpy(Filename, CurrentPath);
	strcat(Filename, SettingsPath);
	strcat(Filename,"DepthOfField\\DepthOfField.ini");
	GetPrivateProfileSectionNamesA(Sections,32767,Filename);
	pNextSection = Sections;
	while (*pNextSection != NULL)
	{
		GetPrivateProfileStringA(pNextSection,"Enabled","1",value,255,Filename);
		SD.Enabled = atoi(value);
		GetPrivateProfileStringA(pNextSection,"DialogMode","0",value,255,Filename);
		SD.DialogMode = atoi(value);
		GetPrivateProfileStringA(pNextSection,"DistantBlur","0",value,255,Filename);
		SD.DistantBlur = atoi(value);
		GetPrivateProfileStringA(pNextSection,"DistantBlurStartRange","800",value,255,Filename);
		SD.DistantBlurStartRange = atof(value);
		GetPrivateProfileStringA(pNextSection,"DistantBlurEndRange","1000",value,255,Filename);
		SD.DistantBlurEndRange = atof(value);
		GetPrivateProfileStringA(pNextSection,"BaseBlurRadius","1.0",value,255,Filename);
		SD.BaseBlurRadius = atof(value);
		GetPrivateProfileStringA(pNextSection,"BlurFallOff","2.0",value,255,Filename);
		SD.BlurFallOff = atof(value);
		GetPrivateProfileStringA(pNextSection,"Radius","8.0",value,255,Filename);
		SD.Radius = atof(value);
		GetPrivateProfileStringA(pNextSection,"DiameterRange","0.009",value,255,Filename);
		SD.DiameterRange = atof(value);
		GetPrivateProfileStringA(pNextSection,"NearBlurCutOff","0.8",value,255,Filename);
		SD.NearBlurCutOff = atof(value);
		SettingsDepthOfField[std::string(pNextSection)] = SD;
		pNextSection = pNextSection + strlen(pNextSection) + 1;
	}

	strcpy(Filename, CurrentPath);
	strcat(Filename, SettingsPath);
	strcat(Filename,"AmbientOcclusion\\AmbientOcclusion.ini");
	GetPrivateProfileSectionNamesA(Sections,32767,Filename);
	pNextSection = Sections;
	while (*pNextSection != NULL)
	{
		GetPrivateProfileStringA(pNextSection,"Enabled","1",value,255,Filename);
		SA.Enabled = atoi(value);
		GetPrivateProfileStringA(pNextSection,"RadiusMultiplier","8",value,255,Filename);
		SA.RadiusMultiplier = atof(value);
		GetPrivateProfileStringA(pNextSection,"StrengthMultiplier","4",value,255,Filename);
		SA.StrengthMultiplier = atof(value);
		GetPrivateProfileStringA(pNextSection,"ClampStrength","0.1",value,255,Filename);
		SA.ClampStrength = atof(value);
		GetPrivateProfileStringA(pNextSection,"AngleBias","15",value,255,Filename);
		SA.AngleBias = atof(value);
		GetPrivateProfileStringA(pNextSection,"Range","1500",value,255,Filename);
		SA.Range = atof(value);
		GetPrivateProfileStringA(pNextSection,"LumThreshold","0.2",value,255,Filename);
		SA.LumThreshold = atof(value);
		GetPrivateProfileStringA(pNextSection,"BlurDropThreshold","100",value,255,Filename);
		SA.BlurDropThreshold = atof(value);
		GetPrivateProfileStringA(pNextSection,"BlurRadiusMultiplier","1.0",value,255,Filename);
		SA.BlurRadiusMultiplier = atof(value);
		SettingsAmbientOcclusion[std::string(pNextSection)] = SA;
		pNextSection = pNextSection + strlen(pNextSection) + 1;
	}

	strcpy(Filename, CurrentPath);
	strcat(Filename, SettingsPath);
	strcat(Filename,"Coloring\\Coloring.ini");
	GetPrivateProfileStringA("Default","Strength","0.5",Coloring_Strength,255,Filename);
	SC.Strength = atof(Coloring_Strength);
	GetPrivateProfileStringA("Default","BaseGamma","1.6",Coloring_BaseGamma,255,Filename);
	SC.BaseGamma = atof(Coloring_BaseGamma);
	GetPrivateProfileStringA("Default","Fade","0.0",Coloring_Fade,255,Filename);
	SC.Fade = atof(Coloring_Fade);
	GetPrivateProfileStringA("Default","Contrast","0.8",Coloring_Contrast,255,Filename);
	SC.Contrast = atof(Coloring_Contrast);
	GetPrivateProfileStringA("Default","Saturation","-0.1",Coloring_Saturation,255,Filename);
	SC.Saturation = atof(Coloring_Saturation);
	GetPrivateProfileStringA("Default","Bleach","0.1",Coloring_Bleach,255,Filename);
	SC.Bleach = atof(Coloring_Bleach);
	GetPrivateProfileStringA("Default","BleachLuma","0.2",Coloring_BleachLuma,255,Filename);
	SC.BleachLuma = atof(Coloring_BleachLuma);
	GetPrivateProfileStringA("Default","ColorCurve","1.2",Coloring_ColorCurve,255,Filename);
	SC.ColorCurve = atof(Coloring_ColorCurve);
	GetPrivateProfileStringA("Default","ColorCurveR","0.9",Coloring_ColorCurveR,255,Filename);
	SC.ColorCurveR = atof(Coloring_ColorCurveR);
	GetPrivateProfileStringA("Default","ColorCurveG","1.0",Coloring_ColorCurveG,255,Filename);
	SC.ColorCurveG = atof(Coloring_ColorCurveG);
	GetPrivateProfileStringA("Default","ColorCurveB","1.0",Coloring_ColorCurveB,255,Filename);
	SC.ColorCurveB = atof(Coloring_ColorCurveB);
	GetPrivateProfileStringA("Default","EffectGamma","0.9",Coloring_EffectGamma,255,Filename);
	SC.EffectGamma = atof(Coloring_EffectGamma);
	GetPrivateProfileStringA("Default","EffectGammaR","1.0",Coloring_EffectGammaR,255,Filename);
	SC.EffectGammaR = atof(Coloring_EffectGammaR);
	GetPrivateProfileStringA("Default","EffectGammaG","1.0",Coloring_EffectGammaG,255,Filename);
	SC.EffectGammaG = atof(Coloring_EffectGammaG);
	GetPrivateProfileStringA("Default","EffectGammaB","1.0",Coloring_EffectGammaB,255,Filename);
	SC.EffectGammaB = atof(Coloring_EffectGammaB);
	GetPrivateProfileStringA("Default","Linearization","1.1",Coloring_Linearization,255,Filename);
	SC.Linearization = atof(Coloring_Linearization);
	SettingsColoring["Default"] = SC;

	GetPrivateProfileSectionNamesA(Sections,32767,Filename);
	pNextSection = Sections;
	while (*pNextSection != NULL)
	{
		if (strcmp(pNextSection,"Default") != 0) {
			GetPrivateProfileStringA(pNextSection,"Strength",Coloring_Strength,value,255,Filename);
			SC.Strength = atof(value);
			GetPrivateProfileStringA(pNextSection,"BaseGamma",Coloring_BaseGamma,value,255,Filename);
			SC.BaseGamma = atof(value);
			GetPrivateProfileStringA(pNextSection,"Fade",Coloring_Fade,value,255,Filename);
			SC.Fade = atof(value);
			GetPrivateProfileStringA(pNextSection,"Contrast",Coloring_Contrast,value,255,Filename);
			SC.Contrast = atof(value);
			GetPrivateProfileStringA(pNextSection,"Saturation",Coloring_Saturation,value,255,Filename);
			SC.Saturation = atof(value);
			GetPrivateProfileStringA(pNextSection,"Bleach",Coloring_Bleach,value,255,Filename);
			SC.Bleach = atof(value);
			GetPrivateProfileStringA(pNextSection,"BleachLuma",Coloring_BleachLuma,value,255,Filename);
			SC.BleachLuma = atof(value);
			GetPrivateProfileStringA(pNextSection,"ColorCurve",Coloring_ColorCurve,value,255,Filename);
			SC.ColorCurve = atof(value);
			GetPrivateProfileStringA(pNextSection,"ColorCurveR",Coloring_ColorCurveR,value,255,Filename);
			SC.ColorCurveR = atof(value);
			GetPrivateProfileStringA(pNextSection,"ColorCurveG",Coloring_ColorCurveG,value,255,Filename);
			SC.ColorCurveG = atof(value);
			GetPrivateProfileStringA(pNextSection,"ColorCurveB",Coloring_ColorCurveB,value,255,Filename);
			SC.ColorCurveB = atof(value);
			GetPrivateProfileStringA(pNextSection,"EffectGamma",Coloring_EffectGamma,value,255,Filename);
			SC.EffectGamma = atof(value);
			GetPrivateProfileStringA(pNextSection,"EffectGammaR",Coloring_EffectGammaR,value,255,Filename);
			SC.EffectGammaR = atof(value);
			GetPrivateProfileStringA(pNextSection,"EffectGammaG",Coloring_EffectGammaG,value,255,Filename);
			SC.EffectGammaG = atof(value);
			GetPrivateProfileStringA(pNextSection,"EffectGammaB",Coloring_EffectGammaB,value,255,Filename);
			SC.EffectGammaB = atof(value);
			GetPrivateProfileStringA(pNextSection,"Linearization",Coloring_Linearization,value,255,Filename);
			SC.Linearization = atof(value);
			SettingsColoring[std::string(pNextSection)] = SC;
		}
		pNextSection = pNextSection + strlen(pNextSection) + 1;
	}

	strcpy(Filename, CurrentPath);
	strcat(Filename, SettingsPath);
	strcat(Filename,"Cinema\\Cinema.ini");
	GetPrivateProfileStringA("Default","DialogMode","0",value,255,Filename);
	SettingsCinema.DialogMode = atoi(value);
	GetPrivateProfileStringA("Default","AspectRatio","1.20",value,255,Filename);
	SettingsCinema.AspectRatio = atof(value);
	GetPrivateProfileStringA("Default","VignetteRadius","6.0",value,255,Filename);
	SettingsCinema.VignetteRadius = atof(value);
	GetPrivateProfileStringA("Default","VignetteDarkness","0.55",value,255,Filename);
	SettingsCinema.VignetteDarkness = atof(value);

	strcpy(Filename, CurrentPath);
	strcat(Filename, SettingsPath);
	strcat(Filename,"Bloom\\Bloom.ini");
	GetPrivateProfileStringA("Default","BloomIntensity","1.4",Bloom_BloomIntensity,255,Filename);
	SB.BloomIntensity = atof(Bloom_BloomIntensity);
	GetPrivateProfileStringA("Default","OriginalIntensity","1.0",Bloom_OriginalIntensity,255,Filename);
	SB.OriginalIntensity = atof(Bloom_OriginalIntensity);
	GetPrivateProfileStringA("Default","BloomSaturation","1.2",Bloom_BloomSaturation,255,Filename);
	SB.BloomSaturation = atof(Bloom_BloomSaturation);
	GetPrivateProfileStringA("Default","OriginalSaturation","1.0",Bloom_OriginalSaturation,255,Filename);
	SB.OriginalSaturation = atof(Bloom_OriginalSaturation);
	GetPrivateProfileStringA("Default","Luminance","0.06",Bloom_Luminance,255,Filename);
	SB.Luminance = atof(Bloom_Luminance);
	GetPrivateProfileStringA("Default","MiddleGray","0.18",Bloom_MiddleGray,255,Filename);
	SB.MiddleGray = atof(Bloom_MiddleGray);
	GetPrivateProfileStringA("Default","WhiteCutOff","0.8",Bloom_WhiteCutOff,255,Filename);
	SB.WhiteCutOff = atof(Bloom_WhiteCutOff);
	SettingsBloom["Default"] = SB;

	GetPrivateProfileSectionNamesA(Sections,32767,Filename);
	pNextSection = Sections;
	while (*pNextSection != NULL)
	{
		if (strcmp(pNextSection,"Default") != 0) {
			GetPrivateProfileStringA(pNextSection,"BloomIntensity",Bloom_BloomIntensity,value,255,Filename);
			SB.BloomIntensity = atof(value);
			GetPrivateProfileStringA(pNextSection,"OriginalIntensity",Bloom_OriginalIntensity,value,255,Filename);
			SB.OriginalIntensity = atof(value);
			GetPrivateProfileStringA(pNextSection,"BloomSaturation",Bloom_BloomSaturation,value,255,Filename);
			SB.BloomSaturation = atof(value);
			GetPrivateProfileStringA(pNextSection,"OriginalSaturation",Bloom_OriginalSaturation,value,255,Filename);
			SB.OriginalSaturation = atof(value);
			GetPrivateProfileStringA(pNextSection,"Luminance",Bloom_Luminance,value,255,Filename);
			SB.Luminance = atof(value);
			GetPrivateProfileStringA(pNextSection,"MiddleGray",Bloom_MiddleGray,value,255,Filename);
			SB.MiddleGray = atof(value);
			GetPrivateProfileStringA(pNextSection,"WhiteCutOff",Bloom_WhiteCutOff,value,255,Filename);
			SB.WhiteCutOff = atof(value);
			SettingsBloom[std::string(pNextSection)] = SB;
		}
		pNextSection = pNextSection + strlen(pNextSection) + 1;
	}

	strcpy(Filename, CurrentPath);
	strcat(Filename, SettingsPath);
	strcat(Filename,"Precipitations\\Precipitations.ini");
	GetPrivateProfileStringA("Default","Intensity","0.25",value,255,Filename);
	SettingsPrecipitations.Intensity = atof(value);
	GetPrivateProfileStringA("Default","Velocity","0.5",value,255,Filename);
	SettingsPrecipitations.Velocity = atof(value);
	GetPrivateProfileStringA("Default","Size","2.0",value,255,Filename);
	SettingsPrecipitations.Size = atof(value);
	GetPrivateProfileStringA("Default","SnowAmount","2.0",value,255,Filename);
	SettingsPrecipitations.SnowAmount = atof(value);
	GetPrivateProfileStringA("Default","SnowIncrease","0.0005",value,255,Filename);
	SettingsPrecipitations.SnowIncrease = atof(value);
	GetPrivateProfileStringA("Default","SnowDecrease","0.0001",value,255,Filename);
	SettingsPrecipitations.SnowDecrease = atof(value);
	GetPrivateProfileStringA("Default","SnowSunPower","0.3",value,255,Filename);
	SettingsPrecipitations.SnowSunPower = atof(value);
	GetPrivateProfileStringA("Default","SnowBlurNormDropThreshhold","-0.5",value,255,Filename);
	SettingsPrecipitations.SnowBlurNormDropThreshhold = atof(value);
	GetPrivateProfileStringA("Default","SnowBlurRadiusMultiplier","1",value,255,Filename);
	SettingsPrecipitations.SnowBlurRadiusMultiplier = atof(value);
	GetPrivateProfileStringA("Default","RainAmount","1.0",value,255,Filename);
	SettingsPrecipitations.RainAmount = atof(value);
	GetPrivateProfileStringA("Default","RainIncrease","0.001",value,255,Filename);
	SettingsPrecipitations.RainIncrease = atof(value);
	GetPrivateProfileStringA("Default","RainDecrease","0.0001",value,255,Filename);
	SettingsPrecipitations.RainDecrease = atof(value);
	GetPrivateProfileStringA("Default","PuddleCoeff_R","0.62",value,255,Filename);
	SettingsPrecipitations.PuddleCoeff_R = atof(value);
	GetPrivateProfileStringA("Default","PuddleCoeff_G","0.6",value,255,Filename);
	SettingsPrecipitations.PuddleCoeff_G = atof(value);
	GetPrivateProfileStringA("Default","PuddleCoeff_B","0.55",value,255,Filename);
	SettingsPrecipitations.PuddleCoeff_B = atof(value);
	GetPrivateProfileStringA("Default","PuddleSpecularMultiplier","0.2",value,255,Filename);
	SettingsPrecipitations.PuddleSpecularMultiplier = atof(value);

	strcpy(Filename, CurrentPath);
	strcat(Filename, SettingsPath);
	strcat(Filename,"Blood\\Blood.ini");
	GetPrivateProfileStringA("Default","LensChance","10",value,255,Filename);
	SettingsBlood.LensChance = atof(value);
	GetPrivateProfileStringA("Default","LensColorR","0.92",value,255,Filename);
	SettingsBlood.LensColorR = atof(value);
	GetPrivateProfileStringA("Default","LensColorG","0.16",value,255,Filename);
	SettingsBlood.LensColorG = atof(value);
	GetPrivateProfileStringA("Default","LensColorB","0.16",value,255,Filename);
	SettingsBlood.LensColorB = atof(value);
	GetPrivateProfileStringA("Default","LensIntensity","0.8",value,255,Filename);
	SettingsBlood.LensIntensity = atof(value);
	GetPrivateProfileStringA("Default","LensTime","360",value,255,Filename);
	SettingsBlood.LensTime = atof(value);

	strcpy(Filename, CurrentPath);
	strcat(Filename, SettingsPath);
	strcat(Filename,"MotionBlur\\MotionBlur.ini");
	GetPrivateProfileSectionNamesA(Sections,32767,Filename);
	pNextSection = Sections;
	while (*pNextSection != NULL)
	{
		GetPrivateProfileStringA(pNextSection,"Enabled","1",value,255,Filename);
		SM.Enabled = atoi(value);
		GetPrivateProfileStringA(pNextSection,"GaussianWeight","6.0",value,255,Filename);
		SM.GaussianWeight = atof(value);
		GetPrivateProfileStringA(pNextSection,"BlurScale","4.0",value,255,Filename);
		SM.BlurScale = atof(value);
		GetPrivateProfileStringA(pNextSection,"BlurOffsetMax","24.0",value,255,Filename);
		SM.BlurOffsetMax = atof(value);
		GetPrivateProfileStringA(pNextSection,"BlurCutOff","15",value,255,Filename);
		SM.BlurCutOff = atof(value);
		SettingsMotionBlur[std::string(pNextSection)] = SM;
		pNextSection = pNextSection + strlen(pNextSection) + 1;
	}

	strcpy(Filename, CurrentPath);
	strcat(Filename, SettingsPath);
	strcat(Filename,"LowHF\\LowHF.ini");
	GetPrivateProfileStringA("Default","HealthLimit","0.5",value,255,Filename);
	SettingsLowHF.HealthLimit = atof(value);
	GetPrivateProfileStringA("Default","FatigueLimit","0.4",value,255,Filename);
	SettingsLowHF.FatigueLimit = atof(value);
	GetPrivateProfileStringA("Default","LumaMultiplier","0.1",value,255,Filename);
	SettingsLowHF.LumaMultiplier = atof(value);
	GetPrivateProfileStringA("Default","BlurMultiplier","0.1",value,255,Filename);
	SettingsLowHF.BlurMultiplier = atof(value);
	GetPrivateProfileStringA("Default","VignetteMultiplier","0.1",value,255,Filename);
	SettingsLowHF.VignetteMultiplier = atof(value);
	GetPrivateProfileStringA("Default","DarknessMultiplier","0.1",value,255,Filename);
	SettingsLowHF.DarknessMultiplier = atof(value);

	strcpy(Filename, CurrentPath);
	strcat(Filename, SettingsPath);
	strcat(Filename,"Sharpening\\Sharpening.ini");
	GetPrivateProfileStringA("Default","Strength","0.75",value,255,Filename);
	SettingsSharpening.Strength = atof(value);
	GetPrivateProfileStringA("Default","Clamp","0.1",value,255,Filename);
	SettingsSharpening.Clamp = atof(value);
	GetPrivateProfileStringA("Default","Offset","2.0",value,255,Filename);
	SettingsSharpening.Offset = atof(value);

	strcpy(Filename, CurrentPath);
	strcat(Filename, SettingsPath);
	strcat(Filename,"Shadows\\Shadows.ini");
	GetPrivateProfileStringA("Default","ShadowIntensity","0.9",value,255,Filename);
	SettingsShadows.ShadowIntensity = atof(value);
	GetPrivateProfileStringA("Default","SelfShadowIntensity","0.6",value,255,Filename);
	SettingsShadows.SelfShadowIntensity = atof(value);

}

void SettingManager::SaveSettings(const char* Name) {

	char Filename[MAX_PATH];
	std::string Sections;

	strcpy(Filename, CurrentPath);
	strcat(Filename, SettingsPath);
	if (!strcmp(Name, "AmbientOcclusion")) {
		WritePrivateProfileStringA("Effects", "EnableAmbientOcclusion", ToString(SettingsMain.EnableAmbientOcclusion).c_str(), SettingsMain.MainSettingsFullFile);
		strcat(Filename,"AmbientOcclusion\\AmbientOcclusion.ini");
		SettingsAmbientOcclusionList::iterator v = SettingsAmbientOcclusion.begin();
		while (v != SettingsAmbientOcclusion.end()) {
			WritePrivateProfileStringA(v->first.c_str(), "AngleBias", ToString(v->second.AngleBias).c_str(), Filename);
			WritePrivateProfileStringA(v->first.c_str(), "BlurDropThreshold", ToString(v->second.BlurDropThreshold).c_str(), Filename);
			WritePrivateProfileStringA(v->first.c_str(), "BlurRadiusMultiplier", ToString(v->second.BlurRadiusMultiplier).c_str(), Filename);
			WritePrivateProfileStringA(v->first.c_str(), "ClampStrength", ToString(v->second.ClampStrength).c_str(), Filename);
			WritePrivateProfileStringA(v->first.c_str(), "Enabled", ToString(v->second.Enabled).c_str(), Filename);
			WritePrivateProfileStringA(v->first.c_str(), "LumThreshold", ToString(v->second.LumThreshold).c_str(), Filename);
			WritePrivateProfileStringA(v->first.c_str(), "RadiusMultiplier", ToString(v->second.RadiusMultiplier).c_str(), Filename);
			WritePrivateProfileStringA(v->first.c_str(), "Range", ToString(v->second.Range).c_str(), Filename);
			WritePrivateProfileStringA(v->first.c_str(), "StrengthMultiplier", ToString(v->second.StrengthMultiplier).c_str(), Filename);
			v++;
		}
	}
	else if (!strcmp(Name, "Blood")) {
		strcat(Filename,"Blood\\Blood.ini");
		WritePrivateProfileStringA("Default", "LensChance", ToString(SettingsBlood.LensChance).c_str(), Filename);
		WritePrivateProfileStringA("Default", "LensColorB", ToString(SettingsBlood.LensColorB).c_str(), Filename);
		WritePrivateProfileStringA("Default", "LensColorG", ToString(SettingsBlood.LensColorG).c_str(), Filename);
		WritePrivateProfileStringA("Default", "LensColorR", ToString(SettingsBlood.LensColorR).c_str(), Filename);
		WritePrivateProfileStringA("Default", "LensIntensity", ToString(SettingsBlood.LensIntensity).c_str(), Filename);
		WritePrivateProfileStringA("Default", "LensTime", ToString(SettingsBlood.LensTime).c_str(), Filename);
	}
	else if (!strcmp(Name, "BloodLens")) {
		WritePrivateProfileStringA("Effects", "EnableBloodLens", ToString(SettingsMain.EnableBloodLens).c_str(), SettingsMain.MainSettingsFullFile);
	}
	else if (!strcmp(Name, "Bloom")) {
		WritePrivateProfileStringA("Effects", "EnableBloom", ToString(SettingsMain.EnableBloom).c_str(), SettingsMain.MainSettingsFullFile);
		strcat(Filename,"Bloom\\Bloom.ini");
		SettingsBloomList::iterator v = SettingsBloom.begin();
		while (v != SettingsBloom.end()) {
			WritePrivateProfileStringA(v->first.c_str(), "BloomIntensity", ToString(v->second.BloomIntensity).c_str(), Filename);
			WritePrivateProfileStringA(v->first.c_str(), "BloomSaturation", ToString(v->second.BloomSaturation).c_str(), Filename);
			WritePrivateProfileStringA(v->first.c_str(), "Luminance", ToString(v->second.Luminance).c_str(), Filename);
			WritePrivateProfileStringA(v->first.c_str(), "MiddleGray", ToString(v->second.MiddleGray).c_str(), Filename);
			WritePrivateProfileStringA(v->first.c_str(), "OriginalIntensity", ToString(v->second.OriginalIntensity).c_str(), Filename);
			WritePrivateProfileStringA(v->first.c_str(), "OriginalSaturation", ToString(v->second.OriginalSaturation).c_str(), Filename);
			WritePrivateProfileStringA(v->first.c_str(), "WhiteCutOff", ToString(v->second.WhiteCutOff).c_str(), Filename);
			v++;
		}
	}
	else if (!strcmp(Name, "Cinema")) {
		WritePrivateProfileStringA("Effects", "EnableCinema", ToString(SettingsMain.EnableCinema).c_str(), SettingsMain.MainSettingsFullFile);
		strcat(Filename,"Cinema\\Cinema.ini");
		WritePrivateProfileStringA("Default", "AspectRatio", ToString(SettingsCinema.AspectRatio).c_str(), Filename);
		WritePrivateProfileStringA("Default", "DialogMode", ToString(SettingsCinema.DialogMode).c_str(), Filename);
		WritePrivateProfileStringA("Default", "VignetteDarkness", ToString(SettingsCinema.VignetteDarkness).c_str(), Filename);
		WritePrivateProfileStringA("Default", "VignetteRadius", ToString(SettingsCinema.VignetteRadius).c_str(), Filename);
	}
	else if (!strcmp(Name, "Coloring")) {
		WritePrivateProfileStringA("Effects", "EnableColoring", ToString(SettingsMain.EnableColoring).c_str(), SettingsMain.MainSettingsFullFile);
		strcat(Filename,"Coloring\\Coloring.ini");
		SettingsColoringList::iterator v = SettingsColoring.begin();
		while (v != SettingsColoring.end()) {
			WritePrivateProfileStringA(v->first.c_str(), "BaseGamma", ToString(v->second.BaseGamma).c_str(), Filename);
			WritePrivateProfileStringA(v->first.c_str(), "Bleach", ToString(v->second.Bleach).c_str(), Filename);
			WritePrivateProfileStringA(v->first.c_str(), "BleachLuma", ToString(v->second.BleachLuma).c_str(), Filename);
			WritePrivateProfileStringA(v->first.c_str(), "ColorCurve", ToString(v->second.ColorCurve).c_str(), Filename);
			WritePrivateProfileStringA(v->first.c_str(), "ColorCurveB", ToString(v->second.ColorCurveB).c_str(), Filename);
			WritePrivateProfileStringA(v->first.c_str(), "ColorCurveG", ToString(v->second.ColorCurveG).c_str(), Filename);
			WritePrivateProfileStringA(v->first.c_str(), "ColorCurveR", ToString(v->second.ColorCurveR).c_str(), Filename);
			WritePrivateProfileStringA(v->first.c_str(), "Contrast", ToString(v->second.Contrast).c_str(), Filename);
			WritePrivateProfileStringA(v->first.c_str(), "EffectGamma", ToString(v->second.EffectGamma).c_str(), Filename);
			WritePrivateProfileStringA(v->first.c_str(), "EffectGammaB", ToString(v->second.EffectGammaB).c_str(), Filename);
			WritePrivateProfileStringA(v->first.c_str(), "EffectGammaG", ToString(v->second.EffectGammaG).c_str(), Filename);
			WritePrivateProfileStringA(v->first.c_str(), "EffectGammaR", ToString(v->second.EffectGammaR).c_str(), Filename);
			WritePrivateProfileStringA(v->first.c_str(), "Fade", ToString(v->second.Fade).c_str(), Filename);
			WritePrivateProfileStringA(v->first.c_str(), "Linearization", ToString(v->second.Linearization).c_str(), Filename);
			WritePrivateProfileStringA(v->first.c_str(), "Saturation", ToString(v->second.Saturation).c_str(), Filename);
			WritePrivateProfileStringA(v->first.c_str(), "Strength", ToString(v->second.Strength).c_str(), Filename);
			v++;
		}
	}
	else if (!strcmp(Name, "DepthOfField")) {
		WritePrivateProfileStringA("Effects", "EnableDepthOfField", ToString(SettingsMain.EnableDepthOfField).c_str(), SettingsMain.MainSettingsFullFile);
		strcat(Filename,"DepthOfField\\DepthOfField.ini");
		SettingsDepthOfFieldList::iterator v = SettingsDepthOfField.begin();
		while (v != SettingsDepthOfField.end()) {
			WritePrivateProfileStringA(v->first.c_str(), "BaseBlurRadius", ToString(v->second.BaseBlurRadius).c_str(), Filename);
			WritePrivateProfileStringA(v->first.c_str(), "BlurFallOff", ToString(v->second.BlurFallOff).c_str(), Filename);
			WritePrivateProfileStringA(v->first.c_str(), "DialogMode", ToString(v->second.DialogMode).c_str(), Filename);
			WritePrivateProfileStringA(v->first.c_str(), "DiameterRange", ToString(v->second.DiameterRange).c_str(), Filename);
			WritePrivateProfileStringA(v->first.c_str(), "DistantBlur", ToString(v->second.DistantBlur).c_str(), Filename);
			WritePrivateProfileStringA(v->first.c_str(), "DistantBlurEndRange", ToString(v->second.DistantBlurEndRange).c_str(), Filename);
			WritePrivateProfileStringA(v->first.c_str(), "DistantBlurStartRange", ToString(v->second.DistantBlurStartRange).c_str(), Filename);
			WritePrivateProfileStringA(v->first.c_str(), "Enabled", ToString(v->second.Enabled).c_str(), Filename);
			WritePrivateProfileStringA(v->first.c_str(), "NearBlurCutOff", ToString(v->second.NearBlurCutOff).c_str(), Filename);
			WritePrivateProfileStringA(v->first.c_str(), "Radius", ToString(v->second.Radius).c_str(), Filename);
			v++;
		}
	}
	else if (!strcmp(Name, "GodRays")) {
		WritePrivateProfileStringA("Effects", "EnableGodRays", ToString(SettingsMain.EnableGodRays).c_str(), SettingsMain.MainSettingsFullFile);
		strcat(Filename,"GodRays\\GodRays.ini");
		WritePrivateProfileStringA("Default", "RayDensity", ToString(SettingsGodRays.RayDensity).c_str(), Filename);
		WritePrivateProfileStringA("Default", "RayIntensity", ToString(SettingsGodRays.RayIntensity).c_str(), Filename);
		WritePrivateProfileStringA("Default", "RayLength", ToString(SettingsGodRays.RayLength).c_str(), Filename);
		WritePrivateProfileStringA("Default", "GlobalMultiplier", ToString(SettingsGodRays.GlobalMultiplier).c_str(), Filename);
		WritePrivateProfileStringA("Default", "Saturate", ToString(SettingsGodRays.Saturate).c_str(), Filename);
		WritePrivateProfileStringA("Default", "LightShaftPasses", ToString(SettingsGodRays.LightShaftPasses).c_str(), Filename);
		WritePrivateProfileStringA("Default", "Luminance", ToString(SettingsGodRays.Luminance).c_str(), Filename);
		WritePrivateProfileStringA("Default", "RayR", ToString(SettingsGodRays.RayR).c_str(), Filename);
		WritePrivateProfileStringA("Default", "RayG", ToString(SettingsGodRays.RayG).c_str(), Filename);
		WritePrivateProfileStringA("Default", "RayB", ToString(SettingsGodRays.RayB).c_str(), Filename);
		WritePrivateProfileStringA("Default", "SunGlareEnabled", ToString(SettingsGodRays.SunGlareEnabled).c_str(), Filename);
		WritePrivateProfileStringA("Default", "RayVisibility", ToString(SettingsGodRays.RayVisibility).c_str(), Filename);
	}
	else if (!strcmp(Name, "Grass")) {
		strcat(Filename,"Grass\\Grass.ini");
		WritePrivateProfileStringA("Default", "GrassDensity", ToString(SettingsGrass.GrassDensity).c_str(), Filename);
		WritePrivateProfileStringA("Default", "MaxDistance", ToString(SettingsGrass.MaxDistance).c_str(), Filename);
		WritePrivateProfileStringA("Default", "MinDistance", ToString(SettingsGrass.MinDistance).c_str(), Filename);
		WritePrivateProfileStringA("Default", "MinHeight", ToString(SettingsGrass.MinHeight).c_str(), Filename);
		WritePrivateProfileStringA("Default", "ScaleX", ToString(SettingsGrass.ScaleX).c_str(), Filename);
		WritePrivateProfileStringA("Default", "ScaleY", ToString(SettingsGrass.ScaleY).c_str(), Filename);
		WritePrivateProfileStringA("Default", "ScaleZ", ToString(SettingsGrass.ScaleZ).c_str(), Filename);
		WritePrivateProfileStringA("Default", "WindCoefficient", ToString(SettingsGrass.WindCoefficient).c_str(), Filename);
		WritePrivateProfileStringA("Default", "WindEnabled", ToString(SettingsGrass.WindEnabled).c_str(), Filename);
	}
	else if (!strcmp(Name, "HDR")) {
		strcat(Filename,"HDR\\HDR.ini");
		WritePrivateProfileStringA("Default", "ToneMapping", ToString(SettingsHDR.ToneMapping).c_str(), Filename);
		WritePrivateProfileStringA("Default", "ToneMappingBlur", ToString(SettingsHDR.ToneMappingBlur).c_str(), Filename);
		WritePrivateProfileStringA("Default", "ToneMappingColor", ToString(SettingsHDR.ToneMappingColor).c_str(), Filename);
	}
	else if (!strcmp(Name, "LowHF")) {
		WritePrivateProfileStringA("Effects", "EnableLowHF", ToString(SettingsMain.EnableLowHF).c_str(), SettingsMain.MainSettingsFullFile);
		strcat(Filename,"LowHF\\LowHF.ini");
		WritePrivateProfileStringA("Default", "BlurMultiplier", ToString(SettingsLowHF.BlurMultiplier).c_str(), Filename);
		WritePrivateProfileStringA("Default", "DarknessMultiplier", ToString(SettingsLowHF.DarknessMultiplier).c_str(), Filename);
		WritePrivateProfileStringA("Default", "FatigueLimit", ToString(SettingsLowHF.FatigueLimit).c_str(), Filename);
		WritePrivateProfileStringA("Default", "HealthLimit", ToString(SettingsLowHF.HealthLimit).c_str(), Filename);
		WritePrivateProfileStringA("Default", "LumaMultiplier", ToString(SettingsLowHF.LumaMultiplier).c_str(), Filename);
		WritePrivateProfileStringA("Default", "VignetteMultiplier", ToString(SettingsLowHF.VignetteMultiplier).c_str(), Filename);
	}
	else if (!strcmp(Name, "MotionBlur")) {
		WritePrivateProfileStringA("Effects", "EnableMotionBlur", ToString(SettingsMain.EnableMotionBlur).c_str(), SettingsMain.MainSettingsFullFile);
		strcat(Filename,"MotionBlur\\MotionBlur.ini");
		SettingsMotionBlurList::iterator v = SettingsMotionBlur.begin();
		while (v != SettingsMotionBlur.end()) {
			WritePrivateProfileStringA(v->first.c_str(), "BloomIntensity", ToString(v->second.BlurCutOff).c_str(), Filename);
			WritePrivateProfileStringA(v->first.c_str(), "BloomSaturation", ToString(v->second.BlurOffsetMax).c_str(), Filename);
			WritePrivateProfileStringA(v->first.c_str(), "Luminance", ToString(v->second.BlurScale).c_str(), Filename);
			WritePrivateProfileStringA(v->first.c_str(), "MiddleGray", ToString(v->second.Enabled).c_str(), Filename);
			WritePrivateProfileStringA(v->first.c_str(), "OriginalIntensity", ToString(v->second.GaussianWeight).c_str(), Filename);
			v++;
		}
	}
	else if (!strcmp(Name, "POM")) {
		strcat(Filename,"POM\\POM.ini");
		WritePrivateProfileStringA("Default", "HeightMapScale", ToString(SettingsPOM.HeightMapScale).c_str(), Filename);
		WritePrivateProfileStringA("Default", "MaxSamples", ToString(SettingsPOM.MaxSamples).c_str(), Filename);
		WritePrivateProfileStringA("Default", "MinSamples", ToString(SettingsPOM.MinSamples).c_str(), Filename);
		WritePrivateProfileStringA("Default", "ShadowSoftening", ToString(SettingsPOM.ShadowSoftening).c_str(), Filename);
	}
	else if (!strcmp(Name, "Precipitations")) {
		strcat(Filename,"Precipitations\\Precipitations.ini");
		WritePrivateProfileStringA("Default", "Intensity", ToString(SettingsPrecipitations.Intensity).c_str(), Filename);
		WritePrivateProfileStringA("Default", "Velocity", ToString(SettingsPrecipitations.Velocity).c_str(), Filename);
		WritePrivateProfileStringA("Default", "Size", ToString(SettingsPrecipitations.Size).c_str(), Filename);
		WritePrivateProfileStringA("Default", "SnowIncrease", ToString(SettingsPrecipitations.SnowIncrease).c_str(), Filename);
		WritePrivateProfileStringA("Default", "SnowDecrease", ToString(SettingsPrecipitations.SnowDecrease).c_str(), Filename);
		WritePrivateProfileStringA("Default", "SnowSunPower", ToString(SettingsPrecipitations.SnowSunPower).c_str(), Filename);
		WritePrivateProfileStringA("Default", "SnowAmount", ToString(SettingsPrecipitations.SnowAmount).c_str(), Filename);
		WritePrivateProfileStringA("Default", "SnowBlurNormDropThreshhold", ToString(SettingsPrecipitations.SnowBlurNormDropThreshhold).c_str(), Filename);
		WritePrivateProfileStringA("Default", "SnowBlurRadiusMultiplier", ToString(SettingsPrecipitations.SnowBlurRadiusMultiplier).c_str(), Filename);
		WritePrivateProfileStringA("Default", "RainIncrease", ToString(SettingsPrecipitations.RainIncrease).c_str(), Filename);
		WritePrivateProfileStringA("Default", "RainDecrease", ToString(SettingsPrecipitations.RainDecrease).c_str(), Filename);
		WritePrivateProfileStringA("Default", "RainAmount", ToString(SettingsPrecipitations.RainAmount).c_str(), Filename);
		WritePrivateProfileStringA("Default", "PuddleCoeff_B", ToString(SettingsPrecipitations.PuddleCoeff_B).c_str(), Filename);
		WritePrivateProfileStringA("Default", "PuddleCoeff_G", ToString(SettingsPrecipitations.PuddleCoeff_G).c_str(), Filename);
		WritePrivateProfileStringA("Default", "PuddleCoeff_R", ToString(SettingsPrecipitations.PuddleCoeff_R).c_str(), Filename);
		WritePrivateProfileStringA("Default", "PuddleSpecularMultiplier", ToString(SettingsPrecipitations.PuddleSpecularMultiplier).c_str(), Filename);
	}
	else if (!strcmp(Name, "Shadows")) {
		strcat(Filename,"Shadows\\Shadows.ini");
		WritePrivateProfileStringA("Default", "SelfShadowIntensity", ToString(SettingsShadows.SelfShadowIntensity).c_str(), Filename);
		WritePrivateProfileStringA("Default", "ShadowIntensity", ToString(SettingsShadows.ShadowIntensity).c_str(), Filename);
	}
	else if (!strcmp(Name, "Skin")) {
		strcat(Filename,"Skin\\Skin.ini");
		WritePrivateProfileStringA("Default", "Attenuation", ToString(SettingsSkin.Attenuation).c_str(), Filename);
		WritePrivateProfileStringA("Default", "ExtCoeffBlue", ToString(SettingsSkin.ExtCoeffBlue).c_str(), Filename);
		WritePrivateProfileStringA("Default", "ExtCoeffGreen", ToString(SettingsSkin.ExtCoeffGreen).c_str(), Filename);
		WritePrivateProfileStringA("Default", "ExtCoeffRed", ToString(SettingsSkin.ExtCoeffRed).c_str(), Filename);
		WritePrivateProfileStringA("Default", "MaterialThickness", ToString(SettingsSkin.MaterialThickness).c_str(), Filename);
		WritePrivateProfileStringA("Default", "RimScalar", ToString(SettingsSkin.RimScalar).c_str(), Filename);
		WritePrivateProfileStringA("Default", "SpecularPower", ToString(SettingsSkin.SpecularPower).c_str(), Filename);
	}
	else if (!strcmp(Name, "Sharpening")) {
		WritePrivateProfileStringA("Effects", "EnableSharpening", ToString(SettingsMain.EnableSharpening).c_str(), SettingsMain.MainSettingsFullFile);
		strcat(Filename,"Sharpening\\Sharpening.ini");
		WritePrivateProfileStringA("Default", "Strength", ToString(SettingsSharpening.Strength).c_str(), Filename);
		WritePrivateProfileStringA("Default", "Clamp", ToString(SettingsSharpening.Clamp).c_str(), Filename);
		WritePrivateProfileStringA("Default", "Offset", ToString(SettingsSharpening.Offset).c_str(), Filename);
	}
	else if (!strcmp(Name, "SMAA")) {
		WritePrivateProfileStringA("Effects", "EnableSMAA", ToString(SettingsMain.EnableSMAA).c_str(), SettingsMain.MainSettingsFullFile);
	}
	else if (!strcmp(Name, "SnowAccumulation")) {
		WritePrivateProfileStringA("Effects", "EnableSnowAccumulation", ToString(SettingsMain.EnableSnowAccumulation).c_str(), SettingsMain.MainSettingsFullFile);
	}
	else if (!strcmp(Name, "Terrain")) {
		strcat(Filename,"Terrain\\Terrain.ini");
		WritePrivateProfileStringA("Default", "DistantNoise", ToString(SettingsTerrain.DistantNoise).c_str(), Filename);
		WritePrivateProfileStringA("Default", "DistantSpecular", ToString(SettingsTerrain.DistantSpecular).c_str(), Filename);
		WritePrivateProfileStringA("Default", "MiddleSpecular", ToString(SettingsTerrain.MiddleSpecular).c_str(), Filename);
		WritePrivateProfileStringA("Default", "NearSpecular", ToString(SettingsTerrain.NearSpecular).c_str(), Filename);
	}
	else if (!strcmp(Name, "Underwater")) {
		WritePrivateProfileStringA("Effects", "EnableUnderwater", ToString(SettingsMain.EnableUnderwater).c_str(), SettingsMain.MainSettingsFullFile);
	}
	else if (!strcmp(Name, "Water")) {
		strcat(Filename,"Water\\Water.ini");
		SettingsWaterList::iterator v = SettingsWater.begin();
		while (v != SettingsWater.end()) {
			WritePrivateProfileStringA(v->first.c_str(), "causticsStrength", ToString(v->second.causticsStrength).c_str(), Filename);
			WritePrivateProfileStringA(v->first.c_str(), "causticsStrengthS", ToString(v->second.causticsStrengthS).c_str(), Filename);
			WritePrivateProfileStringA(v->first.c_str(), "choppiness", ToString(v->second.choppiness).c_str(), Filename);
			WritePrivateProfileStringA(v->first.c_str(), "depthDarkness", ToString(v->second.depthDarkness).c_str(), Filename);
			WritePrivateProfileStringA(v->first.c_str(), "Icechoppiness", ToString(v->second.Icechoppiness).c_str(), Filename);
			WritePrivateProfileStringA(v->first.c_str(), "IceEnabled", ToString(v->second.IceEnabled).c_str(), Filename);
			WritePrivateProfileStringA(v->first.c_str(), "IceinExtCoeff_B", ToString(v->second.IceinExtCoeff_B).c_str(), Filename);
			WritePrivateProfileStringA(v->first.c_str(), "IceinExtCoeff_G", ToString(v->second.IceinExtCoeff_G).c_str(), Filename);
			WritePrivateProfileStringA(v->first.c_str(), "IceinExtCoeff_R", ToString(v->second.IceinExtCoeff_R).c_str(), Filename);
			WritePrivateProfileStringA(v->first.c_str(), "Icereflectivity", ToString(v->second.Icereflectivity).c_str(), Filename);
			WritePrivateProfileStringA(v->first.c_str(), "IceshoreFactor", ToString(v->second.IceshoreFactor).c_str(), Filename);
			WritePrivateProfileStringA(v->first.c_str(), "Iceturbidity", ToString(v->second.Iceturbidity).c_str(), Filename);
			WritePrivateProfileStringA(v->first.c_str(), "IcewaveWidth", ToString(v->second.IcewaveWidth).c_str(), Filename);
			WritePrivateProfileStringA(v->first.c_str(), "inExtCoeff_B", ToString(v->second.inExtCoeff_B).c_str(), Filename);
			WritePrivateProfileStringA(v->first.c_str(), "inExtCoeff_G", ToString(v->second.inExtCoeff_G).c_str(), Filename);
			WritePrivateProfileStringA(v->first.c_str(), "inExtCoeff_R", ToString(v->second.inExtCoeff_R).c_str(), Filename);
			WritePrivateProfileStringA(v->first.c_str(), "inScattCoeff", ToString(v->second.inScattCoeff).c_str(), Filename);
			WritePrivateProfileStringA(v->first.c_str(), "LensAmount", ToString(v->second.LensAmount).c_str(), Filename);
			WritePrivateProfileStringA(v->first.c_str(), "LensTime", ToString(v->second.LensTime).c_str(), Filename);
			WritePrivateProfileStringA(v->first.c_str(), "LensTimeMultA", ToString(v->second.LensTimeMultA).c_str(), Filename);
			WritePrivateProfileStringA(v->first.c_str(), "LensTimeMultB", ToString(v->second.LensTimeMultB).c_str(), Filename);
			WritePrivateProfileStringA(v->first.c_str(), "LensViscosity", ToString(v->second.LensViscosity).c_str(), Filename);
			WritePrivateProfileStringA(v->first.c_str(), "LODdistance", ToString(v->second.LODdistance).c_str(), Filename);
			WritePrivateProfileStringA(v->first.c_str(), "MinLOD", ToString(v->second.MinLOD).c_str(), Filename);
			WritePrivateProfileStringA(v->first.c_str(), "reflectivity", ToString(v->second.reflectivity).c_str(), Filename);
			WritePrivateProfileStringA(v->first.c_str(), "shoreFactor", ToString(v->second.shoreFactor).c_str(), Filename);
			WritePrivateProfileStringA(v->first.c_str(), "turbidity", ToString(v->second.turbidity).c_str(), Filename);
			WritePrivateProfileStringA(v->first.c_str(), "waveSpeed", ToString(v->second.waveSpeed).c_str(), Filename);
			WritePrivateProfileStringA(v->first.c_str(), "waveWidth", ToString(v->second.waveWidth).c_str(), Filename);
			v++;
		}
	}
	else if (!strcmp(Name, "WaterLens")) {
		WritePrivateProfileStringA("Effects", "EnableWaterLens", ToString(SettingsMain.EnableWaterLens).c_str(), SettingsMain.MainSettingsFullFile);
	}
	else if (!strcmp(Name, "WetWorld")) {
		WritePrivateProfileStringA("Effects", "EnableWetWorld", ToString(SettingsMain.EnableWetWorld).c_str(), SettingsMain.MainSettingsFullFile);
	}
}

SettingsList SettingManager::GetShaders() {
	
	SettingsList Shaders;

	Shaders["AmbientOcclusion"] = "Ambient Occlusion";
	Shaders["Blood"] = "Blood";
	Shaders["BloodLens"] = "Blood Lens";
	Shaders["Bloom"] = "Bloom";
	Shaders["Cinema"] = "Cinema";
	Shaders["Coloring"] = "Coloring";
	Shaders["DepthOfField"] = "Depth Of Field";
	Shaders["GodRays"] = "God Rays";
	Shaders["Grass"] = "Grass";
	Shaders["HDR"] = "High Dynamic Range";
	Shaders["LowHF"] = "Low Health and Fatigue";
	Shaders["MotionBlur"] = "Motion Blur";
	Shaders["POM"] = "Parallax Occlusion Mapping";
	Shaders["Precipitations"] = "Precipitations";
	Shaders["Sharpening"] = "Sharpening";
	Shaders["SMAA"] = "Subpixel Morphological AA";
	Shaders["Shadows"] = "Shadows";
	Shaders["SnowAccumulation"] = "Snow Accumulation";
	Shaders["Skin"] = "Skin";
	Shaders["Terrain"] = "Terrain";
	Shaders["Underwater"] = "Underwater";
	Shaders["Water"] = "Water";
	Shaders["WaterLens"] = "Water Lens";
	Shaders["WetWorld"] = "Wet World";
	return Shaders;
}

SettingsList SettingManager::GetSections(const char* Name) {
	
	SettingsList Sections;

	if (!strcmp(Name, "AmbientOcclusion")) {
		SettingsAmbientOcclusionList::iterator v = SettingsAmbientOcclusion.begin();
		while (v != SettingsAmbientOcclusion.end()) {
			Sections[v->first] = "";
			v++;
		}
	}
	else if (!strcmp(Name, "Bloom")) {
		SettingsBloomList::iterator v = SettingsBloom.begin();
		while (v != SettingsBloom.end()) {
			Sections[v->first] = "";
			v++;
		}
	}
	else if (!strcmp(Name, "Coloring")) {
		SettingsColoringList::iterator v = SettingsColoring.begin();
		while (v != SettingsColoring.end()) {
			Sections[v->first] = "";
			v++;
		}
	}
	else if (!strcmp(Name, "DepthOfField")) {
		SettingsDepthOfFieldList::iterator v = SettingsDepthOfField.begin();
		while (v != SettingsDepthOfField.end()) {
			Sections[v->first] = "";
			v++;
		}
	}
	else if (!strcmp(Name, "MotionBlur")) {
		SettingsMotionBlurList::iterator v = SettingsMotionBlur.begin();
		while (v != SettingsMotionBlur.end()) {
			Sections[v->first] = "";
			v++;
		}
	}
	else if (!strcmp(Name, "Water")) {
		SettingsWaterList::iterator v = SettingsWater.begin();
		while (v != SettingsWater.end()) {
			Sections[v->first] = "";
			v++;
		}
	}
	else if (!strcmp(Name, "BloodLens")) {
		Sections["Configure these settings by the Blood menu"] = "";
	}
	else if (!strcmp(Name, "SnowAccumulation") || !strcmp(Name, "WetWorld")) {
		Sections["Configure these settings by the Precipitations menu"] = "";
	}
	else if (!strcmp(Name, "SMAA")) {
		Sections["No settings to configure"] = "";
	}
	else if (!strcmp(Name, "Underwater") || !strcmp(Name, "WaterLens")) {
		Sections["Configure these settings by the Water menu"] = "";
	}
	else
		Sections["Default"] = "";

	return Sections;
}

SettingsList SettingManager::GetSettings(const char* Name, const char* Section) {
	
	SettingsList SettingsShader;

	if (!strcmp(Name, "AmbientOcclusion")) {
		SettingsAmbientOcclusionStruct *sas = GetSettingsAmbientOcclusion(Section);
		SettingsShader["AngleBias"] = sas->AngleBias;
		SettingsShader["BlurDropThreshold"] = sas->BlurDropThreshold;
		SettingsShader["BlurRadiusMultiplier"] = sas->BlurRadiusMultiplier;
		SettingsShader["ClampStrength"] = sas->ClampStrength;
		SettingsShader["Enabled"] = sas->Enabled;
		SettingsShader["LumThreshold"] = sas->LumThreshold;
		SettingsShader["RadiusMultiplier"] = sas->RadiusMultiplier;
		SettingsShader["Range"] = sas->Range;
		SettingsShader["StrengthMultiplier"] = sas->StrengthMultiplier;
	}
	else if (!strcmp(Name, "Blood")) {
		SettingsShader["LensChance"] = SettingsBlood.LensChance;
		SettingsShader["LensColorR"] = SettingsBlood.LensColorR;
		SettingsShader["LensColorG"] = SettingsBlood.LensColorG;
		SettingsShader["LensColorB"] = SettingsBlood.LensColorB;
		SettingsShader["LensIntensity"] = SettingsBlood.LensIntensity;
		SettingsShader["LensTime"] = SettingsBlood.LensTime;
	}
	else if (!strcmp(Name, "Bloom")) {
		SettingsBloomStruct *sbs = GetSettingsBloom(Section);
		SettingsShader["BloomIntensity"] = sbs->BloomIntensity;
		SettingsShader["BloomSaturation"] = sbs->BloomSaturation;
		SettingsShader["Luminance"] = sbs->Luminance;
		SettingsShader["MiddleGray"] = sbs->MiddleGray;
		SettingsShader["OriginalIntensity"] = sbs->OriginalIntensity;
		SettingsShader["OriginalSaturation"] = sbs->OriginalSaturation;
		SettingsShader["WhiteCutOff"] = sbs->WhiteCutOff;
	}
	else if (!strcmp(Name, "Cinema")) {
		SettingsShader["AspectRatio"] = SettingsCinema.AspectRatio;
		SettingsShader["DialogMode"] = SettingsCinema.DialogMode;
		SettingsShader["VignetteDarkness"] = SettingsCinema.VignetteDarkness;
		SettingsShader["VignetteRadius"] = SettingsCinema.VignetteRadius;
	}
	else if (!strcmp(Name, "Coloring")) {
		SettingsColoringStruct *scs = GetSettingsColoring(Section);
		SettingsShader["BaseGamma"] = scs->BaseGamma;
		SettingsShader["Bleach"] = scs->Bleach;
		SettingsShader["BleachLuma"] = scs->BleachLuma;
		SettingsShader["ColorCurve"] = scs->ColorCurve;
		SettingsShader["ColorCurveB"] = scs->ColorCurveB;
		SettingsShader["ColorCurveG"] = scs->ColorCurveG;
		SettingsShader["ColorCurveR"] = scs->ColorCurveR;
		SettingsShader["Contrast"] = scs->Contrast;
		SettingsShader["EffectGamma"] = scs->EffectGamma;
		SettingsShader["EffectGammaB"] = scs->EffectGammaB;
		SettingsShader["EffectGammaG"] = scs->EffectGammaG;
		SettingsShader["EffectGammaR"] = scs->EffectGammaR;
		SettingsShader["Fade"] = scs->Fade;
		SettingsShader["Linearization"] = scs->Linearization;
		SettingsShader["Saturation"] = scs->Saturation;
		SettingsShader["Strength"] = scs->Strength;
	}
	else if (!strcmp(Name, "DepthOfField")) {
		SettingsDepthOfFieldStruct *sds = GetSettingsDepthOfField(Section);
		SettingsShader["BaseBlurRadius"] = sds->BaseBlurRadius;
		SettingsShader["BlurFallOff"] = sds->BlurFallOff;
		SettingsShader["DialogMode"] = sds->DialogMode;
		SettingsShader["DiameterRange"] = sds->DiameterRange;
		SettingsShader["DistantBlur"] = sds->DistantBlur;
		SettingsShader["DistantBlurEndRange"] = sds->DistantBlurEndRange;
		SettingsShader["DistantBlurStartRange"] = sds->DistantBlurStartRange;
		SettingsShader["Enabled"] = sds->Enabled;
		SettingsShader["NearBlurCutOff"] = sds->NearBlurCutOff;
		SettingsShader["Radius"] = sds->Radius;
	}
	else if (!strcmp(Name, "GodRays")) {
		SettingsShader["GlobalMultiplier"] = SettingsGodRays.GlobalMultiplier;
		SettingsShader["LightShaftPasses"] = SettingsGodRays.LightShaftPasses;
		SettingsShader["Luminance"] = SettingsGodRays.Luminance;
		SettingsShader["RayDensity"] = SettingsGodRays.RayDensity;
		SettingsShader["RayIntensity"] = SettingsGodRays.RayIntensity;
		SettingsShader["RayLength"] = SettingsGodRays.RayLength;
		SettingsShader["RayVisibility"] = SettingsGodRays.RayVisibility;
		SettingsShader["RayR"] = SettingsGodRays.RayR;
		SettingsShader["RayG"] = SettingsGodRays.RayG;
		SettingsShader["RayB"] = SettingsGodRays.RayB;
		SettingsShader["Saturate"] = SettingsGodRays.Saturate;
		SettingsShader["SunGlareEnabled"] = SettingsGodRays.SunGlareEnabled;
	}
	else if (!strcmp(Name, "Grass")) {
		SettingsShader["GrassDensity"] = SettingsGrass.GrassDensity;
		SettingsShader["MaxDistance"] = SettingsGrass.MaxDistance;
		SettingsShader["MinDistance"] = SettingsGrass.MinDistance;
		SettingsShader["MinHeight"] = SettingsGrass.MinHeight;	
		SettingsShader["ScaleX"] = SettingsGrass.ScaleX;	
		SettingsShader["ScaleY"] = SettingsGrass.ScaleY;	
		SettingsShader["ScaleZ"] = SettingsGrass.ScaleZ;	
		SettingsShader["WindCoefficient"] = SettingsGrass.WindCoefficient;	
		SettingsShader["WindEnabled"] = SettingsGrass.WindEnabled;	
	}
	else if (!strcmp(Name, "HDR")) {
		SettingsShader["ToneMapping"] = SettingsHDR.ToneMapping;
		SettingsShader["ToneMappingBlur"] = SettingsHDR.ToneMappingBlur;
		SettingsShader["ToneMappingColor"] = SettingsHDR.ToneMappingColor;
	}
	else if (!strcmp(Name, "LowHF")) {
		SettingsShader["BlurMultiplier"] = SettingsLowHF.BlurMultiplier;
		SettingsShader["DarknessMultiplier"] = SettingsLowHF.DarknessMultiplier;
		SettingsShader["FatigueLimit"] = SettingsLowHF.FatigueLimit;
		SettingsShader["HealthLimit"] = SettingsLowHF.HealthLimit;
		SettingsShader["LumaMultiplier"] = SettingsLowHF.LumaMultiplier;	
		SettingsShader["VignetteMultiplier"] = SettingsLowHF.VignetteMultiplier;	
	}
	else if (!strcmp(Name, "MotionBlur")) {
		SettingsMotionBlurStruct *sms = GetSettingsMotionBlur(Section);
		SettingsShader["BlurCutOff"] = sms->BlurCutOff;
		SettingsShader["BlurOffsetMax"] = sms->BlurOffsetMax;
		SettingsShader["BlurScale"] = sms->BlurScale;
		SettingsShader["Enabled"] = sms->Enabled;
		SettingsShader["GaussianWeight"] = sms->GaussianWeight;
	}
	else if (!strcmp(Name, "POM")) {
		SettingsShader["HeightMapScale"] = SettingsPOM.HeightMapScale;
		SettingsShader["MaxSamples"] = SettingsPOM.MaxSamples;
		SettingsShader["MinSamples"] = SettingsPOM.MinSamples;
		SettingsShader["ShadowSoftening"] = SettingsPOM.ShadowSoftening;	
	}
	else if (!strcmp(Name, "Precipitations")) {
		SettingsShader["Intensity"] = SettingsPrecipitations.Intensity;
		SettingsShader["Size"] = SettingsPrecipitations.Size;
		SettingsShader["Velocity"] = SettingsPrecipitations.Velocity;
		SettingsShader["PuddleCoeff_B"] = SettingsPrecipitations.PuddleCoeff_B;
		SettingsShader["PuddleCoeff_G"] = SettingsPrecipitations.PuddleCoeff_G;
		SettingsShader["PuddleCoeff_R"] = SettingsPrecipitations.PuddleCoeff_R;
		SettingsShader["PuddleSpecularMultiplier"] = SettingsPrecipitations.PuddleSpecularMultiplier;
		SettingsShader["RainAmount"] = SettingsPrecipitations.RainAmount;
		SettingsShader["RainDecrease"] = SettingsPrecipitations.RainDecrease;
		SettingsShader["RainIncrease"] = SettingsPrecipitations.RainIncrease;
		SettingsShader["SnowAmount"] = SettingsPrecipitations.SnowAmount;
		SettingsShader["SnowBlurNormDropThreshhold"] = SettingsPrecipitations.SnowBlurNormDropThreshhold;
		SettingsShader["SnowBlurRadiusMultiplier"] = SettingsPrecipitations.SnowBlurRadiusMultiplier;
		SettingsShader["SnowDecrease"] = SettingsPrecipitations.SnowDecrease;
		SettingsShader["SnowIncrease"] = SettingsPrecipitations.SnowIncrease;
		SettingsShader["SnowSunPower"] = SettingsPrecipitations.SnowSunPower;
	}
	else if (!strcmp(Name, "Shadows")) {
		SettingsShader["SelfShadowIntensity"] = SettingsShadows.SelfShadowIntensity;
		SettingsShader["ShadowIntensity"] = SettingsShadows.ShadowIntensity;
	}
	else if (!strcmp(Name, "Sharpening")) {
		SettingsShader["Strength"] = SettingsSharpening.Strength;
		SettingsShader["Clamp"] = SettingsSharpening.Clamp;
		SettingsShader["Offset"] = SettingsSharpening.Offset;
	}
	else if (!strcmp(Name, "Skin")) {
		SettingsShader["Attenuation"] = SettingsSkin.Attenuation;
		SettingsShader["ExtCoeffBlue"] = SettingsSkin.ExtCoeffBlue;
		SettingsShader["ExtCoeffGreen"] = SettingsSkin.ExtCoeffGreen;
		SettingsShader["ExtCoeffRed"] = SettingsSkin.ExtCoeffRed;
		SettingsShader["MaterialThickness"] = SettingsSkin.MaterialThickness;
		SettingsShader["RimScalar"] = SettingsSkin.RimScalar;
		SettingsShader["SpecularPower"] = SettingsSkin.SpecularPower;
	}
	else if (!strcmp(Name, "Terrain")) {
		SettingsShader["DistantNoise"] = SettingsTerrain.DistantNoise;
		SettingsShader["DistantSpecular"] = SettingsTerrain.DistantSpecular;
		SettingsShader["MiddleSpecular"] = SettingsTerrain.MiddleSpecular;
		SettingsShader["NearSpecular"] = SettingsTerrain.NearSpecular;
	}
	else if (!strcmp(Name, "Water")) {
		SettingsWaterStruct *sws = GetSettingsWater(Section);
		SettingsShader["causticsStrength"] = sws->causticsStrength;
		SettingsShader["causticsStrengthS"] = sws->causticsStrengthS;
		SettingsShader["choppiness"] = sws->choppiness;
		SettingsShader["depthDarkness"] = sws->depthDarkness;
		SettingsShader["Icechoppiness"] = sws->Icechoppiness;
		SettingsShader["IceEnabled"] = sws->IceEnabled;
		SettingsShader["IceinExtCoeff_B"] = sws->IceinExtCoeff_B;
		SettingsShader["IceinExtCoeff_G"] = sws->IceinExtCoeff_G;
		SettingsShader["IceinExtCoeff_R"] = sws->IceinExtCoeff_R;
		SettingsShader["Icereflectivity"] = sws->Icereflectivity;
		SettingsShader["IceshoreFactor"] = sws->IceshoreFactor;
		SettingsShader["Iceturbidity"] = sws->Iceturbidity;
		SettingsShader["IcewaveWidth"] = sws->IcewaveWidth;
		SettingsShader["inExtCoeff_B"] = sws->inExtCoeff_B;
		SettingsShader["inExtCoeff_G"] = sws->inExtCoeff_G;
		SettingsShader["inExtCoeff_R"] = sws->inExtCoeff_R;
		SettingsShader["inScattCoeff"] = sws->inScattCoeff;
		SettingsShader["LensAmount"] = sws->LensAmount;
		SettingsShader["LensTime"] = sws->LensTime;
		SettingsShader["LensTimeMultA"] = sws->LensTimeMultA;
		SettingsShader["LensTimeMultB"] = sws->LensTimeMultB;
		SettingsShader["LensViscosity"] = sws->LensViscosity;
		SettingsShader["LODdistance"] = sws->LODdistance;
		SettingsShader["MinLOD"] = sws->MinLOD;
		SettingsShader["reflectivity"] = sws->reflectivity;
		SettingsShader["shoreFactor"] = sws->shoreFactor;
		SettingsShader["turbidity"] = sws->turbidity;
		SettingsShader["waveSpeed"] = sws->waveSpeed;
		SettingsShader["waveWidth"] = sws->waveWidth;
	}
	return SettingsShader;

}

float SettingManager::GetSetting(const char* Name, const char* Section, const char* Setting) {
	
	float Value;

	if (!strcmp(Name, "AmbientOcclusion")) {
		SettingsAmbientOcclusionStruct *sas = GetSettingsAmbientOcclusion(Section);
		if (!strcmp(Setting, "AngleBias"))
			Value = sas->AngleBias;
		else if (!strcmp(Setting, "BlurDropThreshold"))
			Value = sas->BlurDropThreshold;
		else if (!strcmp(Setting, "BlurRadiusMultiplier"))
			Value = sas->BlurRadiusMultiplier;
		else if (!strcmp(Setting, "ClampStrength"))
			Value = sas->ClampStrength;
		else if (!strcmp(Setting, "Enabled"))
			Value = sas->Enabled;
		else if (!strcmp(Setting, "LumThreshold"))
			Value = sas->LumThreshold;
		else if (!strcmp(Setting, "RadiusMultiplier"))
			Value = sas->RadiusMultiplier;
		else if (!strcmp(Setting, "Range"))
			Value = sas->Range;
		else if (!strcmp(Setting, "StrengthMultiplier"))
			Value = sas->StrengthMultiplier;
	}
	else if (!strcmp(Name, "Blood")) {
		if (!strcmp(Setting, "LensChance"))
			Value = SettingsBlood.LensChance;
		else if (!strcmp(Setting, "LensColorR"))
			Value = SettingsBlood.LensColorR;
		else if (!strcmp(Setting, "LensColorG"))
			Value = SettingsBlood.LensColorG;
		else if (!strcmp(Setting, "LensColorB"))
			Value = SettingsBlood.LensColorB;
		else if (!strcmp(Setting, "LensIntensity"))
			Value = SettingsBlood.LensIntensity;
		else if (!strcmp(Setting, "LensTime"))
			Value = SettingsBlood.LensTime;
	}
	else if (!strcmp(Name, "Bloom")) {
		SettingsBloomStruct *sbs = GetSettingsBloom(Section);
		if (!strcmp(Setting, "BloomIntensity"))
			Value = sbs->BloomIntensity;
		else if (!strcmp(Setting, "BloomSaturation"))
			Value = sbs->BloomSaturation;
		else if (!strcmp(Setting, "Luminance"))
			Value = sbs->Luminance;
		else if (!strcmp(Setting, "MiddleGray"))
			Value = sbs->MiddleGray;
		else if (!strcmp(Setting, "OriginalIntensity"))
			Value = sbs->OriginalIntensity;
		else if (!strcmp(Setting, "OriginalSaturation"))
			Value = sbs->OriginalSaturation;
		else if (!strcmp(Setting, "WhiteCutOff"))
			Value = sbs->WhiteCutOff;
	}
	else if (!strcmp(Name, "Cinema")) {
		if (!strcmp(Setting, "AspectRatio"))
			Value = SettingsCinema.AspectRatio;
		else if (!strcmp(Setting, "DialogMode"))
			Value = SettingsCinema.DialogMode;
		else if (!strcmp(Setting, "VignetteDarkness"))
			Value = SettingsCinema.VignetteDarkness;
		else if (!strcmp(Setting, "VignetteRadius"))
			Value = SettingsCinema.VignetteRadius;
	}
	else if (!strcmp(Name, "Coloring")) {
		SettingsColoringStruct *scs = GetSettingsColoring(Section);
		if (!strcmp(Setting, "BaseGamma"))
			Value = scs->BaseGamma;
		else if (!strcmp(Setting, "Bleach"))
			Value = scs->Bleach;
		else if (!strcmp(Setting, "BleachLuma"))
			Value = scs->BleachLuma;
		else if (!strcmp(Setting, "ColorCurve"))
			Value = scs->ColorCurve;
		else if (!strcmp(Setting, "ColorCurveB"))
			Value = scs->ColorCurveB;
		else if (!strcmp(Setting, "ColorCurveG"))
			Value = scs->ColorCurveG;
		else if (!strcmp(Setting, "ColorCurveR"))
			Value = scs->ColorCurveR;
		else if (!strcmp(Setting, "Contrast"))
			Value = scs->Contrast;
		else if (!strcmp(Setting, "EffectGamma"))
			Value = scs->EffectGamma;
		else if (!strcmp(Setting, "EffectGammaB"))
			Value = scs->EffectGammaB;
		else if (!strcmp(Setting, "EffectGammaG"))
			Value = scs->EffectGammaG;
		else if (!strcmp(Setting, "EffectGammaR"))
			Value = scs->EffectGammaR;
		else if (!strcmp(Setting, "Fade"))
			Value = scs->Fade;
		else if (!strcmp(Setting, "Linearization"))
			Value = scs->Linearization;
		else if (!strcmp(Setting, "Saturation"))
			Value = scs->Saturation;
		else if (!strcmp(Setting, "Strength"))
			Value = scs->Strength;
	}
	else if (!strcmp(Name, "DepthOfField")) {
		SettingsDepthOfFieldStruct *sds = GetSettingsDepthOfField(Section);
		if (!strcmp(Setting, "BaseBlurRadius"))
			Value = sds->BaseBlurRadius;
		else if (!strcmp(Setting, "BlurFallOff"))
			Value = sds->BlurFallOff;
		else if (!strcmp(Setting, "DialogMode"))
			Value = sds->DialogMode;
		else if (!strcmp(Setting, "DiameterRange"))
			Value = sds->DiameterRange;
		else if (!strcmp(Setting, "DistantBlur"))
			Value = sds->DistantBlur;
		else if (!strcmp(Setting, "DistantBlurEndRange"))
			Value = sds->DistantBlurEndRange;
		else if (!strcmp(Setting, "DistantBlurStartRange"))
			Value = sds->DistantBlurStartRange;
		else if (!strcmp(Setting, "Enabled"))
			Value = sds->Enabled;
		else if (!strcmp(Setting, "NearBlurCutOff"))
			Value = sds->NearBlurCutOff;
		else if (!strcmp(Setting, "Radius"))
			Value = sds->Radius;
	}
	else if (!strcmp(Name, "GodRays")) {
		if (!strcmp(Setting, "RayDensity"))
			Value = SettingsGodRays.RayDensity;
		else if (!strcmp(Setting, "RayIntensity"))
			Value = SettingsGodRays.RayIntensity;
		else if (!strcmp(Setting, "RayLength"))
			Value = SettingsGodRays.RayLength;
		else if (!strcmp(Setting, "GlobalMultiplier"))
			Value = SettingsGodRays.GlobalMultiplier;
		else if (!strcmp(Setting, "Saturate"))
			Value = SettingsGodRays.Saturate;
		else if (!strcmp(Setting, "LightShaftPasses"))
			Value = SettingsGodRays.LightShaftPasses;
		else if (!strcmp(Setting, "Luminance"))
			Value = SettingsGodRays.Luminance;
		else if (!strcmp(Setting, "RayR"))
			Value = SettingsGodRays.RayR;
		else if (!strcmp(Setting, "RayG"))
			Value = SettingsGodRays.RayG;
		else if (!strcmp(Setting, "RayB"))
			Value = SettingsGodRays.RayB;
		else if (!strcmp(Setting, "SunGlareEnabled"))
			Value = SettingsGodRays.SunGlareEnabled;
		else if (!strcmp(Setting, "RayVisibility"))
			Value = SettingsGodRays.RayVisibility;
	}
	else if (!strcmp(Name, "Grass")) {
		if (!strcmp(Setting, "GrassDensity"))
			Value = SettingsGrass.GrassDensity;
		else if (!strcmp(Setting, "MaxDistance"))
			Value = SettingsGrass.MaxDistance;
		else if (!strcmp(Setting, "MinDistance"))
			Value = SettingsGrass.MinDistance;
		else if (!strcmp(Setting, "MinHeight"))
			Value = SettingsGrass.MinHeight;	
		else if (!strcmp(Setting, "ScaleX"))
			Value = SettingsGrass.ScaleX;	
		else if (!strcmp(Setting, "ScaleY"))
			Value = SettingsGrass.ScaleY;	
		else if (!strcmp(Setting, "ScaleZ"))
			Value = SettingsGrass.ScaleZ;	
		else if (!strcmp(Setting, "WindCoefficient"))
			Value = SettingsGrass.WindCoefficient;	
		else if (!strcmp(Setting, "WindEnabled"))
			Value = SettingsGrass.WindEnabled;	
	}
	else if (!strcmp(Name, "HDR")) {
		if (!strcmp(Setting, "ToneMapping"))
			Value = SettingsHDR.ToneMapping;
		else if (!strcmp(Setting, "ToneMappingBlur"))
			Value = SettingsHDR.ToneMappingBlur;
		else if (!strcmp(Setting, "ToneMappingColor"))
			Value = SettingsHDR.ToneMappingColor;
	}
	else if (!strcmp(Name, "LowHF")) {
		if (!strcmp(Setting, "BlurMultiplier"))
			Value = SettingsLowHF.BlurMultiplier;
		else if (!strcmp(Setting, "DarknessMultiplier"))
			Value = SettingsLowHF.DarknessMultiplier;
		else if (!strcmp(Setting, "FatigueLimit"))
			Value = SettingsLowHF.FatigueLimit;
		else if (!strcmp(Setting, "HealthLimit"))
			Value = SettingsLowHF.HealthLimit;
		else if (!strcmp(Setting, "LumaMultiplier"))
			Value = SettingsLowHF.LumaMultiplier;
		else if (!strcmp(Setting, "VignetteMultiplier"))
			Value = SettingsLowHF.VignetteMultiplier;
	}
	else if (!strcmp(Name, "MotionBlur")) {
		SettingsMotionBlurStruct *sms = GetSettingsMotionBlur(Section);
		if (!strcmp(Setting, "BlurCutOff"))
			Value = sms->BlurCutOff;
		else if (!strcmp(Setting, "BlurOffsetMax"))
			Value = sms->BlurOffsetMax;
		else if (!strcmp(Setting, "BlurScale"))
			Value = sms->BlurScale;
		else if (!strcmp(Setting, "Enabled"))
			Value = sms->Enabled;
		else if (!strcmp(Setting, "GaussianWeight"))
			Value = sms->GaussianWeight;
	}
	else if (!strcmp(Name, "POM")) {
		if (!strcmp(Setting, "HeightMapScale"))
			Value = SettingsPOM.HeightMapScale;
		else if (!strcmp(Setting, "MaxSamples"))
			Value = SettingsPOM.MaxSamples;
		else if (!strcmp(Setting, "MinSamples"))
			Value = SettingsPOM.MinSamples;
		else if (!strcmp(Setting, "ShadowSoftening"))
			Value = SettingsPOM.ShadowSoftening;	
	}
	else if (!strcmp(Name, "Precipitations")) {
		if (!strcmp(Setting, "Intensity"))
			Value = SettingsPrecipitations.Intensity;
		else if (!strcmp(Setting, "Size"))
			Value = SettingsPrecipitations.Size;
		else if (!strcmp(Setting, "Velocity"))
			Value = SettingsPrecipitations.Velocity;
		else if (!strcmp(Setting, "SnowIncrease"))
			Value = SettingsPrecipitations.SnowIncrease;
		else if (!strcmp(Setting, "SnowDecrease"))
			Value = SettingsPrecipitations.SnowDecrease;
		else if (!strcmp(Setting, "SnowSunPower"))
			Value = SettingsPrecipitations.SnowSunPower;
		else if (!strcmp(Setting, "SnowAmount"))
			Value = SettingsPrecipitations.SnowAmount;
		else if (!strcmp(Setting, "SnowBlurNormDropThreshhold"))
			Value = SettingsPrecipitations.SnowBlurNormDropThreshhold;
		else if (!strcmp(Setting, "SnowBlurRadiusMultiplier"))
			Value = SettingsPrecipitations.SnowBlurRadiusMultiplier;
		else if (!strcmp(Setting, "RainIncrease"))
			Value = SettingsPrecipitations.RainIncrease;
		else if (!strcmp(Setting, "RainDecrease"))
			Value = SettingsPrecipitations.RainDecrease;
		else if (!strcmp(Setting, "RainAmount"))
			Value = SettingsPrecipitations.RainAmount;
		else if (!strcmp(Setting, "PuddleCoeff_B"))
			Value = SettingsPrecipitations.PuddleCoeff_B;
		else if (!strcmp(Setting, "PuddleCoeff_G"))
			Value = SettingsPrecipitations.PuddleCoeff_G;
		else if (!strcmp(Setting, "PuddleCoeff_R"))
			Value = SettingsPrecipitations.PuddleCoeff_R;
		else if (!strcmp(Setting, "PuddleSpecularMultiplier"))
			Value = SettingsPrecipitations.PuddleSpecularMultiplier;
	}
	else if (!strcmp(Name, "Shadows")) {
		if (!strcmp(Setting, "SelfShadowIntensity"))
			Value = SettingsShadows.SelfShadowIntensity;
		else if (!strcmp(Setting, "ShadowIntensity"))
			Value = SettingsShadows.ShadowIntensity;
	}
	else if (!strcmp(Name, "Sharpening")) {
		if (!strcmp(Setting, "Clamp"))
			Value = SettingsSharpening.Clamp;
		else if (!strcmp(Setting, "Offset"))
			Value = SettingsSharpening.Offset;
		else if (!strcmp(Setting, "Strength"))
			Value = SettingsSharpening.Strength;
	}
	else if (!strcmp(Name, "Skin")) {
		if (!strcmp(Setting, "Attenuation"))
			Value = SettingsSkin.Attenuation;
		else if (!strcmp(Setting, "ExtCoeffBlue"))
			Value = SettingsSkin.ExtCoeffBlue;
		else if (!strcmp(Setting, "ExtCoeffGreen"))
			Value = SettingsSkin.ExtCoeffGreen;
		else if (!strcmp(Setting, "ExtCoeffRed"))
			Value = SettingsSkin.ExtCoeffRed;
		else if (!strcmp(Setting, "MaterialThickness"))
			Value = SettingsSkin.MaterialThickness;
		else if (!strcmp(Setting, "RimScalar"))
			Value = SettingsSkin.RimScalar;
		else if (!strcmp(Setting, "SpecularPower"))
			Value = SettingsSkin.SpecularPower;
	}
	else if (!strcmp(Name, "Terrain")) {
		if (!strcmp(Setting, "DistantNoise"))
			Value = SettingsTerrain.DistantNoise;
		else if (!strcmp(Setting, "DistantSpecular"))
			Value = SettingsTerrain.DistantSpecular;
		else if (!strcmp(Setting, "MiddleSpecular"))
			Value = SettingsTerrain.MiddleSpecular;
		else if (!strcmp(Setting, "NearSpecular"))
			Value = SettingsTerrain.NearSpecular;
	}
	else if (!strcmp(Name, "Water")) {
		SettingsWaterStruct *sws = GetSettingsWater(Section);
		if (!strcmp(Setting, "causticsStrength"))
			Value = sws->causticsStrength;
		else if (!strcmp(Setting, "causticsStrengthS"))
			Value = sws->causticsStrengthS;
		else if (!strcmp(Setting, "choppiness"))
			Value = sws->choppiness;
		else if (!strcmp(Setting, "depthDarkness"))
			Value = sws->depthDarkness;
		else if (!strcmp(Setting, "Icechoppiness"))
			Value = sws->Icechoppiness;
		else if (!strcmp(Setting, "IceEnabled"))
			Value = sws->IceEnabled;
		else if (!strcmp(Setting, "IceinExtCoeff_B"))
			Value = sws->IceinExtCoeff_B;
		else if (!strcmp(Setting, "IceinExtCoeff_G"))
			Value = sws->IceinExtCoeff_G;
		else if (!strcmp(Setting, "IceinExtCoeff_R"))
			Value = sws->IceinExtCoeff_R;
		else if (!strcmp(Setting, "Icereflectivity"))
			Value = sws->Icereflectivity;
		else if (!strcmp(Setting, "IceshoreFactor"))
			Value = sws->IceshoreFactor;
		else if (!strcmp(Setting, "Iceturbidity"))
			Value = sws->Iceturbidity;
		else if (!strcmp(Setting, "IcewaveWidth"))
			Value = sws->IcewaveWidth;
		else if (!strcmp(Setting, "inExtCoeff_B"))
			Value = sws->inExtCoeff_B;
		else if (!strcmp(Setting, "inExtCoeff_G"))
			Value = sws->inExtCoeff_G;
		else if (!strcmp(Setting, "inExtCoeff_R"))
			Value = sws->inExtCoeff_R;
		else if (!strcmp(Setting, "inScattCoeff"))
			Value = sws->inScattCoeff;
		else if (!strcmp(Setting, "LensAmount"))
			Value = sws->LensAmount;
		else if (!strcmp(Setting, "LensTime"))
			Value = sws->LensTime;
		else if (!strcmp(Setting, "LensTimeMultA"))
			Value = sws->LensTimeMultA;
		else if (!strcmp(Setting, "LensTimeMultB"))
			Value = sws->LensTimeMultB;
		else if (!strcmp(Setting, "LensViscosity"))
			Value = sws->LensViscosity;
		else if (!strcmp(Setting, "LODdistance"))
			Value = sws->LODdistance;
		else if (!strcmp(Setting, "MinLOD"))
			Value = sws->MinLOD;
		else if (!strcmp(Setting, "reflectivity"))
			Value = sws->reflectivity;
		else if (!strcmp(Setting, "shoreFactor"))
			Value = sws->shoreFactor;
		else if (!strcmp(Setting, "turbidity"))
			Value = sws->turbidity;
		else if (!strcmp(Setting, "waveSpeed"))
			Value = sws->waveSpeed;
		else if (!strcmp(Setting, "waveWidth"))
			Value = sws->waveWidth;
	}
	return Value;
}

void SettingManager::SetSetting(const char* Name, const char* Section, const char* Setting, float Value) {
	
	if (!strcmp(Name, "AmbientOcclusion")) {
		SettingsAmbientOcclusionStruct *sas = GetSettingsAmbientOcclusion(Section);
		if (!strcmp(Setting, "AngleBias"))
			sas->AngleBias = Value;
		else if (!strcmp(Setting, "BlurDropThreshold"))
			sas->BlurDropThreshold = Value;
		else if (!strcmp(Setting, "BlurRadiusMultiplier"))
			sas->BlurRadiusMultiplier = Value;
		else if (!strcmp(Setting, "ClampStrength"))
			sas->ClampStrength = Value;
		else if (!strcmp(Setting, "Enabled"))
			sas->Enabled = Value;
		else if (!strcmp(Setting, "LumThreshold"))
			sas->LumThreshold = Value;
		else if (!strcmp(Setting, "RadiusMultiplier"))
			sas->RadiusMultiplier = Value;
		else if (!strcmp(Setting, "Range"))
			sas->Range = Value;
		else if (!strcmp(Setting, "StrengthMultiplier"))
			sas->StrengthMultiplier = Value;	
	}
	else if (!strcmp(Name, "Blood")) {
		if (!strcmp(Setting, "LensChance"))
			SettingsBlood.LensChance = Value;
		else if (!strcmp(Setting, "LensColorR"))
			SettingsBlood.LensColorR = Value;
		else if (!strcmp(Setting, "LensColorG"))
			SettingsBlood.LensColorG = Value;
		else if (!strcmp(Setting, "LensColorB"))
			SettingsBlood.LensColorB = Value;
		else if (!strcmp(Setting, "LensIntensity"))
			SettingsBlood.LensIntensity = Value;
		else if (!strcmp(Setting, "LensTime"))
			SettingsBlood.LensTime = Value;	
	}
	else if (!strcmp(Name, "Bloom")) {
		SettingsBloomStruct *sbs = GetSettingsBloom(Section);
		if (!strcmp(Setting, "BloomIntensity"))
			sbs->BloomIntensity = Value;
		else if (!strcmp(Setting, "BloomSaturation"))
			sbs->BloomSaturation = Value;
		else if (!strcmp(Setting, "Luminance"))
			sbs->Luminance = Value;
		else if (!strcmp(Setting, "MiddleGray"))
			sbs->MiddleGray = Value;
		else if (!strcmp(Setting, "OriginalIntensity"))
			sbs->OriginalIntensity = Value;
		else if (!strcmp(Setting, "OriginalSaturation"))
			sbs->OriginalSaturation = Value;
		else if (!strcmp(Setting, "WhiteCutOff"))
			sbs->WhiteCutOff = Value;	
	}
	else if (!strcmp(Name, "Cinema")) {
		if (!strcmp(Setting, "AspectRatio"))
			SettingsCinema.AspectRatio = Value;
		else if (!strcmp(Setting, "DialogMode"))
			SettingsCinema.DialogMode = Value;
		else if (!strcmp(Setting, "VignetteDarkness"))
			SettingsCinema.VignetteDarkness = Value;
		else if (!strcmp(Setting, "VignetteRadius"))
			SettingsCinema.VignetteRadius = Value;	
	}
	else if (!strcmp(Name, "Coloring")) {
		SettingsColoringStruct *scs = GetSettingsColoring(Section);
		if (!strcmp(Setting, "BaseGamma"))
			scs->BaseGamma = Value;
		else if (!strcmp(Setting, "Bleach"))
			scs->Bleach = Value;
		else if (!strcmp(Setting, "BleachLuma"))
			scs->BleachLuma = Value;
		else if (!strcmp(Setting, "ColorCurve"))
			scs->ColorCurve = Value;
		else if (!strcmp(Setting, "ColorCurveB"))
			scs->ColorCurveB = Value;
		else if (!strcmp(Setting, "ColorCurveG"))
			scs->ColorCurveG = Value;
		else if (!strcmp(Setting, "ColorCurveR"))
			scs->ColorCurveR = Value;
		else if (!strcmp(Setting, "Contrast"))
			scs->Contrast = Value;
		else if (!strcmp(Setting, "EffectGamma"))
			scs->EffectGamma = Value;
		else if (!strcmp(Setting, "EffectGammaB"))
			scs->EffectGammaB = Value;
		else if (!strcmp(Setting, "EffectGammaG"))
			scs->EffectGammaG = Value;
		else if (!strcmp(Setting, "EffectGammaR"))
			scs->EffectGammaR = Value;
		else if (!strcmp(Setting, "Fade"))
			scs->Fade = Value;
		else if (!strcmp(Setting, "Linearization"))
			scs->Linearization = Value;
		else if (!strcmp(Setting, "Saturation"))
			scs->Saturation = Value;
		else if (!strcmp(Setting, "Strength"))
			scs->Strength = Value;	
	}
	else if (!strcmp(Name, "DepthOfField")) {
		SettingsDepthOfFieldStruct *sds = GetSettingsDepthOfField(Section);
		if (!strcmp(Setting, "BaseBlurRadius"))
			sds->BaseBlurRadius = Value;
		else if (!strcmp(Setting, "BlurFallOff"))
			sds->BlurFallOff = Value;
		else if (!strcmp(Setting, "DialogMode"))
			sds->DialogMode = Value;
		else if (!strcmp(Setting, "DiameterRange"))
			sds->DiameterRange = Value;
		else if (!strcmp(Setting, "DistantBlur"))
			sds->DistantBlur = Value;
		else if (!strcmp(Setting, "DistantBlurEndRange"))
			sds->DistantBlurEndRange = Value;
		else if (!strcmp(Setting, "DistantBlurStartRange"))
			sds->DistantBlurStartRange = Value;
		else if (!strcmp(Setting, "Enabled"))
			sds->Enabled = Value;
		else if (!strcmp(Setting, "NearBlurCutOff"))
			sds->NearBlurCutOff = Value;
		else if (!strcmp(Setting, "Radius"))
			sds->Radius = Value;	
	}
	else if (!strcmp(Name, "GodRays")) {
		if (!strcmp(Setting, "RayDensity"))
			SettingsGodRays.RayDensity = Value;
		else if (!strcmp(Setting, "RayIntensity"))
			SettingsGodRays.RayIntensity = Value;
		else if (!strcmp(Setting, "RayLength"))
			SettingsGodRays.RayLength = Value;
		else if (!strcmp(Setting, "GlobalMultiplier"))
			SettingsGodRays.GlobalMultiplier = Value;
		else if (!strcmp(Setting, "Saturate"))
			SettingsGodRays.Saturate = Value;
		else if (!strcmp(Setting, "LightShaftPasses"))
			SettingsGodRays.LightShaftPasses = Value;
		else if (!strcmp(Setting, "Luminance"))
			SettingsGodRays.Luminance = Value;
		else if (!strcmp(Setting, "RayR"))
			SettingsGodRays.RayR = Value;
		else if (!strcmp(Setting, "RayG"))
			SettingsGodRays.RayG = Value;
		else if (!strcmp(Setting, "RayB"))
			SettingsGodRays.RayB = Value;
		else if (!strcmp(Setting, "SunGlareEnabled"))
			SettingsGodRays.SunGlareEnabled = Value;
		else if (!strcmp(Setting, "RayVisibility"))
			SettingsGodRays.RayVisibility = Value;
	}
	else if (!strcmp(Name, "Grass")) {
		if (!strcmp(Setting, "GrassDensity"))
			 SettingsGrass.GrassDensity = Value;
		else if (!strcmp(Setting, "MaxDistance"))
			 SettingsGrass.MaxDistance = Value;
		else if (!strcmp(Setting, "MinDistance"))
			 SettingsGrass.MinDistance = Value;
		else if (!strcmp(Setting, "MinHeight"))
			 SettingsGrass.MinHeight = Value;	
		else if (!strcmp(Setting, "ScaleX"))
			 SettingsGrass.ScaleX = Value;	
		else if (!strcmp(Setting, "ScaleY"))
			 SettingsGrass.ScaleY = Value;	
		else if (!strcmp(Setting, "ScaleZ"))
			 SettingsGrass.ScaleZ = Value;	
		else if (!strcmp(Setting, "WindCoefficient"))
			 SettingsGrass.WindCoefficient = Value;	
		else if (!strcmp(Setting, "WindEnabled"))
			 SettingsGrass.WindEnabled = Value;	
	}
	else if (!strcmp(Name, "HDR")) {
		if (!strcmp(Setting, "ToneMapping"))
			SettingsHDR.ToneMapping = Value;
		else if (!strcmp(Setting, "ToneMappingBlur"))
			SettingsHDR.ToneMappingBlur = Value;
		else if (!strcmp(Setting, "ToneMappingColor"))
			SettingsHDR.ToneMappingColor = Value;	
	}
	else if (!strcmp(Name, "LowHF")) {
		if (!strcmp(Setting, "BlurMultiplier"))
			SettingsLowHF.BlurMultiplier = Value;
		else if (!strcmp(Setting, "DarknessMultiplier"))
			SettingsLowHF.DarknessMultiplier = Value;
		else if (!strcmp(Setting, "FatigueLimit"))
			SettingsLowHF.FatigueLimit = Value;
		else if (!strcmp(Setting, "HealthLimit"))
			SettingsLowHF.HealthLimit = Value;
		else if (!strcmp(Setting, "LumaMultiplier"))
			SettingsLowHF.LumaMultiplier = Value;
		else if (!strcmp(Setting, "VignetteMultiplier"))
			SettingsLowHF.VignetteMultiplier = Value;
	}
	else if (!strcmp(Name, "MotionBlur")) {
		SettingsMotionBlurStruct *sms = GetSettingsMotionBlur(Section);
		if (!strcmp(Setting, "BlurCutOff"))
			sms->BlurCutOff = Value;
		else if (!strcmp(Setting, "BlurOffsetMax"))
			sms->BlurOffsetMax = Value;
		else if (!strcmp(Setting, "BlurScale"))
			sms->BlurScale = Value;
		else if (!strcmp(Setting, "Enabled"))
			sms->Enabled = Value;
		else if (!strcmp(Setting, "GaussianWeight"))
			sms->GaussianWeight = Value;
	}
	else if (!strcmp(Name, "POM")) {
		if (!strcmp(Setting, "HeightMapScale"))
			SettingsPOM.HeightMapScale = Value;
		else if (!strcmp(Setting, "MaxSamples"))
			SettingsPOM.MaxSamples = Value;
		else if (!strcmp(Setting, "MinSamples"))
			SettingsPOM.MinSamples = Value;
		else if (!strcmp(Setting, "ShadowSoftening"))
			SettingsPOM.ShadowSoftening = Value;	
	}
	else if (!strcmp(Name, "Precipitations")) {
		if (!strcmp(Setting, "Intensity"))
			SettingsPrecipitations.Intensity = Value;
		else if (!strcmp(Setting, "Size"))
			SettingsPrecipitations.Size = Value;
		else if (!strcmp(Setting, "Velocity"))
			SettingsPrecipitations.Velocity = Value;
		else if (!strcmp(Setting, "SnowIncrease"))
			SettingsPrecipitations.SnowIncrease = Value;
		else if (!strcmp(Setting, "SnowDecrease"))
			SettingsPrecipitations.SnowDecrease = Value;
		else if (!strcmp(Setting, "SnowSunPower"))
			SettingsPrecipitations.SnowSunPower = Value;
		else if (!strcmp(Setting, "SnowAmount"))
			SettingsPrecipitations.SnowAmount = Value;
		else if (!strcmp(Setting, "SnowBlurNormDropThreshhold"))
			SettingsPrecipitations.SnowBlurNormDropThreshhold = Value;
		else if (!strcmp(Setting, "SnowBlurRadiusMultiplier"))
			SettingsPrecipitations.SnowBlurRadiusMultiplier = Value;
		else if (!strcmp(Setting, "RainIncrease"))
			SettingsPrecipitations.RainIncrease = Value;
		else if (!strcmp(Setting, "RainDecrease"))
			SettingsPrecipitations.RainDecrease = Value;
		else if (!strcmp(Setting, "RainAmount"))
			SettingsPrecipitations.RainAmount = Value;
		else if (!strcmp(Setting, "PuddleCoeff_B"))
			SettingsPrecipitations.PuddleCoeff_B = Value;
		else if (!strcmp(Setting, "PuddleCoeff_G"))
			SettingsPrecipitations.PuddleCoeff_G = Value;
		else if (!strcmp(Setting, "PuddleCoeff_R"))
			SettingsPrecipitations.PuddleCoeff_R = Value;
		else if (!strcmp(Setting, "PuddleSpecularMultiplier"))
			SettingsPrecipitations.PuddleSpecularMultiplier = Value;
	}
	else if (!strcmp(Name, "Shadows")) {
		if (!strcmp(Setting, "SelfShadowIntensity"))
			SettingsShadows.SelfShadowIntensity = Value;
		else if (!strcmp(Setting, "ShadowIntensity"))
			SettingsShadows.ShadowIntensity = Value;
	}
	else if (!strcmp(Name, "Sharpening")) {
		if (!strcmp(Setting, "Clamp"))
			SettingsSharpening.Clamp = Value;
		else if (!strcmp(Setting, "Offset"))
			SettingsSharpening.Offset = Value;
		else if (!strcmp(Setting, "Strength"))
			SettingsSharpening.Strength = Value;
	}
	else if (!strcmp(Name, "Skin")) {
		if (!strcmp(Setting, "Attenuation"))
			SettingsSkin.Attenuation = Value;
		else if (!strcmp(Setting, "ExtCoeffBlue"))
			SettingsSkin.ExtCoeffBlue = Value;
		else if (!strcmp(Setting, "ExtCoeffGreen"))
			SettingsSkin.ExtCoeffGreen = Value;
		else if (!strcmp(Setting, "ExtCoeffRed"))
			SettingsSkin.ExtCoeffRed = Value;
		else if (!strcmp(Setting, "MaterialThickness"))
			SettingsSkin.MaterialThickness = Value;
		else if (!strcmp(Setting, "RimScalar"))
			SettingsSkin.RimScalar = Value;
		else if (!strcmp(Setting, "SpecularPower"))
			SettingsSkin.SpecularPower = Value;
	}
	else if (!strcmp(Name, "Terrain")) {
		if (!strcmp(Setting, "DistantNoise"))
			SettingsTerrain.DistantNoise = Value;
		else if (!strcmp(Setting, "DistantSpecular"))
			SettingsTerrain.DistantSpecular = Value;
		else if (!strcmp(Setting, "MiddleSpecular"))
			SettingsTerrain.MiddleSpecular = Value;
		else if (!strcmp(Setting, "NearSpecular"))
			SettingsTerrain.NearSpecular = Value;
	}
	else if (!strcmp(Name, "Water")) {
		SettingsWaterStruct *sws = GetSettingsWater(Section);
		if (!strcmp(Setting, "causticsStrength"))
			sws->causticsStrength = Value;
		else if (!strcmp(Setting, "causticsStrengthS"))
			sws->causticsStrengthS = Value;
		else if (!strcmp(Setting, "choppiness"))
			sws->choppiness = Value;
		else if (!strcmp(Setting, "depthDarkness"))
			sws->depthDarkness = Value;
		else if (!strcmp(Setting, "Icechoppiness"))
			sws->Icechoppiness = Value;
		else if (!strcmp(Setting, "IceEnabled"))
			sws->IceEnabled = Value;
		else if (!strcmp(Setting, "IceinExtCoeff_B"))
			sws->IceinExtCoeff_B = Value;
		else if (!strcmp(Setting, "IceinExtCoeff_G"))
			sws->IceinExtCoeff_G = Value;
		else if (!strcmp(Setting, "IceinExtCoeff_R"))
			sws->IceinExtCoeff_R = Value;
		else if (!strcmp(Setting, "Icereflectivity"))
			sws->Icereflectivity = Value;
		else if (!strcmp(Setting, "IceshoreFactor"))
			sws->IceshoreFactor = Value;
		else if (!strcmp(Setting, "Iceturbidity"))
			sws->Iceturbidity = Value;
		else if (!strcmp(Setting, "IcewaveWidth"))
			sws->IcewaveWidth = Value;
		else if (!strcmp(Setting, "inExtCoeff_B"))
			sws->inExtCoeff_B = Value;
		else if (!strcmp(Setting, "inExtCoeff_G"))
			sws->inExtCoeff_G = Value;
		else if (!strcmp(Setting, "inExtCoeff_R"))
			sws->inExtCoeff_R = Value;
		else if (!strcmp(Setting, "inScattCoeff"))
			sws->inScattCoeff = Value;
		else if (!strcmp(Setting, "LensAmount"))
			sws->LensAmount = Value;
		else if (!strcmp(Setting, "LensTime"))
			sws->LensTime = Value;
		else if (!strcmp(Setting, "LensTimeMultA"))
			sws->LensTimeMultA = Value;
		else if (!strcmp(Setting, "LensTimeMultB"))
			sws->LensTimeMultB = Value;
		else if (!strcmp(Setting, "LensViscosity"))
			sws->LensViscosity = Value;
		else if (!strcmp(Setting, "LODdistance"))
			sws->LODdistance = Value;
		else if (!strcmp(Setting, "MinLOD"))
			sws->MinLOD = Value;
		else if (!strcmp(Setting, "reflectivity"))
			sws->reflectivity = Value;
		else if (!strcmp(Setting, "shoreFactor"))
			sws->shoreFactor = Value;
		else if (!strcmp(Setting, "turbidity"))
			sws->turbidity = Value;
		else if (!strcmp(Setting, "waveSpeed"))
			sws->waveSpeed = Value;
		else if (!strcmp(Setting, "waveWidth"))
			sws->waveWidth = Value;	
	}
}

bool SettingManager::GetEnabled(const char* Name)
{
	bool Value;

	if (!strcmp(Name, "AmbientOcclusion")) {
		Value = SettingsMain.EnableAmbientOcclusion;
	}
	else if (!strcmp(Name, "Blood")) {
		Value = SettingsMain.EnableBlood;
	}
	else if (!strcmp(Name, "BloodLens")) {
		Value = SettingsMain.EnableBloodLens;
	}
	else if (!strcmp(Name, "Bloom")) {
		Value = SettingsMain.EnableBloom;
	}
	else if (!strcmp(Name, "Cinema")) {
		Value = SettingsMain.EnableCinema;
	}
	else if (!strcmp(Name, "Coloring")) {
		Value = SettingsMain.EnableColoring;
	}
	else if (!strcmp(Name, "DepthOfField")) {
		Value = SettingsMain.EnableDepthOfField;
	}
	else if (!strcmp(Name, "GodRays")) {
		Value = SettingsMain.EnableGodRays;
	}
	else if (!strcmp(Name, "Grass")) {
		Value = SettingsMain.EnableGrass;
	}
	else if (!strcmp(Name, "HDR")) {
		Value = SettingsMain.EnableHDR;
	}
	else if (!strcmp(Name, "LowHF")) {
		Value = SettingsMain.EnableLowHF;
	}
	else if (!strcmp(Name, "MotionBlur")) {
		Value = SettingsMain.EnableMotionBlur;
	}
	else if (!strcmp(Name, "POM")) {
		Value = SettingsMain.EnablePOM;
	}
	else if (!strcmp(Name, "Precipitations")) {
		Value = SettingsMain.EnablePrecipitations;
	}
	else if (!strcmp(Name, "Shadows")) {
		Value = SettingsMain.EnableShadows;
	}
	else if (!strcmp(Name, "Sharpening")) {
		Value = SettingsMain.EnableSharpening;
	}
	else if (!strcmp(Name, "Skin")) {
		Value = SettingsMain.EnableSkin;
	}
	else if (!strcmp(Name, "SMAA")) {
		Value = SettingsMain.EnableSMAA;
	}
	else if (!strcmp(Name, "SnowAccumulation")) {
		Value = SettingsMain.EnableSnowAccumulation;
	}
	else if (!strcmp(Name, "Terrain")) {
		Value = SettingsMain.EnableTerrain;
	}
	else if (!strcmp(Name, "Underwater")) {
		Value = SettingsMain.EnableUnderwater;
	}
	else if (!strcmp(Name, "Water")) {
		Value = SettingsMain.EnableWater;
	}
	else if (!strcmp(Name, "WaterLens")) {
		Value = SettingsMain.EnableWaterLens;
	}
	else if (!strcmp(Name, "WetWorld")) {
		Value = SettingsMain.EnableWetWorld;
	}

	return Value;

}

SettingsWaterStruct* SettingManager::GetSettingsWater(const char* PlayerLocation) {

	 SettingsWaterList::iterator v = SettingsWater.find(std::string(PlayerLocation));
	 if (v == SettingsWater.end())
		 return NULL;
	 else
		 return &v->second;

}

SettingsDepthOfFieldStruct* SettingManager::GetSettingsDepthOfField(const char* Section) {

	SettingsDepthOfFieldList::iterator v = SettingsDepthOfField.find(std::string(Section));
	if (v == SettingsDepthOfField.end())
		return NULL;
	else
		return &v->second;

}

SettingsAmbientOcclusionStruct* SettingManager::GetSettingsAmbientOcclusion(const char* Section) {

	SettingsAmbientOcclusionList::iterator v = SettingsAmbientOcclusion.find(std::string(Section));
	if (v == SettingsAmbientOcclusion.end())
		return NULL;
	else
		return &v->second;

}

SettingsColoringStruct* SettingManager::GetSettingsColoring(const char* PlayerLocation) {

	 SettingsColoringList::iterator v = SettingsColoring.find(std::string(PlayerLocation));
	 if (v == SettingsColoring.end())
		 return NULL;
	 else
		 return &v->second;

}

SettingsBloomStruct* SettingManager::GetSettingsBloom(const char* PlayerLocation) {

	 SettingsBloomList::iterator v = SettingsBloom.find(std::string(PlayerLocation));
	 if (v == SettingsBloom.end())
		 return NULL;
	 else
		 return &v->second;

}

SettingsMotionBlurStruct* SettingManager::GetSettingsMotionBlur(const char* Section) {

	SettingsMotionBlurList::iterator v = SettingsMotionBlur.find(std::string(Section));
	if (v == SettingsMotionBlur.end())
		return NULL;
	else
		return &v->second;

}

void SettingManager::InitCamera()
{

	SettingsCamera.Move = false;
	SettingsCamera.Rotate = false;
	SettingsCamera.Look = false;
	SettingsCamera.MoveTo = false;
	SettingsCamera.RotateTo = false;
	SettingsCamera.LookTo = false;
	SettingsCamera.Ref = NULL;

}

#if defined(OBLIVION)
class Settings {

public:
	bool TrackSaveSettings();
	bool TrackLoadGame(BSFile* GameFile, char* FileName, UInt8 Arg3);
	bool TrackSaveGame(BSFile* GameFile, char* FileName, UInt8 Arg3);
	void TrackDeleteGame(BSFile* GameFile, char* FileName);
	BSFile* TrackCreateGameFile(BSFile* GameFile, char* FileName, UInt8 Arg3);

};

bool (__thiscall Settings::* SaveSettings)();
bool (__thiscall Settings::* TrackSaveSettings)();
bool Settings::TrackSaveSettings() { // ecx is INISettingCollection*
	
	bool r = false;

	*SettingNearDistance = 10.0;
	if (TheSettingManager->SettingsMain.SaveSettings) r = (this->*SaveSettings)();
	return r;

}

bool (__thiscall Settings::* LoadGame)(BSFile*, char*, UInt8);
bool (__thiscall Settings::* TrackLoadGame)(BSFile*, char*, UInt8);
bool Settings::TrackLoadGame(BSFile* GameFile, char* FileName, UInt8 Arg3) { // ecx is TESSaveLoadGame*

	bool r;
	char ORRecordType[1];
	char ORRecord[12];
	
	TheSettingManager->GameLoading = true;
	r = (this->*LoadGame)(GameFile, FileName, Arg3);
	TheSettingManager->GameLoading = false;
	TheEquipmentManager->LeftWeapons.clear();
	std::ifstream ORSaveGame(TheSettingManager->GameFilePath,std::ios::in|std::ios::binary);
	if (ORSaveGame.is_open()) {
		ORSaveGame.read(ORRecord, 10);
		if (!memcmp(ORRecord, "ORSAVEGAME", 10)) {
			while (!ORSaveGame.eof()) {
				ORSaveGame.read(ORRecordType, 1);
				switch (ORRecordType[0])
				{
					case '0':
						{
							ORSaveGame.read(ORRecord, 8);
							char* pORRecord = ORRecord;
							UInt32 LeftWeaponID = *(UInt32*)pORRecord;
							UInt32 Count = *(UInt32*)(pORRecord += 4);
							TESForm* Form = LookupFormByID(LeftWeaponID);
							if (Form) {
								TESObjectARMO* LeftWeapon = (TESObjectARMO*)Oblivion_DynamicCast(Form, 0, RTTI_TESForm, RTTI_TESObjectARMO, 0);
								if (LeftWeapon) {
									char* ModelPath = LeftWeapon->bipedModel.bipedModel[0].nifPath.m_data;
									LeftWeaponData LeftWeaponD;
									LeftWeaponD.LeftWeapon = LeftWeapon;
									LeftWeaponD.Count = 1;
									TheEquipmentManager->LeftWeapons[std::string(ModelPath)] = LeftWeaponD;
								}
							}
						}
						break;
					case '1':
						ORSaveGame.read(ORRecord, 12);
						break;
				}
			}
		}
		ORSaveGame.close();
	}
	TheShaderManager->InitializeConstants();
	return r;

}

bool (__thiscall Settings::* SaveGame)(BSFile*, char*, UInt8);
bool (__thiscall Settings::* TrackSaveGame)(BSFile*, char*, UInt8);
bool Settings::TrackSaveGame(BSFile* GameFile, char* FileName, UInt8 Arg3) {
	
	TESSaveLoadGame* SaveGameList = (TESSaveLoadGame*)this;
	Actor* Act = (*g_thePlayer);
	bool r;

	LeftWeaponList::iterator vw = TheEquipmentManager->LeftWeapons.begin();
	while (vw != TheEquipmentManager->LeftWeapons.end()) {
		LeftWeaponData* LeftWeapon = &vw->second;
		if (LeftWeapon->Count > 0) // Adds only the used left weapons to the Oblivion's savegame to avoid to bloat the savegame
			AddFormToCreatedBaseObjectsList(SaveGameList, LeftWeapon->LeftWeapon);
		vw++;
	}	
	r = (this->*SaveGame)(GameFile, FileName, Arg3);
	std::ofstream ORSaveGame(TheSettingManager->GameFilePath,std::ios::out|std::ios::binary);
	ORSaveGame.write("ORSAVEGAME", 10);
	vw = TheEquipmentManager->LeftWeapons.begin();
	while (vw != TheEquipmentManager->LeftWeapons.end()) {
		LeftWeaponData* LeftWeapon = &vw->second;
		if (LeftWeapon->Count > 0) {
			ORSaveGame.write("0", 1); // Record type
			ORSaveGame.write((char*)&LeftWeapon->LeftWeapon->refID, 4);
			ORSaveGame.write((char*)&LeftWeapon->Count, 4);
		}
		vw++;
	}
	NiORExtraData* ORData = TheEquipmentManager->GetORData(Act->niNode);
	if (ORData->LeftWeapon) {
		ORSaveGame.write("1", 1); // Record type
		ORSaveGame.write((char*)&Act->refID, 4);
		ORSaveGame.write((char*)&ORData->LeftWeapon->refID, 4);
		ORSaveGame.write((char*)&ORData->OriginalWeapon->refID, 4);
	}
	tList<Actor>* HighActorlist = &g_actorProcessManager->highActors;
	for (tList<Actor>::Iterator iter = HighActorlist->Begin(); !iter.End(); ++iter) {
		Act = iter.Get();
		if (Act && Act->niNode) {
			NiORExtraData* ORData = TheEquipmentManager->GetORData(Act->niNode);
			if (ORData->LeftWeapon) {
				ORSaveGame.write("1", 1); // Record type
				ORSaveGame.write((char*)&Act->refID, 4);
				ORSaveGame.write((char*)&ORData->LeftWeapon->refID, 4);
				ORSaveGame.write((char*)&ORData->OriginalWeapon->refID, 4);
			}
		}
	}
	ORSaveGame.close();
	return r;

}

void (__thiscall Settings::* DeleteGame)(BSFile*, char*);
void (__thiscall Settings::* TrackDeleteGame)(BSFile*, char*);
void Settings::TrackDeleteGame(BSFile* GameFile, char* FileName) { // ecx is TESSaveLoadGame*

	(this->*DeleteGame)(GameFile, FileName);
	DeleteFileA(TheSettingManager->GameFilePath);

}

BSFile* (__thiscall Settings::* CreateGameFile)(BSFile*, char*, UInt8);
BSFile* (__thiscall Settings::* TrackCreateGameFile)(BSFile*, char*, UInt8);
BSFile* Settings::TrackCreateGameFile(BSFile* GameFile, char* FileName, UInt8 Arg3) { // ecx is TESSaveLoadGame*

	BSFile* NewGameFile = (this->*CreateGameFile)(GameFile, FileName, Arg3);
	strcpy(TheSettingManager->GameFilePath, NewGameFile->m_path);
	char* ORFileName = TheSettingManager->GameFilePath;
	int i = (int)strstr(ORFileName, ".ess") - int(ORFileName);
	ORFileName[i] = '\0';
	strcat(ORFileName, ".or");
	return NewGameFile;

}

void CreateSettingsHook() {

	*((int *)&SaveSettings)		= 0x004A8560;
	TrackSaveSettings			= &Settings::TrackSaveSettings;
	*((int *)&LoadGame)			= 0x00465860;
	TrackLoadGame				= &Settings::TrackLoadGame;
	*((int *)&SaveGame)			= 0x00465130;
	TrackSaveGame				= &Settings::TrackSaveGame;
	*((int *)&DeleteGame)		= 0x00453480;
	TrackDeleteGame				= &Settings::TrackDeleteGame;
	*((int *)&CreateGameFile)	= 0x0045F2E0;
	TrackCreateGameFile			= &Settings::TrackCreateGameFile;

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)SaveSettings,		*((PVOID *)&TrackSaveSettings));
	DetourAttach(&(PVOID&)LoadGame,			*((PVOID *)&TrackLoadGame));
	DetourAttach(&(PVOID&)SaveGame,			*((PVOID *)&TrackSaveGame));
	DetourAttach(&(PVOID&)DeleteGame,		*((PVOID *)&TrackDeleteGame));
	DetourAttach(&(PVOID&)CreateGameFile,	*((PVOID *)&TrackCreateGameFile));
	DetourTransactionCommit();

}
#endif