// No Copyright - Ancient is created by Haluk Oral for demonstrate his skills. See: README.md

#pragma once

#include "CoreMinimal.h"
#include "Ancient/Ancient.h"
#include "DrawDebugHelpers.h"
#include "DebuggerLibrary.generated.h"

//Current Class Name + Function Name where this is called!
#define PRINT_CUR_CLASS_FUNC (FString(__FUNCTION__))

//Current Class where this is called!
#define PRINT_CUR_CLASS (FString(__FUNCTION__).Left(FString(__FUNCTION__).Find(TEXT(":"))) )

//Current Function Name where this is called!
#define PRINT_CUR_FUNC (FString(__FUNCTION__).Right(FString(__FUNCTION__).Len() - FString(__FUNCTION__).Find(TEXT("::")) - 2 ))

//Current Line Number in the code where this is called!
#define PRINT_CUR_LINE  (FString::FromInt(__LINE__))

//Current Class and Line Number where this is called!
#define PRINT_CUR_CLASS_LINE (PRINT_CUR_CLASS + "(" + PRINT_CUR_LINE + ")")

//Current Class, Function and Line Number where this is called!
#define PRINT_CUR_CLASS_FUNC_LINE (PRINT_CUR_CLASS_FUNC + "(" + PRINT_CUR_LINE + ")")

//Current Function Signature where this is called!
#define PRINT_CUR_FUNCSIG (FString(__FUNCSIG__))

// Debug Message On Screen!
#define MSG(Param1)								(GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, *(Param1)) )

#define MSG_TWO_PARAM(Param1,Param2)			(GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, *(PRINT_CUR_CLASS_FUNC_LINE + ": " + Param1 + " " + Param2)) )

#define MSG_F(Param1,Param2)					(GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, *(PRINT_CUR_CLASS_FUNC_LINE + ": " + Param1 + " " + FString::SanitizeFloat(Param2))) )

#define MSG_NULL(Param1, Param2, Param3)		(GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, *(FString::FromInt(Param1) + ". object is not set on " + Param2 + " at " + FString::FromInt(Param3)) ) )

//UE LOG!
#define LOG(Param1) 							UE_LOG(LogAncient, Warning, TEXT("%s: %s"), *PRINT_CUR_CLASS_LINE, *FString(Param1))

#define LOGTYPE(LogType,Param2) 				UE_LOG(LogAncient, Warning, TEXT("%s: %s"), *PRINT_CUR_CLASS_LINE, *FString(Param2))

#define LOGF(Param1,Param2) 					UE_LOG(LogAncient, Warning, TEXT("%s: %s %f"), *PRINT_CUR_CLASS_LINE, *FString(Param1),Param2)

#define LOGFTYPE(LogType,Param1,Param2) 		UE_LOG(LogAncient, Warning, TEXT("%s: %s %f"), *PRINT_CUR_CLASS_LINE, *FString(Param1),Param2)

#define LOGM(FormatString , ...)				UE_LOG(LogAncient, Warning, TEXT("%s: %s"), *PRINT_CUR_CLASS_LINE, *FString::Printf(TEXT(FormatString), ##__VA_ARGS__ ) )

#define DRAWLINE(Start, End) 					DrawDebugLine(GetWorld(), Start, End, FColor(255, 0, 0), false, -1, 0, 10);

#define DRAWBOX(Center, Box, LifeTime)			DrawDebugBox(GetWorld(), Center, Box, FColor::Purple, false, LifeTime, 0, 10)

#define DRAWSPHERE(Center, Radius, Duration)	DrawDebugSphere(GetWorld(), Center, Radius, 12, FColor::Purple, false, Duration);

#define DRAWARROW(Start, End, Size, LifeTime)	DrawDebugDirectionalArrow(GetWorld(), Start, End, Size, FColor::Magenta, true, LifeTime, 0, 5.f);

// CHECK
#define CHECKVALID(Num, Func, Line,  ...) 		UDebuggerLibrary::Test(Num, Func, Line, ##__VA_ARGS__);

UCLASS()
class ANCIENT_API UDebuggerLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/**
	* Test validity of object
	*
	* @param	Num				Number of function arguments. Must be first argument
	* @param	__FUNCTION__	Function Name. Must be second argument
	* @param	__LINE__		Line of function called. Must be third argument
	* @param	Uobject*		The objects to test
	* @return	Return true if the object is usable: non-null and not pending kill
	*/
	static bool Test(int32 ArgumentNumber, ...)
	{
		va_list args;
		va_start(args, ArgumentNumber);

		UPROPERTY()
		TArray<UObject*> Objects;
		
		UPROPERTY()
		TArray<FString> ObjectsName;

		FString FuncName;
		int32 Line = 0;

		Objects.Empty();
		
		for (int i = 0; i < ArgumentNumber; ++i)
		{
			if (i < ArgumentNumber)
			{
				Objects.Add(va_arg(args, UObject*));
			}
			else if (i == 1)
			{
				FuncName = FString(va_arg(args, char*));
			}
			else if (i == 2)
			{
				Line = va_arg(args, int32);
			}
		}
		
		for (int i = 0; i < Objects.Num(); ++i)
		{
			if (Objects[i] == nullptr || Objects[i]->IsPendingKill())
			{
				MSG_NULL(i, FuncName, Line);
				return false;
			}
		}

		va_end(args);
		return true;
	}
};
