#if defined(OBLIVION)
#include "obse_common\SafeWrite.h"
#include "obse\GameTiles.h"
#include "obse\GameMenus.h"
#include "Animation.h"
#include "Hooking\detours\detours.h"
#define ForearmTwistNodeName "Bip01 L ForearmTwist"
#define QuiverNodeName "Quiver"
#define TorchNodeName "Torch"
#define BeltNodeName "Bip01 Pelvis"
#define ScabbardNodeName "Scb"
#define FlameCapName "FlameCap"
#define FlameNode0Name "FlameNode0"
#define FlameNode1Name "FlameNode1"
#define FlameNode2Name "FlameNode2"
#define LightNodeName "AttachLight"

static const UInt32 kPrnHook = 0x0047927B;
static const UInt32 kPrnReturn = 0x0047928A;
static const UInt32 kMenuMouseButtonHook = 0x0058251B;
static const UInt32 kMenuMouseButtonReturn1 = 0x00582525;
static const UInt32 kMenuMouseButtonReturn2 = 0x0058264F;
static const UInt32 kUnequipTorchHook = 0x0048A7AD;
static const UInt32 kUnequipTorchReturn = 0x0048A7B5;

EquipmentManager::EquipmentManager() {
	
	_MESSAGE("Starting the equipment manager...");
	TheEquipmentManager = this;

	OnBackAnim = NULL;
}

void EquipmentManager::SetRotationPosition(NiAVObject* Object, PositionRotationType PositionRotation) {

	NiMatrix33* m = &Object->m_localTransform.rot;
	NiPoint3* v = &Object->m_localTransform.pos;

	if (PositionRotation == PositionRotationType::None) {
		NiPoint3 Rotation = { 0.0f, 0.0f, 0.0f };
		NiPoint3 Position = { 0.0f, 0.0f, 0.0f };
		TheUtilityManager->GenerateRotationMatrixZXY(m, &Rotation, 1);
		TheUtilityManager->MatrixVectorMultiply(v, m, &Position);
	}
	else if (PositionRotation == PositionRotationType::Shield) {
		TheUtilityManager->GenerateRotationMatrixZXY(m, &TheSettingManager->SettingsMain.EquipmentModeShieldOnBackRot, 1);
		TheUtilityManager->MatrixVectorMultiply(v, m, &TheSettingManager->SettingsMain.EquipmentModeShieldOnBackPos);
	}
	else if (PositionRotation == PositionRotationType::Weapon) {
		TheUtilityManager->GenerateRotationMatrixZXY(m, &TheSettingManager->SettingsMain.EquipmentModeWeaponOnBackRot, 1);
		TheUtilityManager->MatrixVectorMultiply(v, m, &TheSettingManager->SettingsMain.EquipmentModeWeaponOnBackPos);
	}
	else if (PositionRotation == PositionRotationType::Light) {
		TheUtilityManager->GenerateRotationMatrixZXY(m, &TheSettingManager->SettingsMain.EquipmentModeTorchOnBeltRot, 1);
		TheUtilityManager->MatrixVectorMultiply(v, m, &TheSettingManager->SettingsMain.EquipmentModeTorchOnBeltPos);
	}

}

void EquipmentManager::SetTorchLight(NiAVObject* Torch, UInt8 Enable)
{

	NiAVObject* FlameCap = Torch->GetObjectByName(FlameCapName);
	NiAVObject* FlameNode0 = Torch->GetObjectByName(FlameNode0Name);
	NiAVObject* FlameNode1 = Torch->GetObjectByName(FlameNode1Name);
	NiAVObject* FlameNode2 = Torch->GetObjectByName(FlameNode2Name);
	NiNode* AttachLight = (NiNode*)Torch->GetObjectByName(LightNodeName);

	if (Enable) {
		if (FlameCap) FlameCap->m_flags &= ~NiAVObject::kFlag_AppCulled;
		if (FlameNode0) FlameNode0->m_flags &= ~NiAVObject::kFlag_AppCulled;
		if (FlameNode1) FlameNode1->m_flags &= ~NiAVObject::kFlag_AppCulled;
		if (FlameNode2) FlameNode2->m_flags &= ~NiAVObject::kFlag_AppCulled;
		if (AttachLight) {
			NiPointLight* Light = (NiPointLight*)AttachLight->m_children.data[0];
			Light->m_flags &= ~NiAVObject::kFlag_AppCulled;
		}
	}
	else {
		if (FlameCap) FlameCap->m_flags |= NiAVObject::kFlag_AppCulled;
		if (FlameNode0) FlameNode0->m_flags |= NiAVObject::kFlag_AppCulled;
		if (FlameNode1) FlameNode1->m_flags |= NiAVObject::kFlag_AppCulled;
		if (FlameNode2) FlameNode2->m_flags |= NiAVObject::kFlag_AppCulled;
		if (AttachLight) {
			NiPointLight* Light = (NiPointLight*)AttachLight->m_children.data[0];
			Light->m_flags |= NiAVObject::kFlag_AppCulled;
		}
	}
}

