// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN( ActionRPG, Log, All );

// 함수, 라인 출력
#define LOG_CALLINFO (FString(__FUNCTION__) + TEXT("(")+FString::FromInt(__LINE__)+TEXT(")"))
// UE_LOG로 Verbosity(로깅 수준) 와 호출된 함수, 라인을 출력
#define RLOG_L(Verbosity) UE_LOG(ActionRPG,Verbosity, TEXT("%s"), *LOG_CALLINFO)
// RLOG_L에 추가적인 Text 출력 가능
#define RLOG(Verbosity, Format, ...) UE_LOG(ActionRPG,Verbosity,TEXT("%s%s"),*LOG_CALLINFO,*FString::Printf(Format,##__VA_ARGS__))
// Assertion 기능 커스텀
#define RCHECK(Expr, ...) {if(!(Expr))\
{RLOG(Error,TEXT("ASSERTION : %s"),TEXT("'"#Expr"'"));\
return __VA_ARGS__;}}