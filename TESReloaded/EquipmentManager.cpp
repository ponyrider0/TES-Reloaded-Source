#if defined(OBLIVION)
#include "Hooking\detours\detours.h"
#include "obse_common\SafeWrite.h"
#include <string>
#include <fstream>

static const UInt32 kPrnHook = 0x0047927B;
static const UInt32 kPrnReturn = 0x0047928A;
static const char* ForearmTwistNodeName = "Bip01 L ForearmTwist";
static const char* QuiverNodeName = "Quiver";
static const char* TorchNodeName = "Torch";

EquipmentManager::EquipmentManager() {

	_MESSAGE("Starting the equipment manager...");
	ShieldAnim = NULL;
	LeftWeaponTemplate = NULL;
	vEquippingState = EquippingState::Equipping_None;

}

NiExtraData* EquipmentManager::GetExtraData(NiObjectNET* Object, const char* Name) {

	return (NiExtraData*)ThisStdCall(0x006FF9C0, Object, Name);

}

// NiORExtraData has some values stored in the Actor->niNode
// These values should be set into the HighProcess, but i should replace the HighProcess with a my implemented class, so i decided for this way.
NiORExtraData* EquipmentManager::CreateORData(NiNode* DestinationNode) {

	NiORExtraData* ORData = (NiORExtraData*)FormHeap_Allocate(0x18);
	ORData = (NiORExtraData*)ThisStdCall(0x00721350, ORData); // Runs the internal NiExtraData constructor
	ORData->m_pcName = "ORD";
	ResetORData(ORData);
	ThisStdCall(0x006FF570, DestinationNode, ORData); //Attaches the extradata to the node
	return ORData;

}

NiORExtraData* EquipmentManager::GetORData(NiNode* DestinationNode) {

	NiORExtraData* ORData = (NiORExtraData*)GetExtraData(DestinationNode, "ORD");

	if (!ORData) ORData = CreateORData(DestinationNode);
	return ORData;

}

void EquipmentManager::ResetORData(NiORExtraData* ORData) {

	ORData->vActionState = ActionState::Action_None;
	ORData->vShieldState = ShieldState::Shield_Out;
	ORData->vPad0E = 0;
	ORData->vPad0F = 0;
	ORData->OriginalWeapon = NULL;
	ORData->LeftWeapon = NULL;

}

NiORExtraData* EquipmentManager::LoadORData(Actor* Act)
{
	char ORRecordType[1];
	char ORRecord[12];

	NiORExtraData* ORData = TheEquipmentManager->GetORData(Act->niNode);
	std::ifstream ORSaveGame(TheSettingManager->GameFilePath, std::ios::in | std::ios::binary);
	if (ORSaveGame.is_open()) {
		ORSaveGame.read(ORRecord, 10);
		if (!memcmp(ORRecord, "ORSAVEGAME", 10)) {
			while (!ORSaveGame.eof()) {
				ORSaveGame.read(ORRecordType, 1);
				switch (ORRecordType[0])
				{
				case '0':
					ORSaveGame.read(ORRecord, 8);
					break;
				case '1':
				{
					ORSaveGame.read(ORRecord, 12);
					char* pORRecord = ORRecord;
					UInt32 ActorID = *(UInt32*)pORRecord;
					UInt32 LeftWeaponID = *(UInt32*)(pORRecord += 4);
					UInt32 OriginalWeaponID = *(UInt32*)(pORRecord += 4);
					if (Act->refID == ActorID) {
						TESForm* Form = LookupFormByID(LeftWeaponID);
						if (Form) {
							TESObjectARMO* LeftWeapon = (TESObjectARMO*)Oblivion_DynamicCast(Form, 0, RTTI_TESForm, RTTI_TESObjectARMO, 0);
							if (LeftWeapon) {
								ORData->LeftWeapon = LeftWeapon;
								Form = LookupFormByID(OriginalWeaponID);
								if (Form) ORData->OriginalWeapon = (TESObjectWEAP*)Oblivion_DynamicCast(Form, 0, RTTI_TESForm, RTTI_TESObjectWEAP, 0);
							}
						}
					}
				}
				break;
				}
			}
		}
		ORSaveGame.close();
	}
	return ORData;

}