void EquipmentManager::SetAnimations(HighProcess* Process, UInt8 LeftWeapon)
{
	if (LeftWeapon) {
		SetAnimGroup(Process, 0, 2, "Characters\\_Male\\OnehandIdle_OR_dual.kf");
		SetAnimGroup(Process, 3, 2, "Characters\\_Male\\OnehandForward_OR_dual.kf");
		SetAnimGroup(Process, 4, 2, "Characters\\_Male\\OnehandBackward_OR_dual.kf");
		SetAnimGroup(Process, 5, 2, "Characters\\_Male\\OnehandLeft_OR_dual.kf");
		SetAnimGroup(Process, 6, 2, "Characters\\_Male\\OnehandRight_OR_dual.kf");
		SetAnimGroup(Process, 7, 2, "Characters\\_Male\\OnehandFastForward_OR_dual.kf");
		SetAnimGroup(Process, 8, 2, "Characters\\_Male\\OnehandFastBackward_OR_dual.kf");
		SetAnimGroup(Process, 9, 2, "Characters\\_Male\\OnehandFastLeft_OR_dual.kf");
		SetAnimGroup(Process, 10, 2, "Characters\\_Male\\OnehandFastRight_OR_dual.kf");
		SetAnimGroup(Process, 15, 2, "Characters\\_Male\\OnehandTurnLeft_OR_dual.kf");
		SetAnimGroup(Process, 16, 2, "Characters\\_Male\\OnehandTurnRight_OR_dual.kf");
		SetAnimGroup(Process, 20, 2, "Characters\\_Male\\OnehandAttackLeft_OR_dual.kf");
		SetAnimGroup(Process, 21, 2, "Characters\\_Male\\OnehandAttackRight_OR_dual.kf");
		SetAnimGroup(Process, 27, 0, "Characters\\_Male\\BlockIdle_OR_dual.kf");
		SetAnimGroup(Process, 28, 0, "Characters\\_Male\\BlockHit_OR_dual.kf");
	}
	else {
		ResetAnimGroup(Process, 0, 2);
		ResetAnimGroup(Process, 3, 2);
		ResetAnimGroup(Process, 4, 2);
		ResetAnimGroup(Process, 5, 2);
		ResetAnimGroup(Process, 6, 2);
		ResetAnimGroup(Process, 7, 2);
		ResetAnimGroup(Process, 8, 2);
		ResetAnimGroup(Process, 9, 2);
		ResetAnimGroup(Process, 10, 2);
		ResetAnimGroup(Process, 15, 2);
		ResetAnimGroup(Process, 16, 2);
		ResetAnimGroup(Process, 20, 2);
		ResetAnimGroup(Process, 21, 2);
		ResetAnimGroup(Process, 27, 0);
		ResetAnimGroup(Process, 28, 0);
	}
}

void EquipmentManager::SetAnimGroup(HighProcess* Process, UInt8 AnimGroup, UInt8 AnimType, const char* Filename)
{
	BSAnimGroupSequence* AnimGroupSequence = NULL;
	ActorAnimDataEx* AnimData = (ActorAnimDataEx*)Process->animData;
	AnimSequenceBase* AnimSequence = NULL;
	UInt16 AnimGroupKey = (AnimType << 8) | AnimGroup;

	if (TheUtilityManager->GetAnimSequence(AnimData->animsMap, AnimGroupKey, &AnimSequence)) {
		NiTList<BSAnimGroupSequence>::Node* Iter = AnimData->ORAnims->start;
		while (Iter) {
			AnimGroupSequence = Iter->data;
			if (!stricmp(AnimGroupSequence->filePath, Filename)) break;
			Iter = Iter->next;
			AnimGroupSequence = NULL;
		}
		if (AnimSequence->IsSingle()) {
			AnimSequenceSingleEx* AnimSequenceS = (AnimSequenceSingleEx*)AnimSequence;
			AnimSequenceS->ORAnim = AnimGroupSequence;
		}
		else {
			AnimSequenceMultipleEx* AnimSequenceM = (AnimSequenceMultipleEx*)AnimSequence;
			AnimSequenceM->ORAnim = AnimGroupSequence;
		}
	}
}

void EquipmentManager::ResetAnimGroup(HighProcess* Process, UInt8 AnimGroup, UInt8 AnimType)
{
	ActorAnimData* AnimData = Process->animData;
	AnimSequenceBase* AnimSequence = NULL;
	UInt16 AnimGroupKey = (AnimType << 8) | AnimGroup;

	if (TheUtilityManager->GetAnimSequence(AnimData->animsMap, AnimGroupKey, &AnimSequence)) {
		if (AnimSequence->IsSingle()) {
			AnimSequenceSingleEx* AnimSequenceS = (AnimSequenceSingleEx*)AnimSequence;
			AnimSequenceS->ORAnim = NULL;
		}
		else {
			AnimSequenceMultipleEx* AnimSequenceM = (AnimSequenceMultipleEx*)AnimSequence;
			AnimSequenceM->ORAnim = NULL;
		}
	}
}

class Equipment {

public:
	HighProcess* TrackNewHighProcess();
	void TrackManageItem(int SelectedID, TileRect* SelectedRect);
	int TrackProcessAction(float Arg1, float Arg2);
	ExtraContainerChanges::EntryData* TrackGetEquippedWeaponData(UInt8 Arg1);
	bool TrackSetEquippedWeaponData(ExtraContainerChanges::EntryData* InventoryData, NiNode* ActorNode);
	void TrackEquipItem(TESForm* Form, int Quantity, Actor* Act, ExtraDataList* DataList, UInt8 Left, UInt8 Lock);
	void TrackUnequipItem(UInt8* Arg1, TESForm* Form, int Quantity, Actor* Act, ExtraDataList* DataList, UInt8 Left, UInt8 Lock);
	void TrackEquipWeapon(TESObjectWEAP* Weapon);
	void TrackUnequipWeapon();
	void TrackEquipShield(TESObjectARMO* Shield);
	void TrackEquipLight(TESObjectLIGH* Light);
	void TrackUnequipLight();
	void TrackHideEquipment(TESForm* Form, UInt8 Arg2);
	ExtraContainerChanges::EntryData* TrackGetEquippedLightData(UInt8 Arg1);

};

