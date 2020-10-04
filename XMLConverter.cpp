// ---------------------------------------------------------------------------

#pragma hdrstop

#include "XMLConverter.h"
#include "MainUnit.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)

const char xmlheader[] = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
const char *bodyheader[] =
{"<primitives>\n", "\n</primitives>"};

const char *primtag[] =
{"  <primitive>\n", "  </primitive>\n"};

const char *coords[] =
{"    <coords>\n", "    </coords>\n"};
const char *colors[] =
{"    <colors>\n", "    </colors>\n"};

const char *_linewidth_ = "linewidth";
const char *_linestyle_ = "linestyle";

const char *_float_ = "float";
const char *_angle_ = "angle";

__fastcall XMLConverter::XMLConverter(char* filename, bool save) : correct(false), file(NULL)
{
	int size = strlen(filename) + 10;
	char *chrs = new char[size];
	memset(chrs, 0, size);
	strcpy(chrs, filename);
	const char * sxml = ".sxml";
	if (!strstr(chrs, sxml))
	{
		strcpy(&chrs[strlen(chrs)], sxml);
	}
	fopen_s(&file, chrs, (save) ? "wb" : "rb");
	delete[]chrs;
	correct = true;
	if (save)
	{
		fwrite(xmlheader, strlen(xmlheader), 1, file);
		fwrite(bodyheader[0], strlen(bodyheader[0]), 1, file);
	}
	this->save = save;
	name = chrs;
	CoInitialize(0);
}

XMLConverter::~XMLConverter()
{
	if (file)
	{
		if (save)
		{
			fwrite(bodyheader[1], strlen(bodyheader[1]), 1, file);
		}
		fclose(file);
		file = NULL;
	}
}

void __fastcall XMLConverter::saveElement(Primitive *primitive)
{
	fwrite(primtag[0], strlen(primtag[0]), 1, file);
	char ltype[32] =
	{0};
	sprintf_s(ltype, sizeof ltype, "    <t>%s</t>\n", primitive->type());
	fwrite(ltype, strlen(ltype), 1, file);

	fwrite(coords[0], strlen(coords[0]), 1, file);
	auto coordlist = primitive->getCoords();
	for (int i = 0; i < 4; ++i)
	{
		char str[64] =
		{0};
		sprintf_s(str, sizeof str, "      <%s>%f</%s>\n", _float_, coordlist[i], _float_);
		fwrite(str, strlen(str), 1, file);
	}
	fwrite(coords[1], strlen(coords[1]), 1, file);
	char linewidth[128] =
	{0};
	sprintf_s(linewidth, sizeof linewidth, "    <%s>%d</%s>\n", _linewidth_, *primitive->getWidthLine(), _linewidth_);
	fwrite(linewidth, strlen(linewidth), 1, file);
	sprintf_s(linewidth, sizeof linewidth, "    <%s>%d</%s>\n", _linestyle_, *primitive->getLineStyle(), _linestyle_);
	fwrite(linewidth, strlen(linewidth), 1, file);

	fwrite(colors[0], strlen(colors[0]), 1, file);
	float *colorlist = primitive->getStyle();
	for (int i = 0; i < 8; ++i)
	{
		char str[64] =
		{0};
		sprintf_s(str, sizeof str, "      <%s>%f</%s>\n", _float_, colorlist[i], _float_);
		fwrite(str, strlen(str), 1, file);
	}
	fwrite(colors[1], strlen(colors[1]), 1, file);
	{
		char str[64] =
		{0};
		sprintf_s(str, sizeof ltype, "    <angle>%f</angle>\n", *primitive->getInsideAngle());
		fwrite(str, strlen(str), 1, file);

	} fwrite(primtag[1], strlen(primtag[1]), 1, file);
}

void __fastcall XMLConverter::parseDocument(PrimitiveManager *primitiveManager)
{
	if (file)
	{
		fclose(file);
		file = NULL;
	}
	MainForm->XMLDocument1->LoadFromFile(&name[0]);
	int p = MainForm->XMLDocument1->ChildNodes->GetNode(1)->ChildNodes->GetCount();
	auto work = MainForm->XMLDocument1->ChildNodes->GetNode(1);
	for (int i = 0; i < p; ++i)
	{
		auto primitive = work->ChildNodes->GetNode(i);
		auto tTag = primitive->ChildNodes->FindNode("t");
		if (!tTag)
		{
			ShowMessage("Ошибка XML");
			return;
		}
		Primitive *prim = NULL;
		if (tTag->GetText() == "line")
		{
			prim = new LinePrimitive();
		}
		else if (tTag->GetText() == "ellipse")
		{
			prim = new EllipsePrimitive();
		}
		else
		{
			prim = new RectPrimitive();
		}
		if (!prim)
		{
			ShowMessage("Ошибка в создание примитива");
			return;
		}
		primitiveManager->addPrimitive(prim);
		////------------Координаты----------------------------
		auto tCoords = primitive->ChildNodes->FindNode("coords");

		if (!tCoords)
		{
			ShowMessage("Ошибка XML");
			return;
		}
		int amnt = tCoords->ChildNodes->GetCount();
		auto coordprim = prim->getCoords();
		for (int j = 0; j < amnt; ++j)
		{
			coordprim[j] = getFloatFromWString(tCoords->ChildNodes->GetNode(j)->GetText().c_str());
		}
		// ----Тип линии-----------------
		auto tlinewidth = primitive->ChildNodes->FindNode("linewidth");
		if (!tlinewidth)
		{
			ShowMessage("Ошибка XML");
			return;
		}
		*(prim->getWidthLine()) = StrToInt(tlinewidth->GetText());

		auto tlinestyle = primitive->ChildNodes->FindNode("linestyle");
		if (!tlinewidth)
		{
			ShowMessage("Ошибка XML");
			return;
		}

		int *style = (int *)(prim->getLineStyle());
		*style = StrToInt(tlinestyle->GetText());
		// ---------------------Настройка цвета-------------------
		auto tColors = primitive->ChildNodes->FindNode("colors");
		if (!tColors)
		{
			ShowMessage("Ошибка XML");
			return;
		}
		amnt = tColors->ChildNodes->GetCount();
		coordprim = prim->getStyle();

		for (int j = 0; j < amnt; ++j)
		{
			coordprim[j] = getFloatFromWString(tColors->ChildNodes->GetNode(j)->GetText().c_str());
		}
		////-----------Считывание угла поворота-------------------------
		auto tAngle = primitive->ChildNodes->FindNode("angle");
		if (!tAngle)
		{
			ShowMessage("Ошибка XML");
			return;
		}
		*(prim->getInsideAngle()) = getFloatFromWString(tAngle->GetText().c_str());
		// -------------------------------------
		prim->convertRectToCoord();
		prim->updateResource(false);
		MainForm->addPrimitiveToList();
	}
}

float __fastcall XMLConverter::getFloatFromWString(std::wstring txt)
{
	//ShowMessage(&txt[0]);
	int z = txt.find(L'.');
	if (z != -1)
	{
		txt[z] = L',';
	}
	return StrToFloat(&txt[0]);
}