TESObjectARMO* EquipmentManager::GetLeftWeapon(TESObjectWEAP* Weapon) {
	
	const char* ModelPath = Weapon->model.nifPath.m_data;
	const char* IconPath = Weapon->icon.ddsPath.m_data;
	std::string StringPath = std::string(ModelPath);
	LeftWeaponData LeftWeapon;
	
	LeftWeaponList::iterator v = LeftWeapons.find(StringPath);
	if (v == LeftWeapons.end()) {
		LeftWeapon.LeftWeapon = (TESObjectARMO*)CreateFormInstance(LeftWeaponTemplate->typeID);
		LeftWeapon.LeftWeapon->CopyFrom(LeftWeaponTemplate);
		AddFormToDataHandler(*g_dataHandler, LeftWeapon.LeftWeapon);
		LeftWeapon.LeftWeapon->fullName.name.Set(Weapon->fullName.name.m_data);
		LeftWeapon.LeftWeapon->bipedModel.bipedModel[0].SetPath(ModelPath);
		LeftWeapon.LeftWeapon->bipedModel.bipedModel[1].SetPath(ModelPath);
		LeftWeapon.LeftWeapon->bipedModel.groundModel[0].SetPath(ModelPath);
		LeftWeapon.LeftWeapon->bipedModel.groundModel[1].SetPath(ModelPath);
		LeftWeapon.LeftWeapon->bipedModel.icon[0].SetPath(IconPath);
		LeftWeapon.LeftWeapon->bipedModel.icon[1].SetPath(IconPath);
		LeftWeapon.LeftWeapon->weight.weight = Weapon->weight.weight;
		LeftWeapon.LeftWeapon->health.health = Weapon->health.health;
		LeftWeapon.LeftWeapon->value.value = Weapon->value.value;
		LeftWeapon.LeftWeapon->armorRating = Weapon->attackDmg.damage * 0.5;
		LeftWeapon.Count = 0;
		LeftWeapons[StringPath] = LeftWeapon;
	}
	else
		LeftWeapon = v->second;
	return LeftWeapon.LeftWeapon;

}

void EquipmentManager::AddLeftWeaponRef(TESObjectARMO* LeftWeapon) {
	
	const char* ModelPath = LeftWeapon->bipedModel.bipedModel[0].nifPath.m_data;

	LeftWeaponList::iterator v = LeftWeapons.find(std::string(ModelPath));
	if (v != LeftWeapons.end())
		v->second.Count = v->second.Count + 1;
	
}

void EquipmentManager::RemoveLeftWeaponRef(TESObjectARMO* LeftWeapon) {

	const char* ModelPath = LeftWeapon->bipedModel.bipedModel[0].nifPath.m_data;

	LeftWeaponList::iterator v = LeftWeapons.find(std::string(ModelPath));
	if (v != LeftWeapons.end())
		v->second.Count = v->second.Count - 1;

}

void SetRP(NiAVObject* ShieldObject, NiPoint3 Rotation, NiPoint3 Position) {

	NiMatrix33* m = &ShieldObject->m_localTransform.rot;
	NiPoint3* v = &ShieldObject->m_localTransform.pos;
	TheUtilityManager->GenerateRotationMatrixZXY(m, Rotation.x, Rotation.y, Rotation.z, 1);
	TheUtilityManager->MatrixVectorMultiply(v, m, &Position);

}