HighProcess* (__thiscall Equipment::* NewHighProcess)();
HighProcess* (__thiscall Equipment::* TrackNewHighProcess)();
HighProcess* Equipment::TrackNewHighProcess() {

	HighProcessEx* Process = (HighProcessEx*)(this->*NewHighProcess)();

	Process->EquippedLeftWeaponData = NULL;
	Process->LeftWeaponObject = NULL;
	Process->LeftWeaponObjectFirst = NULL;
	Process->OnBackActionState = HighProcessEx::State::None;
	Process->OnBackState = HighProcessEx::State::In;
	Process->EquippingState = HighProcessEx::State::None;
	Process->LeftEquippingState = HighProcessEx::State::None;
	Process->OnBeltActionState = HighProcessEx::State::None;
	Process->OnBeltState = HighProcessEx::State::In;
	return Process;

}

void (__thiscall Equipment::* ManageItem)(int, TileRect*);
void (__thiscall Equipment::* TrackManageItem)(int, TileRect*);
void Equipment::TrackManageItem(int SelectedID, TileRect* SelectedRect) {
	
	HighProcessEx* Process = (HighProcessEx*)(*g_thePlayer)->process;

	if (!memcmp(SelectedRect->name.m_data, "inv_OREquipment_button", 22) && TheScriptManager->EquipmentSetup->ConfigStep == EquipmentSetupScript::StepType::None) TheScriptManager->EquipmentSetup->ConfigStep = EquipmentSetupScript::StepType::Request;
	if (TheKeyboardManager->OnMouseUp(1)) {
		float fIndex = 0.0f;
		if (SelectedRect->GetFloatValue(kTileValue_user11, &fIndex)) {
			ExtraContainerChanges::EntryData* InventoryData = TheUtilityManager->GetInventoryItem(*g_thePlayer, fIndex);
			if (InventoryData->type->typeID == FormType::kFormType_Weapon) {
				TESObjectWEAP* Weapon = (TESObjectWEAP*)InventoryData->type;
				if ((Weapon->type == TESObjectWEAP::kType_BladeOneHand || Weapon->type == TESObjectWEAP::kType_BluntOneHand) && !(Weapon->flags & 0x400) && !Weapon->scriptable.script) Process->LeftEquippingState = HighProcessEx::State::In;
			}
		}
	}
	(this->*ManageItem)(SelectedID, SelectedRect);
	Process->LeftEquippingState = HighProcessEx::State::None;

}

