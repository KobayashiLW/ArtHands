#pragma once
#include <vector>
#include <string>

using std::vector;
using std::string;


class UILang
{
public:
	enum
	{
		File,
		Edit,
		Open,
		Save,
		SaveAs,
		ChooseFile,
		ImportModel,
		ExportModel,
		Exit,
		GameObject,
		Window,
		Setting,
		Language,
		English,
		Chinese,
		LightMass,
		StyleEditor,
		Help,
		About,
		Error,
		OK,
		Cancel,
		NoShowAgain,
		Revert,
		NonReadableFile,
		OutputUVSize,
		OutputUVSlot,
		InputUVSlot,
		OutputImageSize,
		UvMapper,
		UVMargin,
		UVStretch,
		UVChart,
		StartUVMapper,
		EnableGI,
		EnableIBL,
		SampleCount,
		EnvironmentColor,
		EnvironmentIntensity,
		RayTracingZnear,
		RayTracingZfar,
		InterpolationPasses,
		InterpolationThreshold,
		StartLightMass,
		UnsupportModel,
		Process,
		Succeeded,
		ChooseModel,
		Camera,
		Assert,
		Material,
		Inspector,
		Lights,
		LightProbes,
		Meshes,
		Import,
		Export,
		TextureFrom,
		TextureFlip,
		TextureLoop,
		Texture_sRGB,
		TextureType,
		TextureSwizzle,
		Color,
		Color_sRGB,
		Strength,
		Albedo,
		AlbedoSub,
		Normal,
		NormalSub,
		Smoothness,
		Metalness,
		Specular,
		Occlusion,
		Emissive,
		EmissiveIntensity,
		ShadingModel,
		CustomA,
		CustomB,
		Summary,
		Transform,
		Position,
		Rotation,
		Scale,
		CreateProbe
	};

	enum class Lang
	{
		Chinese,
		English
	};
};

void GetLangPackage(UILang::Lang type, std::vector<std::string>& langs) noexcept;