void ToggleShieldPosition(NiNode* ActorNode, NiORExtraData* ORData, TESObjectARMO* Shield) {
	
	NiNode* ForearmTwistNode = (NiNode*)ActorNode->GetObjectByName(ForearmTwistNodeName);
	NiNode* QuiverNode = (NiNode*)ActorNode->GetObjectByName(QuiverNodeName);
	NiNode* TorchNode = (NiNode*)ActorNode->GetObjectByName(TorchNodeName);
	if (ORData->vActionState == EquipmentManager::ActionState::Action_In) {
		if (Shield == ORData->LeftWeapon) {
			NiAVObject* ShieldObject = *TorchNode->m_children.data;
			TorchNode->RemoveObject(&ShieldObject, ShieldObject);
			NiPoint3 Rotation = {15.0f, 5.0f, 0.0f};
			NiPoint3 Position = {6.0f, -30.0f, 2.0f};
			SetRP(ShieldObject, Rotation, Position);
			QuiverNode->AddObject(ShieldObject, 1);
		}
		else {
			NiAVObject* ShieldObject = *ForearmTwistNode->m_children.data;
			ForearmTwistNode->RemoveObject(&ShieldObject, ShieldObject);
			NiPoint3 Rotation = {0.0f, 270.0f, 90.0f};
			NiPoint3 Position = {0.0f, 2.0f, 0.0f};
			SetRP(ShieldObject, Rotation, Position);
			QuiverNode->AddObject(ShieldObject, 1);
		}
		ORData->vShieldState = EquipmentManager::ShieldState::Shield_In;
	}
	else {
		NiAVObject* ShieldObject = NULL;
		NiTArray<NiAVObject*> QuiverChildren = QuiverNode->m_children;
		for (int i = 0; i < QuiverChildren.numObjs; i++) {
			if (!memcmp(QuiverChildren.data[i]->m_pcName, "Shield", 6)) {
				ShieldObject = QuiverChildren.data[i];
				break;
			}
		}
		QuiverNode->RemoveObject(&ShieldObject, ShieldObject);
		NiPoint3 Rotation = {0.0f, 0.0f, 0.0f};
		NiPoint3 Position = {0.0f, 0.0f, 0.0f};
		SetRP(ShieldObject, Rotation, Position);
		if (Shield == ORData->LeftWeapon)
			TorchNode->AddObject(ShieldObject, 1);
		else
			ForearmTwistNode->AddObject(ShieldObject, 1);
		ORData->vShieldState = EquipmentManager::ShieldState::Shield_Out;
	}
	ORData->vActionState	 = EquipmentManager::ActionState::Action_None;

}

void SetORAction(HighProcess* Proc, NiORExtraData* ORData) {
	
	if (Proc->WeaponState && ORData->vShieldState == EquipmentManager::ShieldState::Shield_In)
		ORData->vActionState = EquipmentManager::ActionState::Action_Out;
	else if (!Proc->WeaponState && ORData->vShieldState == EquipmentManager::ShieldState::Shield_Out)
		ORData->vActionState = EquipmentManager::ActionState::Action_In;

}

class Equipment {

public:
	int TrackProcessAction(float Arg1, float Arg2);
	void TrackUnequipItem(TESForm* Form, int Quantity, BaseExtraList* ExtraList, int Arg4, int Arg5, int Arg6);
	void TrackEquipShield(TESObjectARMO* Shield);

};

int (__thiscall Equipment::* ProcessAction)(float, float);
int (__thiscall Equipment::* TrackProcessAction)(float, float);
int Equipment::TrackProcessAction(float Arg1, float Arg2) {
	
	Actor* Act = (Actor*)this;
	
	if (Act->process->GetProcessLevel() == 0) {
		HighProcess* Proc = (HighProcess*)Act->process;
		ExtraContainerChanges::EntryData* WeaponData = Proc->equippedWeaponData;
		ExtraContainerChanges::EntryData* ShieldData = Proc->equippedShieldData;
		ExtraContainerChanges::EntryData* LightData = Proc->equippedLightData;
		if (ShieldData && WeaponData && !LightData) {
			TESObjectWEAP* Weapon = (TESObjectWEAP*)WeaponData->type;
			TESObjectARMO* Shield = (TESObjectARMO*)ShieldData->type;
			if (Weapon->type == TESObjectWEAP::kType_BladeOneHand || Weapon->type == TESObjectWEAP::kType_BluntOneHand) {
				NiNode* ActorNode = Act->niNode;
				NiORExtraData* ORData = TheEquipmentManager->GetORData(ActorNode);
				switch (Proc->currentAction)
				{
					case HighProcess::kAction_None:
						if (ORData->vActionState == EquipmentManager::ActionState::Action_None)
							SetORAction(Proc, ORData);
						if (ORData->vActionState == EquipmentManager::ActionState::Action_In || ORData->vActionState == EquipmentManager::ActionState::Action_Out)
							ThisStdCall(0x00477DB0, Proc->animData, TheEquipmentManager->ShieldAnim, Act, TheEquipmentManager->ShieldAnim->animFlags, 3);
						break;
					case HighProcess::kAction_SpecialIdle:
						if (ORData->vActionState == EquipmentManager::ActionState::Action_In || ORData->vActionState == EquipmentManager::ActionState::Action_Out) {
							for (int i = 0; i < 5; i++) {
								BSAnimGroupSequence* AnimSequence = Proc->animData->animSequences[i];
								if (AnimSequence && !memcmp(AnimSequence->filePath, ShieldAnimFullFile, 46) && AnimSequence->last >= 0.4)
									ToggleShieldPosition(ActorNode, ORData, Shield);
							}
						}
						break;
					default:
						break;
				}
			}
		}
	}
	return (this->*ProcessAction)(Arg1, Arg2);

}