int (__thiscall Equipment::* ProcessAction)(float, float);
int (__thiscall Equipment::* TrackProcessAction)(float, float);
int Equipment::TrackProcessAction(float Arg1, float Arg2) {

	Character* Act = (Character*)this;
	HighProcessEx* Process = (HighProcessEx*)Act->process;

	if (Process && Process->GetProcessLevel() == 0) {
		ExtraContainerChanges::EntryData* WeaponData = Process->equippedWeaponData;
		ExtraContainerChanges::EntryData* ShieldData = Process->equippedShieldData;
		ExtraContainerChanges::EntryData* LightData = Process->equippedLightData;
		ExtraContainerChanges::EntryData* LeftWeaponData = Process->EquippedLeftWeaponData;
		if (WeaponData && (ShieldData || LeftWeaponData) && (!LightData || (LightData && Process->OnBeltState == HighProcessEx::State::In))) {
			TESObjectWEAP* Weapon = (TESObjectWEAP*)WeaponData->type;
			if (Weapon->type == TESObjectWEAP::kType_BladeOneHand || Weapon->type == TESObjectWEAP::kType_BluntOneHand) {
				switch (Process->currentAction)
				{
					case HighProcess::kAction_None:
						if (Process->OnBackActionState == HighProcessEx::State::None) {
							if (Process->WeaponState && Process->OnBackState == HighProcessEx::State::In)
								Process->OnBackActionState = HighProcessEx::State::Out;
							else if (!Process->WeaponState && Process->OnBackState == HighProcessEx::State::Out)
								Process->OnBackActionState = HighProcessEx::State::In;
						}
						if (Process->OnBackActionState == HighProcessEx::State::In || Process->OnBackActionState == HighProcessEx::State::Out) TheUtilityManager->QueueIdle(Process->animData, TheEquipmentManager->OnBackAnim, Act, TheEquipmentManager->OnBackAnim->animFlags);
						break;
					case HighProcess::kAction_SpecialIdle:
						if (Process->OnBackActionState == HighProcessEx::State::In || Process->OnBackActionState == HighProcessEx::State::Out) {
							BSAnimGroupSequence* AnimSequence = Process->animData->animSequences[4];
							if (AnimSequence && !memcmp(AnimSequence->filePath, "Meshes\\Characters\\_Male\\IdleAnims\\oronbackanim.kf", 49) && AnimSequence->last >= 0.4) {
								NiNode* ForearmTwistNode = Act->ActorSkinInfo->LForearmTwistNode;
								NiNode* QuiverNode = Act->ActorSkinInfo->QuiverNode;
								NiNode* TorchNode = Act->ActorSkinInfo->TorchNode;
								NiAVObject* Object = NULL;
								if (Process->OnBackActionState == HighProcessEx::State::In) {
									if (LeftWeaponData) {
										Object = Process->LeftWeaponObject;
										TorchNode->RemoveObject(&Object, Object);
										TheEquipmentManager->SetRotationPosition(Object, EquipmentManager::PositionRotationType::Weapon);
										QuiverNode->AddObject(Object, 1);
										if (Act == *g_thePlayer) {
											Object = Process->LeftWeaponObjectFirst;
											QuiverNode = (*g_thePlayer)->firstPersonSkinInfo->QuiverNode;
											TorchNode = (*g_thePlayer)->firstPersonSkinInfo->TorchNode;
											TorchNode->RemoveObject(&Object, Object);
											TheEquipmentManager->SetRotationPosition(Object, EquipmentManager::PositionRotationType::Weapon);
											QuiverNode->AddObject(Object, 1);
										}
									}
									else {
										Object = Act->ActorSkinInfo->ShieldObject;
										ForearmTwistNode->RemoveObject(&Object, Object);
										TheEquipmentManager->SetRotationPosition(Object, EquipmentManager::PositionRotationType::Shield);
										QuiverNode->AddObject(Object, 1);
										if (Act == *g_thePlayer) {
											Object = (*g_thePlayer)->firstPersonSkinInfo->ShieldObject;
											ForearmTwistNode = (*g_thePlayer)->firstPersonSkinInfo->LForearmTwistNode;
											QuiverNode = (*g_thePlayer)->firstPersonSkinInfo->QuiverNode;
											ForearmTwistNode->RemoveObject(&Object, Object);
											TheEquipmentManager->SetRotationPosition(Object, EquipmentManager::PositionRotationType::Shield);
											QuiverNode->AddObject(Object, 1);
										}
									}
									Process->OnBackState = HighProcessEx::State::In;
								}
								else {
									if (LeftWeaponData) {
										Object = Process->LeftWeaponObject;
										QuiverNode->RemoveObject(&Object, Object);
										TheEquipmentManager->SetRotationPosition(Object, EquipmentManager::PositionRotationType::None);
										TorchNode->AddObject(Object, 1);
										if (Act == *g_thePlayer) {
											Object = Process->LeftWeaponObjectFirst;
											QuiverNode = (*g_thePlayer)->firstPersonSkinInfo->QuiverNode;
											TorchNode = (*g_thePlayer)->firstPersonSkinInfo->TorchNode;
											QuiverNode->RemoveObject(&Object, Object);
											TheEquipmentManager->SetRotationPosition(Object, EquipmentManager::PositionRotationType::None);
											TorchNode->AddObject(Object, 1);
										}
									}
									else {
										Object = Act->ActorSkinInfo->ShieldObject;
										QuiverNode->RemoveObject(&Object, Object);
										TheEquipmentManager->SetRotationPosition(Object, EquipmentManager::PositionRotationType::None);
										ForearmTwistNode->AddObject(Object, 1);
										if (Act == *g_thePlayer) {
											Object = (*g_thePlayer)->firstPersonSkinInfo->ShieldObject;
											QuiverNode = (*g_thePlayer)->firstPersonSkinInfo->QuiverNode;
											ForearmTwistNode = (*g_thePlayer)->firstPersonSkinInfo->LForearmTwistNode;
											QuiverNode->RemoveObject(&Object, Object);
											TheEquipmentManager->SetRotationPosition(Object, EquipmentManager::PositionRotationType::None);
											ForearmTwistNode->AddObject(Object, 1);
										}
									}
									Process->OnBackState = HighProcessEx::State::Out;
								}
								Process->OnBackActionState = HighProcessEx::State::None;
							}
						}
						break;
					default:
						break;
				}
			}
		}
		if (LightData && Process->OnBackState == HighProcessEx::State::In && TheKeyboardManager->OnKeyDown(TheSettingManager->SettingsMain.EquipmentModeTorchKey)) {
			NiNode* BeltNode = (NiNode*)Act->niNode->GetObjectByName(BeltNodeName);
			NiNode* TorchNode = Act->ActorSkinInfo->TorchNode;
			NiAVObject* Object = NULL;
			if (Process->OnBeltState == HighProcessEx::State::In) {
				Object = Act->ActorSkinInfo->LightObject;
				BeltNode->RemoveObject(&Object, Object);
				TheEquipmentManager->SetTorchLight(Object, 1);
				TheEquipmentManager->SetRotationPosition(Object, EquipmentManager::PositionRotationType::None);
				TorchNode->AddObject(Object, 1);
				if (Act == *g_thePlayer) {
					Object = (*g_thePlayer)->firstPersonSkinInfo->LightObject;
					BeltNode = (NiNode*)(*g_thePlayer)->firstPersonNiNode->GetObjectByName(BeltNodeName);
					TorchNode = (*g_thePlayer)->firstPersonSkinInfo->TorchNode;
					BeltNode->RemoveObject(&Object, Object);
					TheEquipmentManager->SetTorchLight(Object, 1);
					TheEquipmentManager->SetRotationPosition(Object, EquipmentManager::PositionRotationType::None);
					TorchNode->AddObject(Object, 1);
				}
				Process->OnBeltState = HighProcessEx::State::Out;
			}
			else {
				Object = Act->ActorSkinInfo->LightObject;
				TorchNode->RemoveObject(&Object, Object);
				TheEquipmentManager->SetTorchLight(Object, 0);
				TheEquipmentManager->SetRotationPosition(Object, EquipmentManager::PositionRotationType::Light);
				BeltNode->AddObject(Object, 1);
				if (Act == *g_thePlayer) {
					Object = (*g_thePlayer)->firstPersonSkinInfo->LightObject;
					BeltNode = (NiNode*)(*g_thePlayer)->firstPersonNiNode->GetObjectByName(BeltNodeName);
					TorchNode = (*g_thePlayer)->firstPersonSkinInfo->TorchNode;
					TorchNode->RemoveObject(&Object, Object);
					TheEquipmentManager->SetTorchLight(Object, 0);
					TheEquipmentManager->SetRotationPosition(Object, EquipmentManager::PositionRotationType::Light);
					BeltNode->AddObject(Object, 1);
				}
				Process->OnBeltState = HighProcessEx::State::In;
			}
		}
	}
	return (this->*ProcessAction)(Arg1, Arg2);

}

