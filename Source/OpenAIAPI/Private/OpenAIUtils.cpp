// Copyright Kellan Mythen 2023. All rights Reserved.


#include "OpenAIUtils.h"
#include "OpenAIDefinitions.h"
#include "OpenAIAPI.h"
#include "Math/UnrealMathVectorConstants.h"
#include "Math/VectorRegister.h"
#include "Modules/ModuleManager.h"

void UOpenAIUtils::setOpenAIApiKey(FString apiKey)
{
	FOpenAIAPIModule& mod = FModuleManager::Get().LoadModuleChecked<FOpenAIAPIModule>("OpenAIAPI");
	mod._apiKey = apiKey;
}

FString UOpenAIUtils::getApiKey()
{
	FOpenAIAPIModule& mod = FModuleManager::Get().LoadModuleChecked<FOpenAIAPIModule>("OpenAIAPI");
	return mod._apiKey;
}

void UOpenAIUtils::	setUseOpenAIApiKeyFromEnvironmentVars(bool bUseEnvVariable)
{
	FOpenAIAPIModule& mod = FModuleManager::Get().LoadModuleChecked<FOpenAIAPIModule>("OpenAIAPI");
	mod._useApiKeyFromEnvVariable = bUseEnvVariable;
}

bool UOpenAIUtils::getUseApiKeyFromEnvironmentVars()
{

	FOpenAIAPIModule& mod = FModuleManager::Get().LoadModuleChecked<FOpenAIAPIModule>("OpenAIAPI");
	return mod._useApiKeyFromEnvVariable;
}

FString UOpenAIUtils::GetEnvironmentVariable(FString key)
{
	FString result;
#if PLATFORM_WINDOWS
	result = FWindowsPlatformMisc::GetEnvironmentVariable(*key);
#endif
#if PLATFORM_MAC
	result = FApplePlatformMisc::GetEnvironmentVariable(*key); 
#endif

#if PLATFORM_LINUX
	result = FLinuxPlatformMisc::GetEnvironmentVariable(*key);
#endif
	return result;
}

float UOpenAIUtils::HDVectorDotProductSIMD(const FHighDimensionalVector& A, const FHighDimensionalVector& B)
{
	check(A.Components.Num() == B.Components.Num());
	check(A.Components.Num() % 4 == 0);

	VectorRegister4Float Sum = MakeVectorRegisterFloat(0.0f, 0.0f, 0.0f, 0.0f);
	for (int32 i = 0; i < A.Components.Num(); i += 4)
	{
		VectorRegister4Float AVec = VectorLoadAligned(&A.Components[i]);
		VectorRegister4Float BVec = VectorLoadAligned(&B.Components[i]);
		Sum = VectorAdd(Sum, VectorMultiply(AVec, BVec));
	}

	float Result[4];
	VectorStore(Sum, Result);
	return Result[0] + Result[1] + Result[2] + Result[3];
}

float UOpenAIUtils::HDVectorLengthSIMD(const FHighDimensionalVector& Vector)
{
	return FMath::Sqrt(HDVectorDotProductSIMD(Vector, Vector));
}

float UOpenAIUtils::HDVectorCosineSimilaritySIMD(const FHighDimensionalVector& A, const FHighDimensionalVector& B)
{
	float DotProductValue = HDVectorDotProductSIMD(A, B);
	float LengthProduct = HDVectorLengthSIMD(A) * HDVectorLengthSIMD(B);
	return DotProductValue / LengthProduct;
}

float UOpenAIUtils::HDVectorDotProduct(const FHighDimensionalVector& A, const FHighDimensionalVector& B)
{
	check(A.Components.Num() == B.Components.Num());
        
	float Sum = 0.0f;
	for (int32 i = 0; i < A.Components.Num(); i++)
	{
		Sum += A.Components[i] * B.Components[i];
	}
	return Sum;
}

float UOpenAIUtils::HDVectorLength(const FHighDimensionalVector& Vector)
{
	return FMath::Sqrt(HDVectorDotProduct(Vector, Vector));
}

float UOpenAIUtils::HDVectorCosineSimilarity(const FHighDimensionalVector& A, const FHighDimensionalVector& B)
{
	float DotProductValue = HDVectorDotProduct(A, B);
	float LengthProduct = HDVectorLength(A) * HDVectorLength(B);
	return DotProductValue / LengthProduct;
}