void (__thiscall Equipment::* UnequipItem)(TESForm*, int, BaseExtraList*, int, int, int);
void (__thiscall Equipment::* TrackUnequipItem)(TESForm*, int, BaseExtraList*, int, int, int);
void Equipment::TrackUnequipItem(TESForm* Form, int Quantity, BaseExtraList* ExtraList, int Arg4, int Arg5, int Arg6) {

	Actor* Act = (Actor*)this;
	NiNode* ActorNode = Act->niNode;

	if (ActorNode && Act->process->GetProcessLevel() == 0) {
		if (Form->typeID == FormType::kFormType_Weapon) {
			TESObjectWEAP* Weapon = (TESObjectWEAP*)Form;
			if (Weapon->type == TESObjectWEAP::kType_BladeOneHand || Weapon->type == TESObjectWEAP::kType_BluntOneHand) {
				HighProcess* Proc = (HighProcess*)Act->process;
				NiORExtraData* ORData = TheEquipmentManager->GetORData(ActorNode);
				ExtraContainerChanges::EntryData* ShieldData = Proc->equippedShieldData;
				if (ShieldData && ShieldData->type == ORData->LeftWeapon)
					TheCommandManager->UnequipItem(Act, ORData->LeftWeapon);
			}
		}
	}
	(this->*UnequipItem)(Form, Quantity, ExtraList, Arg4, Arg5, Arg6);
	if (ActorNode && Act->process->GetProcessLevel() == 0) {
		NiORExtraData* ORData = TheEquipmentManager->GetORData(ActorNode);
		if (Form == ORData->LeftWeapon && ORData->vActionState == EquipmentManager::ActionState::Action_None) {
			TheCommandManager->RemoveItem(Act, Form);
			TheCommandManager->AddItem(Act, ORData->OriginalWeapon);
			TheEquipmentManager->RemoveLeftWeaponRef(ORData->LeftWeapon);
			TheEquipmentManager->ResetORData(ORData);
		}
	}

}

void (__thiscall Equipment::* EquipShield)(TESObjectARMO*);
void (__thiscall Equipment::* TrackEquipShield)(TESObjectARMO*);
void Equipment::TrackEquipShield(TESObjectARMO* Shield) {

	Actor* Act = (Actor*)this;
	NiNode* ActorNode = Act->niNode;

	if (ActorNode && Act->process->GetProcessLevel() == 0) {
		HighProcess* Proc = (HighProcess*)Act->process;
		NiORExtraData* ORData = NULL;
		if (TheSettingManager->GameLoading)
			ORData = TheEquipmentManager->LoadORData(Act);
		else
			ORData = TheEquipmentManager->GetORData(ActorNode);
		if (Shield == ORData->LeftWeapon) {
			if (Proc->WeaponState) {
				ORData->vShieldState = EquipmentManager::ShieldState::Shield_Out;
				TheEquipmentManager->vEquippingState = EquipmentManager::EquippingState::Equipping_Weapon_Out;
			}
			else {
				ORData->vShieldState = EquipmentManager::ShieldState::Shield_In;
				TheEquipmentManager->vEquippingState = EquipmentManager::EquippingState::Equipping_Weapon_In;
			}
		}
		else {
			if (Proc->WeaponState) {
				ORData->vShieldState = EquipmentManager::ShieldState::Shield_Out;
				TheEquipmentManager->vEquippingState = EquipmentManager::EquippingState::Equipping_Shield_Out;
			}
			else {
				ORData->vShieldState = EquipmentManager::ShieldState::Shield_In;
				TheEquipmentManager->vEquippingState = EquipmentManager::EquippingState::Equipping_Shield_In;
			}
		}
	}
	(this->*EquipShield)(Shield);

}