ExtraContainerChanges::EntryData* (__thiscall Equipment::* GetEquippedWeaponData)(UInt8);
ExtraContainerChanges::EntryData* (__thiscall Equipment::* TrackGetEquippedWeaponData)(UInt8);
ExtraContainerChanges::EntryData* Equipment::TrackGetEquippedWeaponData(UInt8 Arg1) {
	
	HighProcessEx* Process = (HighProcessEx*)this;

	if (Process->GetProcessLevel() == 0 && Process->LeftEquippingState) return Process->EquippedLeftWeaponData;
	return (this->*GetEquippedWeaponData)(Arg1);

}

bool (__thiscall Equipment::* SetEquippedWeaponData)(ExtraContainerChanges::EntryData*, NiNode*);
bool (__thiscall Equipment::* TrackSetEquippedWeaponData)(ExtraContainerChanges::EntryData*, NiNode*);
bool Equipment::TrackSetEquippedWeaponData(ExtraContainerChanges::EntryData* InventoryData, NiNode* ActorNode) {	
	
	HighProcessEx* Process = (HighProcessEx*)this;
	
	if (Process->GetProcessLevel() == 0 && Process->LeftEquippingState) {
		Process->EquippedLeftWeaponData = InventoryData;
		return 1;
	}
	return (this->*SetEquippedWeaponData)(InventoryData, ActorNode);

}

void (__thiscall Equipment::* EquipItem)(TESForm*, int, Actor*, ExtraDataList*, UInt8, UInt8);
void (__thiscall Equipment::* TrackEquipItem)(TESForm*, int, Actor*, ExtraDataList*, UInt8, UInt8);
void Equipment::TrackEquipItem(TESForm* Form, int Quantity, Actor* Act, ExtraDataList* DataList, UInt8 Left, UInt8 Lock) {
	
	HighProcessEx* Process = (HighProcessEx*)Act->process;

	if (Process && Process->GetProcessLevel() == 0 && Process->LeftEquippingState) Left = 1;
	(this->*EquipItem)(Form, Quantity, Act, DataList, Left, Lock);

}

void (__thiscall Equipment::* UnequipItem)(UInt8*, TESForm*, int, Actor*, ExtraDataList*, UInt8, UInt8);
void (__thiscall Equipment::* TrackUnequipItem)(UInt8*, TESForm*, int, Actor*, ExtraDataList*, UInt8, UInt8);
void Equipment::TrackUnequipItem(UInt8* Arg1, TESForm* Form, int Quantity, Actor* Act, ExtraDataList* DataList, UInt8 Left, UInt8 Lock) {
	
	HighProcessEx* Process = (HighProcessEx*)Act->process;

	if (Process && Process->GetProcessLevel() == 0 && Form->typeID == FormType::kFormType_Weapon && Left) Process->LeftEquippingState = HighProcessEx::State::Out;
	(this->*UnequipItem)(Arg1, Form, Quantity, Act, DataList, Left, Lock);
	if (Process && Process->GetProcessLevel() == 0 && Form->typeID == FormType::kFormType_Weapon && Left) Process->LeftEquippingState = HighProcessEx::State::None;

}

void (__thiscall Equipment::* EquipWeapon)(TESObjectWEAP*);
void (__thiscall Equipment::* TrackEquipWeapon)(TESObjectWEAP*);
void Equipment::TrackEquipWeapon(TESObjectWEAP* Weapon) {

	Actor* Act = (Actor*)this;
	HighProcessEx* Process = (HighProcessEx*)Act->process;

	if (Process && Process->GetProcessLevel() == 0 && Process->LeftEquippingState) {
		NiNode* (__cdecl * LoadNiFile)(char*, UInt32, Actor*, NiNode*) = (NiNode*(__cdecl *)(char*, UInt32, Actor*, NiNode*))0x00479450;
		Process->EquippingState = HighProcessEx::State::In;
		if (Process->WeaponState)
			Process->OnBackState = HighProcessEx::State::Out;
		else
			Process->OnBackState = HighProcessEx::State::In;
		NiNode* ObjectNode = LoadNiFile(Weapon->model.nifPath.m_data, 9, Act, Act->niNode);
		ObjectNode->SetName("LeftWeapon");
		Process->LeftWeaponObject = ObjectNode;
		if (Act == *g_thePlayer) {
			ObjectNode = LoadNiFile(Weapon->model.nifPath.m_data, 9, *g_thePlayer, (*g_thePlayer)->firstPersonNiNode);
			ObjectNode->SetName("LeftWeapon");
			Process->LeftWeaponObjectFirst = ObjectNode;
		}
		Process->EquippingState = HighProcessEx::State::None;
		TheEquipmentManager->SetAnimations(Process, 1);
	}
	else (this->*EquipWeapon)(Weapon);

}

