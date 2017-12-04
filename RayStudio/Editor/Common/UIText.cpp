#include "UIText.h"

// +----------------------------------------------------------------------
// | Project : ray.
// | All rights reserved.
// +----------------------------------------------------------------------
// | Copyright (c) 2017-2018.
// +----------------------------------------------------------------------
// | * Redistribution and use of this software in source and binary forms,
// |   with or without modification, are permitted provided that the following
// |   conditions are met:
// |
// | * Redistributions of source code must retain the above
// |   copyright notice, this list of conditions and the
// |   following disclaimer.
// |
// | * Redistributions in binary form must reproduce the above
// |   copyright notice, this list of conditions and the
// |   following disclaimer in the documentation and/or other
// |   materials provided with the distribution.

#include "UIText.h"

const char* en_lang[] = {
	" File ",
	" Edit ",
	" Open ",
	" Save ",
	" Save as ",
	" Choose File ",
	" Import Model ",
	" Export Model ",
	" Exit ",
	" GameObject ",
	" Window ",
	"Setting",
	"Language",
	"English",
	"Chinese",
	"Light Mass",
	"Style Editor",
	"Help",
	"About",
	"Error?",
	"Ok",
	"Cancel",
	"Don't ask me next time?",
	" Revert ",
	"Non readable PMX file.",
	"Output UV size:",
	"Output UV slot:",
	"Input UV slot:",
	"Output image size:",
	"Uv Mapper",
	"margin:",
	"stretch:",
	"chart:",
	" Start mapper ",
	"Enable GI",
	"Enable IBL",
	"Sample Count:",
	"Environment Color:",
	"Environment Intensity:",
	"Ray tracing znear:",
	"Ray tracing zfar:",
	"Interpolation Passes:",
	"Interpolation Threshold:",
	" Start Baking ",
	"Unsupported Model",
	"Process",
	"Succeeded",
	"Please load a model before start baking.",
	"Camera",
	"Asset",
	"Material",
	"Inspector",
	"Lights",
	"LightProbes",
	"Meshes",
	"Import...",
	"Export...",
	"Texture from:",
	"Texture Flip:",
	"Texture loop:",
	"Texture (sRGB):",
	"Texture type:",
	"Texture swizzle:",
	"Color:",
	"Color (sRGB):",
	"Strength:",
	"Albedo",
	"AlbedoSub",
	"Normal",
	"NormalSub",
	"Smoothness:",
	"Metalness:",
	"Specular",
	"Occlusion:",
	"Emissive:",
	"Intensity:",
	"ShadingModel",
	"CustomA:",
	"CustomB:",
	"Summary: %s",
	"Transform",
	"Position",
	"Rotation",
	"Scale",
	"Light Probe"
};

const wchar_t* cn_lang[] = {
	L" �ļ� ",
	L" �༭ ",
	L"��",
	L"����",
	L"���Ϊ",
	L"ѡ���ļ�",
	L"����ģ��",
	L"����ģ��",
	L"�˳�",
	L" ��Ϸ���� ",
	L" ���� ",
	L"����",
	L"����",
	L"Ӣ��",
	L"����",
	L"���պ���",
	L"��������",
	L" ���� ",
	L"����",
	L"����?",
	L"ȷ��",
	L"ȡ��",
	L"��һ�β�����ʾ?",
	L" ���� ",
	L"���ɶ���PMX�ļ�",
	L"չ����UV��С:",
	L"�����UV���:",
	L"�����UV���:",
	L"�����ͼ���С:",
	L"��������ȫչ��",
	L"չ�����:",
	L"�ϲ�UV��С��ֵ:",
	L"չ�������������:",
	L" ��ʼչ�� ",
	L"ʹ��ȫ�ֹ�",
	L"ʹ��HDR��Դ",
	L"������:",
	L"��������ɫ:",
	L"������ǿ:",
	L"׷�ٵ���С��Χ",
	L"׷�ٵ����Χ",
	L"��ֵ����",
	L"��ֵ��ֵ",
	L" ��ʼ���� ",
	L"��֧�ֵ�ģ��",
	L"������",
	L"��ʾ",
	L"��������һ��ģ��",
	L"���",
	L"��Դ",
	L"����",
	L"����",
	L"��Դ",
	L"��Դ̽��",
	L"ģ��",
	L"����...",
	L"����...",
	L"����ģʽ:",
	L"�������귭ת:",
	L"�������:",
	L"���� (sRGB):",
	L"��������:",
	L"����Ƶ��:",
	L"��ɫ:",
	L"��ɫ (sRGB):",
	L"ǿ��:",
	L"������ɫ",
	L"�ι���ɫ",
	L"������",
	L"�η���",
	L"�⻬��:",
	L"������:",
	L"�߹�",
	L"�������ڱ�:",
	L"�Է���",
	L"ǿ��:",
	L"��ɫģ��",
	L"CustomA:",
	L"CustomB:",
	L"��Ҫ %s",
	L"�任",
	L"����",
	L"��ת",
	L"����",
	L"����̽��"
};

std::vector<std::string> cn_lang_utf8;

void GetLangPackage(UILang::Lang type, std::vector<std::string>& langs) noexcept
{
	
	//static_assert(sizeof(cn_lang) == sizeof(en_lang));

	langs.clear();

	if (type == UILang::Lang::English)
	{
		for (auto& it : en_lang)
			langs.push_back(it);
	}
	else
	{
		char buffer[260];

		/*if (cn_lang_utf8.empty())
		{
			for (auto& it : cn_lang)
			{
				if ((it, strlen(it), buffer, 260))
					cn_lang_utf8.push_back(buffer);
			}
		}

		for (auto& it : cn_lang_utf8)
			langs.push_back(it);*/
	}
}