void Prn(NiNode* ActorNode, NiNode* AttachNode, NiNode* ObjectNode, SkinInfo* ActorSkinInfo, int SkinInfoSlot) {
	
	if (SkinInfoSlot == 13) { // Equipping the shield from the SkinInfo (the EquipShield is not called on first game loading)
		Actor* Act = ActorSkinInfo->Actor150;

		if (Act->process->GetProcessLevel() == 0) {
			NiORExtraData* ORData = TheEquipmentManager->LoadORData(Act);
			if (ActorSkinInfo->ShieldForm == ORData->LeftWeapon) {
				ORData->vShieldState = EquipmentManager::ShieldState::Shield_In;
				TheEquipmentManager->vEquippingState = EquipmentManager::EquippingState::Equipping_Weapon_In;
			}
			else {
				ORData->vShieldState = EquipmentManager::ShieldState::Shield_In;
				TheEquipmentManager->vEquippingState = EquipmentManager::EquippingState::Equipping_Shield_In;
			}
		}
	}
	if (!memcmp(ActorNode->m_pcName, "Player1stPerson", 15)) // First person node exits only for the Player (no action for vanilla first person)
		AttachNode->AddObject(ObjectNode, 1);
	else {
		switch (TheEquipmentManager->vEquippingState)
		{
			case EquipmentManager::EquippingState::Equipping_None: 
			case EquipmentManager::EquippingState::Equipping_Shield_Out:
				AttachNode->AddObject(ObjectNode, 1);
				break;
			case EquipmentManager::EquippingState::Equipping_Shield_In:
				{
					NiNode* QuiverNode = (NiNode*)ActorNode->GetObjectByName(QuiverNodeName);
					NiPoint3 Rotation = {0.0f, 270.0f, 90.0f};
					NiPoint3 Position = {0.0f, 2.0f, 0.0f};
					SetRP(ObjectNode, Rotation, Position);
					QuiverNode->AddObject(ObjectNode, 1);
				}
				break;
			case EquipmentManager::EquippingState::Equipping_Weapon_In:
				{
					NiNode* QuiverNode = (NiNode*)ActorNode->GetObjectByName(QuiverNodeName);
					NiAVObject* ScbNode = ObjectNode->GetObjectByName("Scb");
					if (ScbNode) {
						ObjectNode->RemoveObject(&ScbNode, ScbNode);
						ScbNode->Destructor(true);
					}
					NiPoint3 Rotation = {15.0f, 5.0f, 0.0f};
					NiPoint3 Position = {6.0f, -30.0f, 2.0f};
					SetRP(ObjectNode, Rotation, Position);
					QuiverNode->AddObject(ObjectNode, 1);
				}
				break;
			case EquipmentManager::EquippingState::Equipping_Weapon_Out:
				{
					NiNode* TorchNode = (NiNode*)ActorNode->GetObjectByName(TorchNodeName);
					NiAVObject* ScbNode = ObjectNode->GetObjectByName("Scb");
					if (ScbNode) {
						ObjectNode->RemoveObject(&ScbNode, ScbNode);
						ScbNode->Destructor(true);
					}
					TorchNode->AddObject(ObjectNode, 1);
				}
				break;
		}
		TheEquipmentManager->vEquippingState = EquipmentManager::EquippingState::Equipping_None;
	}
}

static __declspec(naked) void PrnHook()
{
	
	__asm
	{
		pushad
		mov		ecx, dword ptr [esp+0x34]
		mov		eax, dword ptr [esp+0x40]
		mov		ebx, dword ptr [esp+0x44]
		push	ebx
		push	eax
		push	ebp
		push	esi
		push	ecx
		call	Prn
		pop		ecx
		pop		esi
		pop		ebp
		pop		eax
		pop		ebx
		popad
		jmp		kPrnReturn
	}

}

void CreateEquipmentHook()
{

	*((int *)&ProcessAction)	= 0x005FCAB0;
	TrackProcessAction			= &Equipment::TrackProcessAction;
	*((int *)&UnequipItem)		= 0x005F2E70;
	TrackUnequipItem			= &Equipment::TrackUnequipItem;
	*((int *)&EquipShield)		= 0x004E1B40;
	TrackEquipShield			= &Equipment::TrackEquipShield;

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)ProcessAction,	*((PVOID *)&TrackProcessAction));
	DetourAttach(&(PVOID&)UnequipItem,		*((PVOID *)&TrackUnequipItem));
	DetourAttach(&(PVOID&)EquipShield,		*((PVOID *)&TrackEquipShield));
	DetourTransactionCommit();
	
	WriteRelJump(kPrnHook, (UInt32)PrnHook);

}
#elif defined(SKYRIM)
#include "EquipmentManager.h"

EquipmentManager::EquipmentManager() {

	_MESSAGE("Starting the equipment manager...");
	ShieldAnim = NULL;
	LeftWeaponTemplate = NULL;
	vEquippingState = EquippingState::Equipping_None;

}

void CreateEquipmentHook()
{

}
#endif