void (__thiscall Equipment::* UnequipWeapon)();
void (__thiscall Equipment::* TrackUnequipWeapon)();
void Equipment::TrackUnequipWeapon() {

	Actor* Act = (Actor*)this;
	HighProcessEx* Process = (HighProcessEx*)Act->process;

	if (Process && Process->GetProcessLevel() == 0 && Process->LeftEquippingState) {
		char* WeaponModelPath = ((TESObjectWEAP*)Process->EquippedLeftWeaponData->type)->model.nifPath.m_data;
		NiAVObject* Object = Process->LeftWeaponObject;
		NiNode* ParentNode = (NiNode*)Object->m_parent;
		ParentNode->RemoveObject(&Object, Object);
		Object->Destructor(1);
		TheUtilityManager->RemoveModel(WeaponModelPath);
		Process->LeftWeaponObject = NULL;
		if (Act == *g_thePlayer) {
			Object = Process->LeftWeaponObjectFirst;
			ParentNode = (NiNode*)Object->m_parent;
			ParentNode->RemoveObject(&Object, Object);
			Object->Destructor(1);
			TheUtilityManager->RemoveModel(WeaponModelPath);
			Process->LeftWeaponObjectFirst = NULL;
		}
		TheEquipmentManager->SetAnimations(Process, 0);
		if (Process->equippedShieldData && Process->WeaponState) Process->OnBackActionState = HighProcessEx::State::Out;
	}
	else (this->*UnequipWeapon)();

}

void (__thiscall Equipment::* EquipShield)(TESObjectARMO*);
void (__thiscall Equipment::* TrackEquipShield)(TESObjectARMO*);
void Equipment::TrackEquipShield(TESObjectARMO* Shield) {

	Actor* Act = (Actor*)this;
	HighProcessEx* Process = (HighProcessEx*)Act->process;

	if (Process && Process->GetProcessLevel() == 0) {
		Process->EquippingState = HighProcessEx::State::In;
		if (Process->WeaponState)
			Process->OnBackState = HighProcessEx::State::Out;
		else
			Process->OnBackState = HighProcessEx::State::In;
	}
	(this->*EquipShield)(Shield);
	Process->EquippingState = HighProcessEx::State::None;

}

void (__thiscall Equipment::* EquipLight)(TESObjectLIGH*);
void (__thiscall Equipment::* TrackEquipLight)(TESObjectLIGH*);
void Equipment::TrackEquipLight(TESObjectLIGH* Light) {

	Actor* Act = (Actor*)this;
	HighProcessEx* Process = (HighProcessEx*)Act->process;

	if (Act == *g_thePlayer && Process) {
		Process->EquippingState = HighProcessEx::State::Out;
		Process->OnBeltState = HighProcessEx::State::In;
	}
	(this->*EquipLight)(Light);
	Process->EquippingState = HighProcessEx::State::None;

}

void (__thiscall Equipment::* UnequipLight)();
void (__thiscall Equipment::* TrackUnequipLight)();
void Equipment::TrackUnequipLight() {

	PlayerCharacter* Act = (PlayerCharacter*)this;
	HighProcessEx* Process = (HighProcessEx*)Act->process;

	if (Act == *g_thePlayer && Process && Process->OnBeltState == HighProcessEx::State::In && Act->niNode) {
		NiNode* BeltNode = (NiNode*)Act->niNode->GetObjectByName(BeltNodeName);
		NiNode* TorchNode = Act->ActorSkinInfo->TorchNode;
		NiAVObject* Object = Act->ActorSkinInfo->LightObject;
		BeltNode->RemoveObject(&Object, Object);
		TheEquipmentManager->SetTorchLight(Object, 1);
		TheEquipmentManager->SetRotationPosition(Object, EquipmentManager::PositionRotationType::None);
		TorchNode->AddObject(Object, 1);
		Object = Act->firstPersonSkinInfo->LightObject;
		BeltNode = (NiNode*)Act->firstPersonNiNode->GetObjectByName(BeltNodeName);
		TorchNode = Act->firstPersonSkinInfo->TorchNode;
		BeltNode->RemoveObject(&Object, Object);
		TheEquipmentManager->SetTorchLight(Object, 1);
		TheEquipmentManager->SetRotationPosition(Object, EquipmentManager::PositionRotationType::None);
		TorchNode->AddObject(Object, 1);
	}
	(this->*UnequipLight)();

}

void (__thiscall Equipment::* HideEquipment)(TESForm*, UInt8);
void (__thiscall Equipment::* TrackHideEquipment)(TESForm*, UInt8);
void Equipment::TrackHideEquipment(TESForm* Form, UInt8 Arg2) {

	Actor* Act = (Actor*)this;

	if (Act == *g_thePlayer) return;
	(this->*HideEquipment)(Form, Arg2);

}

