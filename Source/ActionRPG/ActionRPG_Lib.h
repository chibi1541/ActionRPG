// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN( ActionRPG, Log, All );

// �Լ�, ���� ���
#define LOG_CALLINFO (FString(__FUNCTION__) + TEXT("(")+FString::FromInt(__LINE__)+TEXT(")"))
// UE_LOG�� Verbosity(�α� ����) �� ȣ��� �Լ�, ������ ���
#define RLOG_L(Verbosity) UE_LOG(ActionRPG,Verbosity, TEXT("%s"), *LOG_CALLINFO)
// RLOG_L�� �߰����� Text ��� ����
#define RLOG(Verbosity, Format, ...) UE_LOG(ActionRPG,Verbosity,TEXT("%s%s"),*LOG_CALLINFO,*FString::Printf(Format,##__VA_ARGS__))
// Assertion ��� Ŀ����
#define RCHECK(Expr, ...) {if(!(Expr))\
{RLOG(Error,TEXT("ASSERTION : %s"),TEXT("'"#Expr"'"));\
return __VA_ARGS__;}}

#define DEFINE_ENUM_TO_STRING(EnumType, EnumPackage) FString EnumToString(const EnumType Value) \
{ \
	static const UEnum* TypeEnum = FindObject<UEnum>(nullptr, TEXT(EnumPackage) TEXT(".") TEXT(#EnumType)); \
	return TypeEnum->GetNameStringByIndex(static_cast<int32>(Value)); \
}

#define DEFINE_ENUM_DISPLAY_NAME_TO_STRING(EnumType, EnumPackage) FString EnumDisplayNameToString(const EnumType Value) \
{ \
	static const UEnum* TypeEnum = FindObject<UEnum>(nullptr, TEXT(EnumPackage) TEXT(".") TEXT(#EnumType)); \
	return TypeEnum->GetDisplayNameTextByIndex(static_cast<int32>(Value)).ToString(); \
}

#define DECLARE_ENUM_TO_STRING(EnumType) FString EnumToString(const EnumType Value)

#define DECLARE_ENUM_DISPLAY_NAME_TO_STRING(EnumType) FString EnumDisplayNameToString(const EnumType Value)