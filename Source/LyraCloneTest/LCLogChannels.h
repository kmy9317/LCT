// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Logging/LogMacros.h"

class UObject;

LYRACLONETEST_API DECLARE_LOG_CATEGORY_EXTERN(LogLC, Log, All);

LYRACLONETEST_API FString GetClientServerContextString(UObject* ContextObject = nullptr);