ExtraContainerChanges::EntryData* (__thiscall Equipment::* GetEquippedLightData)(UInt8);
ExtraContainerChanges::EntryData* (__thiscall Equipment::* TrackGetEquippedLightData)(UInt8);
ExtraContainerChanges::EntryData* Equipment::TrackGetEquippedLightData(UInt8 Arg1) {

	HighProcessEx* Process = (HighProcessEx*)this;

	if (Process == (*g_thePlayer)->process && Process->OnBeltState == HighProcessEx::State::In) return NULL;
	return (this->*GetEquippedLightData)(Arg1);

}

void UnequipTorch(Actor* Act)
{
	HighProcessEx* Process = (HighProcessEx*)Act->process;
	
	if (Act == *g_thePlayer) Process->OnBeltState = HighProcessEx::State::Out;
}

static __declspec(naked) void UnequipTorchHook()
{

	__asm
	{
		pushad
		push	ebp
		call	UnequipTorch
		add		esp, 4
		popad
		mov     edx, [ecx]
		mov     eax, [edx + 0xF0]
		jmp		kUnequipTorchReturn
	}

}

void Prn(NiNode* ActorNode, NiNode* AttachToNode, NiNode* ObjectNode, SkinInfo* Info, int SkinInfoSlot) {

	Character* Act = NULL;
	HighProcessEx* Process = NULL;

	if (SkinInfoSlot == 13) {
		Act = (Character*)Info->Actor150;
		if (Act->process->GetProcessLevel() == 0) { 
			Process = (HighProcessEx*)Act->process;
			Process->EquippingState = HighProcessEx::State::In;
		}
	}
	else if (!Info) {
		if (!memcmp(ActorNode->m_pcName, "Player1stPerson", 15)) {
			Act = *g_thePlayer;
			Info = (*g_thePlayer)->firstPersonSkinInfo;
			Process = (HighProcessEx*)Act->process;
		}
		else {
			TESObjectExtraData* ExtraData = TheUtilityManager->GetRef(ActorNode);
			Act = (Character*)ExtraData->refr;
			Info = Act->ActorSkinInfo;
			if (Act->process->GetProcessLevel() == 0) Process = (HighProcessEx*)Act->process;
		}
	}
	if (Process && Process->EquippingState) {
		if (Process->EquippingState == HighProcessEx::State::In) {
			NiNode* ForearmTwistNode = (NiNode*)ActorNode->GetObjectByName(ForearmTwistNodeName);
			NiNode* TorchNode = (NiNode*)ActorNode->GetObjectByName(TorchNodeName);
			NiNode* QuiverNode = (NiNode*)ActorNode->GetObjectByName(QuiverNodeName);
			NiAVObject* Scabbard = ObjectNode->GetObjectByName(ScabbardNodeName);
			NiAVObject* Object = NULL;
			if (Process->LeftEquippingState && Scabbard) {
				ObjectNode->RemoveObject(&Scabbard, Scabbard);
				Scabbard->Destructor(1);
			}
			if (!Process->WeaponState) {
				if (Process->LeftEquippingState)
					TheEquipmentManager->SetRotationPosition(ObjectNode, EquipmentManager::PositionRotationType::Weapon);
				else
					TheEquipmentManager->SetRotationPosition(ObjectNode, EquipmentManager::PositionRotationType::Shield);
				QuiverNode->AddObject(ObjectNode, 1);
			}
			else {
				if (Process->LeftEquippingState) {
					if (Process->equippedShieldData) {
						Object = Info->ShieldObject;
						ForearmTwistNode->RemoveObject(&Object, Object);
						TheEquipmentManager->SetRotationPosition(Object, EquipmentManager::PositionRotationType::Shield);
						QuiverNode->AddObject(Object, 1);
					}
					TorchNode->AddObject(ObjectNode, 1);
				}
				else {
					if (Process->EquippedLeftWeaponData) {
						TheEquipmentManager->SetRotationPosition(ObjectNode, EquipmentManager::PositionRotationType::Shield);
						QuiverNode->AddObject(ObjectNode, 1);
					}
					else
						AttachToNode->AddObject(ObjectNode, 1);
				}
			}
			if (SkinInfoSlot == 13) Process->EquippingState = HighProcessEx::State::None;
		}
		else {
			NiNode* BeltNode = (NiNode*)ActorNode->GetObjectByName(BeltNodeName);
			TheEquipmentManager->SetTorchLight(ObjectNode, 0);
			TheEquipmentManager->SetRotationPosition(ObjectNode, EquipmentManager::PositionRotationType::Light);
			BeltNode->AddObject(ObjectNode, 1);
		}
	}
	else
		AttachToNode->AddObject(ObjectNode, 1);

}

static __declspec(naked) void PrnHook()
{

	__asm
	{
		pushad
		mov		ecx, dword ptr[esp + 0x34]
		mov		eax, dword ptr[esp + 0x40]
		mov		ebx, dword ptr[esp + 0x44]
		push	ebx
		push	eax
		push	ebp
		push	esi
		push	ecx
		call	Prn
		add		esp, 0x14
		popad
		jmp		kPrnReturn
	}

}

int MouseMenuButton(InterfaceManager* Im, UInt8 Button1State) {
	
	if (Button1State || (Im->activeMenu && Im->activeMenu->id == kMenuType_Inventory && TheKeyboardManager->OnMouseUp(1))) return 1;
	return 0;

}

static __declspec(naked) void MenuMouseButtonHook()
{

	__asm
	{
		pushad
		mov		eax, [esp + 0x68]
		push	eax
		push	esi
		call	MouseMenuButton
		add		esp, 8
		cmp		eax, 0
		jz		short loc_notpressed
		popad
		jmp		kMenuMouseButtonReturn1

	loc_notpressed:
		popad
		jmp		kMenuMouseButtonReturn2
	}

}

void CreateEquipmentHook()
{
	*((int *)&NewHighProcess)			= 0x00628EE0;
	TrackNewHighProcess					= &Equipment::TrackNewHighProcess;
	*((int *)&ManageItem)				= 0x005ABBC0;
	TrackManageItem						= &Equipment::TrackManageItem;
	*((int *)&ProcessAction)			= 0x005FCAB0;
	TrackProcessAction					= &Equipment::TrackProcessAction;
	*((int *)&GetEquippedWeaponData)	= 0x0064B240;
	TrackGetEquippedWeaponData			= &Equipment::TrackGetEquippedWeaponData;
	*((int *)&SetEquippedWeaponData)	= 0x00658550;
	TrackSetEquippedWeaponData			= &Equipment::TrackSetEquippedWeaponData;
	*((int *)&EquipItem)				= 0x00489C30;
	TrackEquipItem						= &Equipment::TrackEquipItem;
	*((int *)&UnequipItem)				= 0x0048A540;
	TrackUnequipItem					= &Equipment::TrackUnequipItem;
	*((int *)&EquipWeapon)				= 0x004E1900;
	TrackEquipWeapon					= &Equipment::TrackEquipWeapon;
	*((int *)&UnequipWeapon)			= 0x004DC5B0;
	TrackUnequipWeapon					= &Equipment::TrackUnequipWeapon;
	*((int *)&EquipShield)				= 0x004E1B40;
	TrackEquipShield					= &Equipment::TrackEquipShield;
	*((int *)&EquipLight)				= 0x004E1C70;
	TrackEquipLight						= &Equipment::TrackEquipLight;
	*((int *)&UnequipLight)				= 0x004DCAB0;
	TrackUnequipLight					= &Equipment::TrackUnequipLight;
	*((int *)&HideEquipment)			= 0x005E7230;
	TrackHideEquipment					= &Equipment::TrackHideEquipment;
	*((int *)&GetEquippedLightData)		= 0x0064B270;
	TrackGetEquippedLightData			= &Equipment::TrackGetEquippedLightData;

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)NewHighProcess,			*((PVOID *)&TrackNewHighProcess));
	DetourAttach(&(PVOID&)ManageItem,				*((PVOID *)&TrackManageItem));
	DetourAttach(&(PVOID&)ProcessAction,			*((PVOID *)&TrackProcessAction));
	DetourAttach(&(PVOID&)GetEquippedWeaponData,	*((PVOID *)&TrackGetEquippedWeaponData));
	DetourAttach(&(PVOID&)SetEquippedWeaponData,	*((PVOID *)&TrackSetEquippedWeaponData));
	DetourAttach(&(PVOID&)EquipItem,				*((PVOID *)&TrackEquipItem));
	DetourAttach(&(PVOID&)UnequipItem,				*((PVOID *)&TrackUnequipItem));
	DetourAttach(&(PVOID&)EquipWeapon,				*((PVOID *)&TrackEquipWeapon));
	DetourAttach(&(PVOID&)UnequipWeapon,			*((PVOID *)&TrackUnequipWeapon));
	DetourAttach(&(PVOID&)EquipShield,				*((PVOID *)&TrackEquipShield));
	DetourAttach(&(PVOID&)EquipLight,				*((PVOID *)&TrackEquipLight));
	DetourAttach(&(PVOID&)UnequipLight,				*((PVOID *)&TrackUnequipLight));
	DetourAttach(&(PVOID&)HideEquipment,			*((PVOID *)&TrackHideEquipment));
	DetourAttach(&(PVOID&)GetEquippedLightData,		*((PVOID *)&TrackGetEquippedLightData));
	DetourTransactionCommit();

	// Extends the HighProcess allocation (for each constructor call) from size 0x2EC to 0x300 to store additional data
	SafeWrite32(0x005FA47C, 0x300);
	SafeWrite32(0x00607582, 0x300);
	SafeWrite32(0x00607920, 0x300);
	SafeWrite32(0x0060CC36, 0x300);
	SafeWrite32(0x00659A9C, 0x300);
	SafeWrite32(0x00659D3D, 0x300);
	SafeWrite32(0x00664B09, 0x300);
	SafeWrite32(0x0066A9AA, 0x300);
	SafeWrite32(0x0068336E, 0x300);
	SafeWrite32(0x0069F2F2, 0x300);
	SafeWrite32(0x0069F3D4, 0x300);

	WriteRelJump(kPrnHook,				(UInt32)PrnHook);
	WriteRelJump(kMenuMouseButtonHook,	(UInt32)MenuMouseButtonHook);
	WriteRelJump(kUnequipTorchHook,		(UInt32)UnequipTorchHook);
}

#elif defined(SKYRIM)
EquipmentManager::EquipmentManager() {

	TheEquipmentManager = this;

}
#endif